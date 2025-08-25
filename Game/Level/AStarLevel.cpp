#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width();
	height = Engine::Get().Height();

	obstaclePosition.clear();

	// Ÿ�̸� ���� (�ð�ȭ�Ǵ� �ð� ����)
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

	// �÷��̾� & ��ǥ�� & ��ֹ� ��ġ ����
	if (aStarState == AStarState::IsBefore)
	{
		// ��ֹ��� ������ �����ӿ��� �÷��̾� �̵� X
		if (!SetObstaclePosition())
		{
			SetPlayerPosition();
		}
		SetDestinationPosition();
	}

	// ��ã�� ���� �غ� �� ���
	if (aStarState == AStarState::IsBefore)
	{
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			// ��ã�� ���� Ʈ���� Ȱ��ȭ
			aStarState = AStarState::IsReady;
			return;
		}
	}

	// ��ã�� ���� ��, A-Star ��ã�� ����
	if (aStarState == AStarState::IsReady)
	{
		aStarState = AStarState::IsPlaying;
		return;
	}

	// �ð�ȭ�� ��� ���� ���
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

	// �ܰ� ��� ���
	PrintOutLine();

	// �÷��̾� & ��ǥ ��ġ ���
	if (hasPlayer)
	{
		Engine::Get().WriteToBuffer(playerPosition, "P", Color::Green);
	}
	if (hasTarget)
	{
		Engine::Get().WriteToBuffer(destinationPosition, "D", Color::Red);
	}

	// ��ֹ� ���
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
		// ��ã�� ���� ���� ���
		char buffer1[25]{ "[ !- ��ã�� ���� -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 1, width + 2), buffer1, Color::Green);

		// ����� ���� ���
		char buffer2[50]{ "[ ����� >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 0, width + 2), buffer2, Color::White);
	}
}

void AStarLevel::SetPlayerPosition()
{
	// ��Ŭ��: �÷��̾� ��ġ ����
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
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
	// ��Ŭ��: ��ǥ ���� ����
	if (Input::Get().GetMouseRightButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
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
	// ��Ŭ�� + �����̽� ��: ��ֹ� ��ġ ����
	if (Input::Get().GetMouseLeftButton() && Input::Get().GetKey(VK_SPACE))
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x >= 0 && mousePos.x < width &&
			mousePos.y >= 0 && mousePos.y < height)
		{
			// �̹� �ش� ��ġ�� ��ֹ��� �ִ��� �˻�
			bool findObstacle = false;

			for (const auto& i : obstaclePosition)
			{
				// �ش� ��ġ�� �÷��̾ ��ǥ ��ġ�� ��� �н�
				if (i == playerPosition || i == destinationPosition) continue;

				// �̹� �ش� ��ġ�� ��ֹ��� ������ ���� ����
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
		// �����¿�
		Vector2 {-1, 0},	// UP
		Vector2 {0, -1},	// LEFT
		Vector2 {1, 0},		// DOWN
		Vector2 {0, 1},		// RIGHT

		// �밢��
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

	// ���� ������ �ʱ�ȭ
	if (!aStarInitialize)
	{
		aStarInitialize = true;

		// �湮 �迭 �ʱ�ȭ
		std::vector<std::vector<bool>> newClosedList(height, std::vector<bool>(width, false));
		closedList = newClosedList;

		// ���� ��ǥ�� ���� ���� ���
		std::vector<std::vector<int>> newBest(height, std::vector<int>(width, INT32_MAX));
		best = newBest;

		// ���� ��� �ʱ�ȭ
		int g = 0;

		// ��ǥ ��ġ������ �Ÿ�
		int h = 10 * (abs(destinationPosition.y - playerPosition.y) + abs(destinationPosition.x - playerPosition.x));
		openList.push(Node{ g + h, g, playerPosition });
		best[playerPosition.y][playerPosition.x] = g + h;
		parent[playerPosition] = playerPosition;
	}

	// ���� �����ӿ� �湮�� ���
	size_t size = openList.size();

	// ���� ���� �ĺ��� ã��
	Node node = openList.top();
	openList.pop();

	// #1: ���� ��尡 �̹� �湮�� ������� Ȯ�� { y, x }
	if (closedList[node.pos.y][node.pos.x]) return false;

	// #2: Best���� ���ؼ� �ش� ��ǥ�� �� ����� ���� �ִ��� Ȯ��
	if (best[node.pos.y][node.pos.x] < node.fCost) return false;

	// �湮 üũ
	closedList[node.pos.y][node.pos.x] = true;

	// ��ǥ ��ġ�� �����ߴ��� Ȯ��
	if (node.pos == destinationPosition)
	{
		return true;
	}

	// ���� ��ȸ
	for (int i = 0; i < 8; ++i)
	{
		Vector2 nextPos = node.pos + Direction[i];

		// #1: �� �� �ִ� �������� Ȯ�� (��ֹ� üũ)
		if (!CanGo(nextPos)) continue;

		// #2: �̹� �湮�� ������� Ȯ��
		if (closedList[nextPos.y][nextPos.x]) continue;

		// #3: ��� ���
		int gCost = node.gCost + Cost[i];
		int hCost = Cost[i] * (abs(destinationPosition.y - nextPos.y) + abs(destinationPosition.x - nextPos.x));

		// #4: �ٸ� ��ο��� �� ���� ���� ã�Ҵ��� Ȯ��
		if (best[nextPos.y][nextPos.x] <= gCost + hCost) continue;

		// OpenList�� �湮�� ��� �߰�
		best[nextPos.y][nextPos.x] = gCost + hCost;
		openList.push(Node{ gCost + hCost, gCost, nextPos });
		parent[nextPos] = node.pos;

		// ��ã�⿡ ���� ��ο� �߰� (�ð�ȭ �뵵)
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

	// A-Star ����
	if (AStar())
	{
		aStarState = AStarState::IsEnded;
		aStarTimer.Reset();
		return false;
	}

	// Ÿ�̸� �ʱ�ȭ
	aStarTimer.Reset();

	// Todo:## ���� ��ã�� ���� �ӵ��� ���� Ÿ�̸� �����ؾ� ��
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
