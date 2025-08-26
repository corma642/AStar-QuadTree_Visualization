#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width() - tempWidth;
	height = Engine::Get().Height() - tempHeight;

	// A-Star 타이머 기본 설정
	aStarTimer.Reset();
	aStarTimer.SetTargetTime(0.2f);
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 플레이어 & 목표점 & 장애물 위치 설정 입력 받기
	// A-Star 시작 입력 받기
	if (aStarState == AStarState::IsBefore)
	{
		IsTickBefore();
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
		aStarTimer.Tick(deltaTime);
		IsTickEnded();
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

	if (aStarState == AStarState::IsBefore)
	{
		IsRenderBefore();
		return;
	}

	// 길찾기로 계산된 경로들 출력
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderPlaying();
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
		IsRenderFinalPath();
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

		// 위치가 목표 위치와 충돌하면 리턴
		if (mousePos == destinationPosition) return;

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// 위치가 장애물과 충돌하면 리턴
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

		// 위치가 플레이어 위치와 충돌하면 리턴
		if (mousePos == playerPosition) return;

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// 위치가 장애물과 충돌하면 리턴
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

		// 해당 위치가 플레이어나 목표 위치인 경우 패스
		if (mousePos == playerPosition || mousePos == destinationPosition) return false;

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// 이미 해당 위치에 장애물이 있는지 검사
			bool findObstacle = false;

			for (const auto& i : obstaclePosition)
			{
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
		Engine::Get().WriteToBuffer(playerPosition, "P", Color::LightGreen);
	}
}

void AStarLevel::PrintDestination()
{
	if (hasTarget)
	{
		Engine::Get().WriteToBuffer(destinationPosition, "D", Color::LightRed);
	}
}

void AStarLevel::PrintObstacle()
{
	for (const auto& i : obstaclePosition)
	{
		Engine::Get().WriteToBuffer(i, "#", Color::LightYellow);
	}
}

void AStarLevel::IsTickBefore()
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

	// 종료 입력
	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Engine::Get().Quit();
	}
}

void AStarLevel::IsTickEnded()
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

void AStarLevel::IsRenderBefore()
{
	// 조작키 문구 출력
	char buffer1[50]{ "[ 플레이어 위치 설정 ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ 목표 위치 설정 ]" };
	char buffer4[50]{ "> \"Right Click\"" };

	char buffer5[50]{ "[ 장애물 위치 설정 ]" };
	char buffer6[50]{ "> \"Space bar + Left Click\"" };

	char buffer7[50]{ "[ A* 시작 ]" };
	char buffer8[50]{ "> \"Enter\"" };

	char buffer9[50]{ "[ 종료 ]" };
	char buffer10[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, 0), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 1), buffer2, Color::LightGreen);

	Engine::Get().WriteToBuffer(Vector2(width + 1, 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 4), buffer4, Color::LightRed);

	Engine::Get().WriteToBuffer(Vector2(width + 1, 6), buffer5, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 7), buffer6, Color::LightYellow);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 6), buffer7, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 5), buffer8, Color::LightSkyBlue);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer9, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer10, Color::LightRed);
}

void AStarLevel::IsRenderPlaying()
{
	char buffer1[50]{ "[ >>>>>>>>>>>>>>> ]" };
	char buffer2[50]{ "   길 찾는 중..." };
	char buffer3[50]{ "[ >>>>>>>>>>>>>>> ]" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 4), buffer1, Color::LightViolet);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer2, Color::LightYellow);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer3, Color::LightViolet);
}

void AStarLevel::IsRenderComputepath()
{
	if (!aStarTimer.IsTimeout())
	{
		for (int i = 0; i < renderComputePathIndex; ++i)
		{
			// 길찾기 실패인 경우, 목표 위치와 가장 근접한 노드까지만 출력 후 종료
			if (aStarState == AStarState::IsFailed)
			{
				if (pathPosition[i] == bestNearingNode.pos)
				{
					renderComputePathIndex = (int)pathPosition.size();
					return;
				}
			}

			// 목표 위치 발견 시, 종료
			if (pathPosition[i] == destinationPosition)
			{
				renderComputePathIndex = (int)pathPosition.size();
				return;
			}

			Engine::Get().WriteToBuffer(pathPosition[i], "@", Color::Intensity);
		}
		return;
	}

	if (renderComputePathIndex < pathPosition.size())
	{
		++renderComputePathIndex;

		// 타이머 초기화
		aStarTimer.Reset();

		// Todo:## 현재 길찾기 렌더 속도에 따라 타이머 설정해야 됨
		aStarTimer.SetTargetTime(0.07f);
	}
	// A* 계산 경로를 다 출력한 경우
	else
	{
		// A*가 성공했으면, 최종 경로를 출력하도록 상태 변경
		if (aStarState != AStarState::IsFailed)
		{
			aStarState = AStarState::IsRenderFinalPath;
		}

		aStarTimer.Reset();
	}
}

