#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width();
	height = Engine::Get().Height();

	obstaclePosition.clear();

	// A-Star 타이머 기본 설정
	aStarTimer.SetTargetTime(0.2f);
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 플레이어 & 목표점 & 장애물 위치 설정 입력 받기
	// A-Star 시작 입력 받기
	if (aStarState == AStarState::IsBefore)
	{
		IsBefore();
		return;
	}

	// A-Star 수행
	if (aStarState == AStarState::IsPlaying)
	{
		AStar();
		return;
	}

	// 길찾기 계산 경로 렌더 틱
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		aStarTimer.Tick(deltaTime);
		return;
	}

	// 최종 경로 렌더 틱
	if (aStarState == AStarState::IsRenderFinalPath)
	{
		aStarTimer.Tick(deltaTime);
		return;
	}

	// 다음 진행 메뉴 입력 받기
	if (aStarState == AStarState::IsEnded)
	{
		IsEnded();
		return;
	}
}

void AStarLevel::Render()
{
	super::Render();

	// 외곽 경계 출력
	PrintOutLine();

	// 플레이어 & 목표점 & 장애물 출력
	PrintPlayer();
	PrintDestination();
	PrintObstacle();

	// 길찾기로 계산된 경로들 출력
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderComputepath();
		return;
	}

	// 최종 경로 출력
	if (aStarState == AStarState::IsRenderFinalPath)
	{
		IsRenderFinalPath();
		return;
	}

	// 다음 진행 메뉴 출력
	if (aStarState == AStarState::IsEnded ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderEnded();
		return;
	}
}

void AStarLevel::SetPlayerPosition()
{
	// 좌클릭: 플레이어 위치 설정
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			for (const auto& i : obstaclePosition)
			{
				if (mousePos == i) return;
			}
			playerPosition = mousePos;
			hasPlayer = true;
		}
	}
}

void AStarLevel::SetDestinationPosition()
{
	// 우클릭: 목표 지점 설정
	if (Input::Get().GetMouseRightButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			for (const auto& i : obstaclePosition)
			{
				if (mousePos == i) return;
			}
			destinationPosition = mousePos;
			hasTarget = true;
		}
	}
}

bool AStarLevel::SetObstaclePosition()
{
	// 좌클릭 + 스페이스 바: 장애물 위치 설정
	if (Input::Get().GetMouseLeftButton() && Input::Get().GetKey(VK_SPACE))
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// 이미 해당 위치에 장애물이 있는지 검사
			bool findObstacle = false;

			for (const auto& i : obstaclePosition)
			{
				// 해당 위치가 플레이어나 목표 위치인 경우 패스
				if (i == playerPosition || i == destinationPosition) continue;

				// 이미 해당 위치에 장애물이 있으면 루프 종료
				if (i == mousePos)
				{
					findObstacle = true;
					break;
				}
			}

			if (!findObstacle)
			{
				obstaclePosition.emplace_back(mousePos);
				return true;
			}
		}
	}
	return false;
}

void AStarLevel::PrintPlayer()
{
	if (hasPlayer)
	{
		Engine::Get().WriteToBuffer(playerPosition, "P", Color::Green);
	}
}

void AStarLevel::PrintDestination()
{
	if (hasTarget)
	{
		Engine::Get().WriteToBuffer(destinationPosition, "D", Color::Red);
	}
}

void AStarLevel::PrintObstacle()
{
	for (const auto& i : obstaclePosition)
	{
		Engine::Get().WriteToBuffer(i, "#", Color::Yellow);
	}
}

void AStarLevel::IsBefore()
{
	// 장애물을 생성한 프레임에는 플레이어 이동 X
	if (!SetObstaclePosition())
	{
		SetPlayerPosition();
	}
	SetDestinationPosition();

	// 길찾기 시작 준비가 된 경우
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 길찾기 시작중으로 변경
		aStarState = AStarState::IsPlaying;
		return;
	}
}

void AStarLevel::IsEnded()
{
	// 재시작 입력
	if (Input::Get().GetKey(VK_RETURN))
	{
		Game::Get().StartAStar(currnetRenderSpeed);
	}

	// 종료 입력
	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Engine::Get().Quit();
	}
}

void AStarLevel::IsRenderComputepath()
{
	static int index = 0;

	if (!aStarTimer.IsTimeout())
	{
		return;
	}

	if (index < pathPosition.size())
	{
		for (int i = 0; i < index; ++i)
		{
			Engine::Get().WriteToBuffer(pathPosition[i], "@", Color::Intensity);
		}
		++index;

		// 타이머 초기화
		aStarTimer.Reset();

		// Todo:## 현재 길찾기 렌더 속도에 따라 타이머 설정해야 됨
		aStarTimer.SetTargetTime(0.2f);
	}
	else
	{
		if (aStarState == AStarState::IsFailed)
		{
			aStarState = AStarState::IsFailed;
		}
		else
		{
			aStarState = AStarState::IsRenderFinalPath;
		}

		aStarTimer.Reset();
	}
}

void AStarLevel::IsRenderFinalPath()
{
	static int index = 0;

	if (!aStarTimer.IsTimeout())
	{
		return;
	}

	if (index < finalPath.size())
	{
		for (int i = 0; i < index; ++i)
		{
			Engine::Get().WriteToBuffer(finalPath[i], "*", Color::Green);
		}
		++index;

		// 타이머 초기화
		aStarTimer.Reset();

		// Todo:## 현재 길찾기 렌더 속도에 따라 타이머 설정해야 됨
		aStarTimer.SetTargetTime(0.2f);
	}
	else
	{
		aStarState = AStarState::IsEnded;
		aStarTimer.Reset();
	}
}

