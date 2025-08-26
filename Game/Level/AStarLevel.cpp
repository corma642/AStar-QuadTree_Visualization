#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width();
	height = Engine::Get().Height();

	obstaclePosition.clear();

	// A-Star Ÿ�̸� �⺻ ����
	aStarTimer.SetTargetTime(0.2f);
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �÷��̾� & ��ǥ�� & ��ֹ� ��ġ ���� �Է� �ޱ�
	// A-Star ���� �Է� �ޱ�
	if (aStarState == AStarState::IsBefore)
	{
		IsBefore();
		return;
	}

	// A-Star ����
	if (aStarState == AStarState::IsPlaying)
	{
		AStar();
		return;
	}

	// ��ã�� ��� ��� ���� ƽ
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		aStarTimer.Tick(deltaTime);
		return;
	}

	// ���� ��� ���� ƽ
	if (aStarState == AStarState::IsRenderFinalPath)
	{
		aStarTimer.Tick(deltaTime);
		return;
	}

	// ���� ���� �޴� �Է� �ޱ�
	if (aStarState == AStarState::IsEnded)
	{
		IsEnded();
		return;
	}
}

void AStarLevel::Render()
{
	super::Render();

	// �ܰ� ��� ���
	PrintOutLine();

	// �÷��̾� & ��ǥ�� & ��ֹ� ���
	PrintPlayer();
	PrintDestination();
	PrintObstacle();

	// ��ã��� ���� ��ε� ���
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderComputepath();
		return;
	}

	// ���� ��� ���
	if (aStarState == AStarState::IsRenderFinalPath)
	{
		IsRenderFinalPath();
		return;
	}

	// ���� ���� �޴� ���
	if (aStarState == AStarState::IsEnded ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderEnded();
		return;
	}
}

void AStarLevel::SetPlayerPosition()
{
	// ��Ŭ��: �÷��̾� ��ġ ����
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
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
	// ��Ŭ��: ��ǥ ���� ����
	if (Input::Get().GetMouseRightButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
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
	// ��Ŭ�� + �����̽� ��: ��ֹ� ��ġ ����
	if (Input::Get().GetMouseLeftButton() && Input::Get().GetKey(VK_SPACE))
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
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
	// ��ֹ��� ������ �����ӿ��� �÷��̾� �̵� X
	if (!SetObstaclePosition())
	{
		SetPlayerPosition();
	}
	SetDestinationPosition();

	// ��ã�� ���� �غ� �� ���
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// ��ã�� ���������� ����
		aStarState = AStarState::IsPlaying;
		return;
	}
}