void AStarLevel::IsRenderFinalPath()
{
	if (!aStarTimer.IsTimeout())
	{
		for (int i = 0; i < renderFinalPathIndex; ++i)
		{
			Engine::Get().WriteToBuffer(finalPath[i], "*", Color::LightGreen);
		}
		return;
	}

	if (renderFinalPathIndex < finalPath.size())
	{
		++renderFinalPathIndex;

		// 타이머 초기화
		aStarTimer.Reset();

		// Todo:## 현재 길찾기 렌더 속도에 따라 타이머 설정해야 됨
		aStarTimer.SetTargetTime(0.07f);
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
		char buffer1[50]{ "[ ! *길찾기 실패* ! ]" };
		Engine::Get().WriteToBuffer(Vector2(width + 1, height - 8), buffer1, Color::LightRed);
	}
	else if (aStarState == AStarState::IsEnded)
	{
		char buffer1[50]{ "[ ! *길찾기 성공* ! ]" };
		Engine::Get().WriteToBuffer(Vector2(width + 1, height - 8), buffer1, Color::LightGreen);
	}

	// 재시작 문구 출력
	char buffer1[50]{ "[ 재시작 ]" };
	char buffer2[50]{ "> \"Enter\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 6), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 5), buffer2, Color::LightGreen);

	// 종료 문구 출력
	char buffer3[50]{ "[ 종료 ]" };
	char buffer4[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer4, Color::LightRed);
}

void AStarLevel::PrintOutLine()
{
	for (int i = 0; i < width; ++i)
	{
		Engine::Get().WriteToBuffer({ i, height - 1 }, "-", Color::LightBlue, 0);
	}

	for (int i = 0; i < height; ++i)
	{
		Engine::Get().WriteToBuffer({ width - 1, i }, "|", Color::LightBlue, 0);
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

		bestNearingNode.gCost = std::numeric_limits<double>::infinity();
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
			// 길찾기 계산 경로 저장 배열 중복 제거
			pathPosition.erase(std::unique(pathPosition.begin(), pathPosition.end()), pathPosition.end());

			// 부모를 추적해서 최종 경로를 저장
			Vector2 pos = destinationPosition;

			while (true)
			{
				pos = parent[pos];

				// 부모가 자신과 같으면 시작 위치이므로, 루프 종료
				if (pos == parent[pos]) break;

				finalPath.emplace_back(pos);
			}

			// 배열을 뒤집어, 끝 -> 시작에서 시작 -> 끝으로 정리
			std::reverse(finalPath.begin(), finalPath.end());

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

			// ##3: 비용 계산
			double nextPosGCost = node.gCost + (i < 4 ? orthCost : diagCost);

			// ##4: 더 좋은 길을 찾았는지 확인
			if (nextPosGCost >= best[nextPos.y][nextPos.x]) continue;

			// ##5: 더 좋은 길이라면, 갱신
			best[nextPos.y][nextPos.x] = nextPosGCost;
			parent[nextPos] = node.pos;

			// ##6: OpenList에 노드 추가
			double fCost = nextPosGCost + Heuristic(nextPos, destinationPosition, orthCost, diagCost);
			openList.push(Node(fCost, nextPosGCost, nextPos));

			// 길찾기에 계산된 경로에 추가 (시각화 용도)
			pathPosition.emplace_back(nextPos);

			// 목표 위치와 가장 근접한 노드 갱신
			if (nextPosGCost < bestNearingNode.gCost)
			{
				bestNearingNode = node;
			}
		}
	}

	// 여기 왔다는거는 경로를 못찾았다는 의미
	pathPosition.erase(std::unique(pathPosition.begin(), pathPosition.end()), pathPosition.end());
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