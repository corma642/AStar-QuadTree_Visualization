#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width();
	height = Engine::Get().Height();

	obstaclePosition.clear();

	// 타이머 설정 (시각화되는 시간 설정)
	aStarTimer.SetTargetTime(0.1f);
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (aStarState == AStarState::IsPlaying)
	{
		if (AStarVisualization(deltaTime))
		{
			return;
		}
	}

	// 플레이어 & 목표점 & 장애물 위치 설정
	if (aStarState == AStarState::IsBefore)
	{
		// 장애물을 생성한 프레임에는 플레이어 이동 X
		if (!SetObstaclePosition())
		{
			SetPlayerPosition();
		}
		SetDestinationPosition();
	}

	// 길찾기 시작 준비가 된 경우
	if (aStarState == AStarState::IsBefore)
	{
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			// 길찾기 시작 트리거 활성화
			aStarState = AStarState::IsReady;
			return;
		}
	}

	// 길찾기 시작 시, A-Star 길찾기 시작
	if (aStarState == AStarState::IsReady)
	{
		aStarState = AStarState::IsPlaying;
		return;
	}

	// 시각화가 모두 끝난 경우
	if (aStarState == AStarState::IsEnded)
	{
		if (Input::Get().GetKey(VK_RETURN))
		{
			Game::Get().StartAStar(currnetRenderSpeed);
		}

		if (Input::Get().GetKey(VK_ESCAPE))
		{
			Engine::Get().Quit();
		}
	}
}

void AStarLevel::Render()
{
	super::Render();

	// 외곽 경계 출력
	PrintOutLine();

	// 플레이어 & 목표 위치 출력
	if (hasPlayer)
	{
		Engine::Get().WriteToBuffer(playerPosition, "P", Color::Green);
	}
	if (hasTarget)
	{
		Engine::Get().WriteToBuffer(destinationPosition, "D", Color::Red);
	}

	// 장애물 출력
	for (const auto& i : obstaclePosition)
	{
		Engine::Get().WriteToBuffer(i, "#", Color::Intensity);
	}

	if (aStarState == AStarState::IsPlaying)
	{
		for (const auto& i : pathPosition)
		{
			Engine::Get().WriteToBuffer(i, "@", Color::Blue);
		}
	}

	if (aStarState == AStarState::IsEnded)
	{
		// 길찾기 종료 문구 출력
		char buffer1[25]{ "[ !- 길찾기 종료 -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 1, width + 2), buffer1, Color::Green);

		// 재시작 문구 출력
		char buffer2[50]{ "[ 재시작 >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 0, width + 2), buffer2, Color::White);
	}
}

void AStarLevel::SetPlayerPosition()
{
	// 좌클릭: 플레이어 위치 설정
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x >= 0 && mousePos.x < width &&
			mousePos.y >= 0 && mousePos.y < height)
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
		if (mousePos.x >= 0 && mousePos.x < width &&
			mousePos.y >= 0 && mousePos.y < height)
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
		if (mousePos.x >= 0 && mousePos.x < width &&
			mousePos.y >= 0 && mousePos.y < height)
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

bool AStarLevel::AStar()
{
	static bool aStarInitialize = false;

	Vector2 Direction[] =
	{
		// 상하좌우
		Vector2 {-1, 0},	// UP
		Vector2 {0, -1},	// LEFT
		Vector2 {1, 0},		// DOWN
		Vector2 {0, 1},		// RIGHT

		// 대각선
		Vector2 {-1, -1},	// UP_LEFT
		Vector2 {1, -1},	// DOWN_LEFT
		Vector2 {1, 1},		// DOWN_RIGHT
		Vector2 {-1, 1},	// UP, RIGHT
	};

	int Cost[] =
	{
		10,	// UP
		10, // LEFT
		10,	// DOWN
		10,	// RIGHT

		14, // UP_LEFT
		14,	// DOWN_LEFT
		14,	// DOWN_RIGH
		14,	// UP, RIGHT
	};

	// 시작 데이터 초기화
	if (!aStarInitialize)
	{
		aStarInitialize = true;

		// 방문 배열 초기화
		std::vector<std::vector<bool>> newClosedList(height, std::vector<bool>(width, false));
		closedList = newClosedList;

		// 현재 좌표의 가장 좋은 비용
		std::vector<std::vector<int>> newBest(height, std::vector<int>(width, INT32_MAX));
		best = newBest;

		// 시작 노드 초기화
		int g = 0;

		// 목표 위치까지의 거리
		int h = 10 * (abs(destinationPosition.y - playerPosition.y) + abs(destinationPosition.x - playerPosition.x));
		openList.push(Node{ g + h, g, playerPosition });
		best[playerPosition.y][playerPosition.x] = g + h;
		parent[playerPosition] = playerPosition;
	}

	// 현재 프레임에 방문할 노드
	size_t size = openList.size();

	// 제일 좋은 후보를 찾음
	Node node = openList.top();
	openList.pop();

	// #1: 현재 노드가 이미 방문한 노드인지 확인 { y, x }
	if (closedList[node.pos.y][node.pos.x]) return false;

	// #2: Best값을 비교해서 해당 좌표의 더 가까운 값이 있는지 확인
	if (best[node.pos.y][node.pos.x] < node.fCost) return false;

	// 방문 체크
	closedList[node.pos.y][node.pos.x] = true;

	// 목표 위치에 도달했는지 확인
	if (node.pos == destinationPosition)
	{
		return true;
	}

	// 방향 순회
	for (int i = 0; i < 8; ++i)
	{
		Vector2 nextPos = node.pos + Direction[i];

		// #1: 갈 수 있는 지역인지 확인 (장애물 체크)
		if (!CanGo(nextPos)) continue;

		// #2: 이미 방문한 노드인지 확인
		if (closedList[nextPos.y][nextPos.x]) continue;

		// #3: 비용 계산
		int gCost = node.gCost + Cost[i];
		int hCost = Cost[i] * (abs(destinationPosition.y - nextPos.y) + abs(destinationPosition.x - nextPos.x));

		// #4: 다른 경로에서 더 좋은 길을 찾았는지 확인
		if (best[nextPos.y][nextPos.x] <= gCost + hCost) continue;

		// OpenList에 방문할 노드 추가
		best[nextPos.y][nextPos.x] = gCost + hCost;
		openList.push(Node{ gCost + hCost, gCost, nextPos });
		parent[nextPos] = node.pos;

		// 길찾기에 계산된 경로에 추가 (시각화 용도)
		pathPosition.emplace_back(nextPos);
	}

	return false;
}

bool AStarLevel::AStarVisualization(float deltaTime)
{
	aStarTimer.Tick(deltaTime);

	if (!aStarTimer.IsTimeout())
	{
		return true;
	}

	// A-Star 수행
	if (AStar())
	{
		aStarState = AStarState::IsEnded;
		aStarTimer.Reset();
		return false;
	}

	// 타이머 초기화
	aStarTimer.Reset();

	// Todo:## 현재 길찾기 렌더 속도에 따라 타이머 설정해야 됨
	aStarTimer.SetTargetTime(0.1f);

	return false;
}

bool AStarLevel::CanGo(const Vector2 goPos)
{
	for (const auto& i : obstaclePosition)
	{
		if (i == goPos) return false;
	}

	return true;
}