void AStarLevel::IsRenderEnded()
{
	// 길찾기 종료 문구 출력
	if (aStarState == AStarState::IsFailed)
	{
		char buffer1[25]{ "[ !- 길찾기 실패 -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 2, width + 2), buffer1, Color::Red);
	}
	else if (aStarState == AStarState::IsEnded)
	{
		char buffer1[25]{ "[ !- 길찾기 성공 -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 2, width + 2), buffer1, Color::Green);
	}

	// 재시작 문구 출력
	char buffer2[50]{ "[ 재시작 >> \"Enter\" ]" };
	Engine::Get().WriteToBuffer(Vector2(height - 1, width + 2), buffer2, Color::White);

	// 종료 문구 출력
	char buffer3[50]{ "[ 종료 >> \"ESC\" ]" };
	Engine::Get().WriteToBuffer(Vector2(height - 0, width + 2), buffer3, Color::White);
}

void AStarLevel::PrintOutLine()
{
	for (int i = 0; i < width; ++i)
	{
		Engine::Get().WriteToBuffer({ i, height - 1 }, "-", Color::Blue, 0);
	}

	for (int i = 0; i < height; ++i)
	{
		Engine::Get().WriteToBuffer({ width - 1, i }, "|", Color::Blue, 0);
	}
}

void AStarLevel::AStar()
{
	// 방향 벡터 및 비용
	Vector2 dirs[] =
	{
		// 상하좌우
		{-1, 0}, {0, -1}, {1, 0}, {0, 1},

		// 대각선 (좌상, 좌하, 우상, 우하)
		{-1, -1}, {1, -1}, {1, 1}, {-1, 1}
	};

	// 이동 비용
	const double orthCost = 10.0; // 직선 이동 비용
	const double diagCost = orthCost * std::sqrt(2.0); // 대각선 이동 비용

	// 시작 데이터 초기화
	{
		// 방문 배열 초기화
		closedList.assign(height, std::vector<bool>(width, false));

		// 베스트 배열 초기화 (모두 최대값으로 초기화)
		best.assign(height, std::vector<double>(width, std::numeric_limits<double>::infinity()));


		// 시작 노드관련 초기화
		best[playerPosition.y][playerPosition.x] = 0.0;
		parent[playerPosition] = playerPosition;

		double h = Heuristic(playerPosition, destinationPosition, orthCost, diagCost);
		openList.push(Node{ h, 0.0, playerPosition });
	}

	while (!openList.empty())
	{
		// #1: 제일 좋은 후보를 찾음
		Node node = openList.top();
		openList.pop();

		// #2: Best값을 비교해서 해당 좌표를 기준으로 비용이 더 싼 노드가 있는지 확인
		if (node.gCost > best[node.pos.y][node.pos.x]) continue;

		// #3: 목표 위치에 도달했는지 확인
		if (node.pos == destinationPosition)
		{
			// 부모를 추적해서 최종 경로를 저장
			Vector2 pos = destinationPosition;
			pathPosition.emplace_back(pos);

			while (true)
			{
				// 부모가 자신과 같으면 시작 위치이므로, 루프 종료
				if (pos == parent[pos]) break;

				pos = parent[pos];
				pathPosition.emplace_back(pos);
			}

			// 배열을 뒤집어, 끝 -> 시작에서 시작 -> 끝으로 정리
			std::reverse(pathPosition.begin(), pathPosition.end());

			// 계산한 좌표를 렌더하도록 상태 변경
			aStarState = AStarState::IsRenderComputepath;
			return;
		}

		// #4: 방문 체크
		closedList[node.pos.y][node.pos.x] = true;

		// #5: 방향 순회
		for (int i = 0; i < 8; ++i)
		{
			Vector2 nextPos = node.pos + dirs[i];

			// ##1: 갈 수 있는 지역인지 확인 (범위, 장애물 체크)
			if (!CanGo(node.pos, nextPos)) continue;

			// ##2: 이미 방문한 노드인지 확인
			if (closedList[nextPos.y][nextPos.x]) continue;

			// 길찾기에 계산된 경로에 추가 (시각화 용도)
			pathPosition.emplace_back(nextPos);

			// ##3: 비용 계산
			double nextPosGCost = node.gCost + (i < 4 ? orthCost : diagCost);

			// ##4: 더 좋은 길을 찾았는지 확인
			if (nextPosGCost >= best[nextPos.y][nextPos.x]) continue;

			// ##5: 더 좋은 길이라면, 갱신
			best[nextPos.y][nextPos.x] = nextPosGCost;
			parent[nextPos] = node.pos;

			// ##6: OpenList에 노드 추가
			double fCost = nextPosGCost + Heuristic(nextPos, destinationPosition, orthCost, diagCost);
			openList.push(Node{ fCost, nextPosGCost, nextPos });
		}
	}

	// 여기 왔다는거는 경로를 못찾았다는 의미
	aStarState = AStarState::IsFailed;
}

bool AStarLevel::CanGo(const Vector2& from, const Vector2& next)
{
	// 유효 범위 체크
	if (next.x < 0 || next.x >= width || next.y < 0 || next.y >= height)
	{
		return false;
	}

	// 가려는 위치에 장애물이 있는지 확인
	// 대각선이라면, 인접한 가로/세로에 장애물이 있는지 추가 확인

	int dx = next.x - from.x;
	int dy = next.y - from.y;

	Vector2 horizontal(from.x + dx, from.y); // 가로
	Vector2 vertical(from.x, from.y + dy); // 세로

	for (const auto& i : obstaclePosition)
	{
		if (i == next || i == horizontal || i == vertical) return false;
	}

	return true;
}