void AStarLevel::IsEnded()
{
	// ����� �Է�
	if (Input::Get().GetKey(VK_RETURN))
	{
		Game::Get().StartAStar(currnetRenderSpeed);
	}

	// ���� �Է�
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

		// Ÿ�̸� �ʱ�ȭ
		aStarTimer.Reset();

		// Todo:## ���� ��ã�� ���� �ӵ��� ���� Ÿ�̸� �����ؾ� ��
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

		// Ÿ�̸� �ʱ�ȭ
		aStarTimer.Reset();

		// Todo:## ���� ��ã�� ���� �ӵ��� ���� Ÿ�̸� �����ؾ� ��
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
	// ��ã�� ���� ���� ���
	if (aStarState == AStarState::IsFailed)
	{
		char buffer1[25]{ "[ !- ��ã�� ���� -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 2, width + 2), buffer1, Color::Red);
	}
	else if (aStarState == AStarState::IsEnded)
	{
		char buffer1[25]{ "[ !- ��ã�� ���� -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 2, width + 2), buffer1, Color::Green);
	}

	// ����� ���� ���
	char buffer2[50]{ "[ ����� >> \"Enter\" ]" };
	Engine::Get().WriteToBuffer(Vector2(height - 1, width + 2), buffer2, Color::White);

	// ���� ���� ���
	char buffer3[50]{ "[ ���� >> \"ESC\" ]" };
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
	// ���� ���� �� ���
	Vector2 dirs[] =
	{
		// �����¿�
		{-1, 0}, {0, -1}, {1, 0}, {0, 1},

		// �밢�� (�»�, ����, ���, ����)
		{-1, -1}, {1, -1}, {1, 1}, {-1, 1}
	};

	// �̵� ���
	const double orthCost = 10.0; // ���� �̵� ���
	const double diagCost = orthCost * std::sqrt(2.0); // �밢�� �̵� ���

	// ���� ������ �ʱ�ȭ
	{
		// �湮 �迭 �ʱ�ȭ
		closedList.assign(height, std::vector<bool>(width, false));

		// ����Ʈ �迭 �ʱ�ȭ (��� �ִ밪���� �ʱ�ȭ)
		best.assign(height, std::vector<double>(width, std::numeric_limits<double>::infinity()));


		// ���� ������ �ʱ�ȭ
		best[playerPosition.y][playerPosition.x] = 0.0;
		parent[playerPosition] = playerPosition;

		double h = Heuristic(playerPosition, destinationPosition, orthCost, diagCost);
		openList.push(Node{ h, 0.0, playerPosition });
	}

	while (!openList.empty())
	{
		// #1: ���� ���� �ĺ��� ã��
		Node node = openList.top();
		openList.pop();

		// #2: Best���� ���ؼ� �ش� ��ǥ�� �������� ����� �� �� ��尡 �ִ��� Ȯ��
		if (node.gCost > best[node.pos.y][node.pos.x]) continue;

		// #3: ��ǥ ��ġ�� �����ߴ��� Ȯ��
		if (node.pos == destinationPosition)
		{
			// �θ� �����ؼ� ���� ��θ� ����
			Vector2 pos = destinationPosition;
			pathPosition.emplace_back(pos);

			while (true)
			{
				// �θ� �ڽŰ� ������ ���� ��ġ�̹Ƿ�, ���� ����
				if (pos == parent[pos]) break;

				pos = parent[pos];
				pathPosition.emplace_back(pos);
			}

			// �迭�� ������, �� -> ���ۿ��� ���� -> ������ ����
			std::reverse(pathPosition.begin(), pathPosition.end());

			// ����� ��ǥ�� �����ϵ��� ���� ����
			aStarState = AStarState::IsRenderComputepath;
			return;
		}

		// #4: �湮 üũ
		closedList[node.pos.y][node.pos.x] = true;

		// #5: ���� ��ȸ
		for (int i = 0; i < 8; ++i)
		{
			Vector2 nextPos = node.pos + dirs[i];

			// ##1: �� �� �ִ� �������� Ȯ�� (����, ��ֹ� üũ)
			if (!CanGo(node.pos, nextPos)) continue;

			// ##2: �̹� �湮�� ������� Ȯ��
			if (closedList[nextPos.y][nextPos.x]) continue;

			// ��ã�⿡ ���� ��ο� �߰� (�ð�ȭ �뵵)
			pathPosition.emplace_back(nextPos);

			// ##3: ��� ���
			double nextPosGCost = node.gCost + (i < 4 ? orthCost : diagCost);

			// ##4: �� ���� ���� ã�Ҵ��� Ȯ��
			if (nextPosGCost >= best[nextPos.y][nextPos.x]) continue;

			// ##5: �� ���� ���̶��, ����
			best[nextPos.y][nextPos.x] = nextPosGCost;
			parent[nextPos] = node.pos;

			// ##6: OpenList�� ��� �߰�
			double fCost = nextPosGCost + Heuristic(nextPos, destinationPosition, orthCost, diagCost);
			openList.push(Node{ fCost, nextPosGCost, nextPos });
		}
	}

	// ���� �Դٴ°Ŵ� ��θ� ��ã�Ҵٴ� �ǹ�
	aStarState = AStarState::IsFailed;
}

bool AStarLevel::CanGo(const Vector2& from, const Vector2& next)
{
	// ��ȿ ���� üũ
	if (next.x < 0 || next.x >= width || next.y < 0 || next.y >= height)
	{
		return false;
	}

	// ������ ��ġ�� ��ֹ��� �ִ��� Ȯ��
	// �밢���̶��, ������ ����/���ο� ��ֹ��� �ִ��� �߰� Ȯ��

	int dx = next.x - from.x;
	int dy = next.y - from.y;

	Vector2 horizontal(from.x + dx, from.y); // ����
	Vector2 vertical(from.x, from.y + dy); // ����

	for (const auto& i : obstaclePosition)
	{
		if (i == next || i == horizontal || i == vertical) return false;
	}

	return true;
}
