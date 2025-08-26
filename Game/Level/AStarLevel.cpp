#include "AStarLevel.h"
#include "game/game.h"
#include "utils/utils.h"

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width() - tempWidth;
	height = Engine::Get().Height() - tempHeight;

	// A-Star Ÿ�̸� �⺻ ����
	aStarTimer.Reset();
	aStarTimer.SetTargetTime(0.2f);
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �÷��̾� & ��ǥ�� & ��ֹ� ��ġ ���� �Է� �ޱ�
	// A-Star ���� �Է� �ޱ�
	if (aStarState == AStarState::IsBefore)
	{
		IsTickBefore();
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
		aStarTimer.Tick(deltaTime);
		IsTickEnded();
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

	if (aStarState == AStarState::IsBefore)
	{
		IsRenderBefore();
		return;
	}

	// ��ã��� ���� ��ε� ���
	if (aStarState == AStarState::IsRenderComputepath ||
		aStarState == AStarState::IsFailed)
	{
		IsRenderPlaying();
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
		IsRenderFinalPath();
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

		// ��ġ�� ��ǥ ��ġ�� �浹�ϸ� ����
		if (mousePos == destinationPosition) return;

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// ��ġ�� ��ֹ��� �浹�ϸ� ����
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

		// ��ġ�� �÷��̾� ��ġ�� �浹�ϸ� ����
		if (mousePos == playerPosition) return;

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// ��ġ�� ��ֹ��� �浹�ϸ� ����
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

		// �ش� ��ġ�� �÷��̾ ��ǥ ��ġ�� ��� �н�
		if (mousePos == playerPosition || mousePos == destinationPosition) return false;

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x >= 0 && mousePos.x < width - 1 &&
			mousePos.y >= 0 && mousePos.y < height - 1)
		{
			// �̹� �ش� ��ġ�� ��ֹ��� �ִ��� �˻�
			bool findObstacle = false;

			for (const auto& i : obstaclePosition)
			{
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

	// ���� �Է�
	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Engine::Get().Quit();
	}
}

void AStarLevel::IsTickEnded()
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

void AStarLevel::IsRenderBefore()
{
	// ����Ű ���� ���
	char buffer1[50]{ "[ �÷��̾� ��ġ ���� ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ ��ǥ ��ġ ���� ]" };
	char buffer4[50]{ "> \"Right Click\"" };

	char buffer5[50]{ "[ ��ֹ� ��ġ ���� ]" };
	char buffer6[50]{ "> \"Space bar + Left Click\"" };

	char buffer7[50]{ "[ A* ���� ]" };
	char buffer8[50]{ "> \"Enter\"" };

	char buffer9[50]{ "[ ���� ]" };
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
	char buffer2[50]{ "   �� ã�� ��..." };
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
			// ��ã�� ������ ���, ��ǥ ��ġ�� ���� ������ �������� ��� �� ����
			if (aStarState == AStarState::IsFailed)
			{
				if (pathPosition[i] == bestNearingNode.pos)
				{
					renderComputePathIndex = (int)pathPosition.size();
					return;
				}
			}

			// ��ǥ ��ġ �߰� ��, ����
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

		// Ÿ�̸� �ʱ�ȭ
		aStarTimer.Reset();

		// Todo:## ���� ��ã�� ���� �ӵ��� ���� Ÿ�̸� �����ؾ� ��
		aStarTimer.SetTargetTime(0.07f);
	}
	// A* ��� ��θ� �� ����� ���
	else
	{
		// A*�� ����������, ���� ��θ� ����ϵ��� ���� ����
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

		// Ÿ�̸� �ʱ�ȭ
		aStarTimer.Reset();

		// Todo:## ���� ��ã�� ���� �ӵ��� ���� Ÿ�̸� �����ؾ� ��
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
	// ��ã�� ���� ���� ���
	if (aStarState == AStarState::IsFailed)
	{
		char buffer1[50]{ "[ ! *��ã�� ����* ! ]" };
		Engine::Get().WriteToBuffer(Vector2(width + 1, height - 8), buffer1, Color::LightRed);
	}
	else if (aStarState == AStarState::IsEnded)
	{
		char buffer1[50]{ "[ ! *��ã�� ����* ! ]" };
		Engine::Get().WriteToBuffer(Vector2(width + 1, height - 8), buffer1, Color::LightGreen);
	}

	// ����� ���� ���
	char buffer1[50]{ "[ ����� ]" };
	char buffer2[50]{ "> \"Enter\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 6), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 5), buffer2, Color::LightGreen);

	// ���� ���� ���
	char buffer3[50]{ "[ ���� ]" };
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

		bestNearingNode.gCost = std::numeric_limits<double>::infinity();
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
			// ��ã�� ��� ��� ���� �迭 �ߺ� ����
			pathPosition.erase(std::unique(pathPosition.begin(), pathPosition.end()), pathPosition.end());

			// �θ� �����ؼ� ���� ��θ� ����
			Vector2 pos = destinationPosition;

			while (true)
			{
				pos = parent[pos];

				// �θ� �ڽŰ� ������ ���� ��ġ�̹Ƿ�, ���� ����
				if (pos == parent[pos]) break;

				finalPath.emplace_back(pos);
			}

			// �迭�� ������, �� -> ���ۿ��� ���� -> ������ ����
			std::reverse(finalPath.begin(), finalPath.end());

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

			// ##3: ��� ���
			double nextPosGCost = node.gCost + (i < 4 ? orthCost : diagCost);

			// ##4: �� ���� ���� ã�Ҵ��� Ȯ��
			if (nextPosGCost >= best[nextPos.y][nextPos.x]) continue;

			// ##5: �� ���� ���̶��, ����
			best[nextPos.y][nextPos.x] = nextPosGCost;
			parent[nextPos] = node.pos;

			// ##6: OpenList�� ��� �߰�
			double fCost = nextPosGCost + Heuristic(nextPos, destinationPosition, orthCost, diagCost);
			openList.push(Node(fCost, nextPosGCost, nextPos));

			// ��ã�⿡ ���� ��ο� �߰� (�ð�ȭ �뵵)
			pathPosition.emplace_back(nextPos);

			// ��ǥ ��ġ�� ���� ������ ��� ����
			if (nextPosGCost < bestNearingNode.gCost)
			{
				bestNearingNode = node;
			}
		}
	}

	// ���� �Դٴ°Ŵ� ��θ� ��ã�Ҵٴ� �ǹ�
	pathPosition.erase(std::unique(pathPosition.begin(), pathPosition.end()), pathPosition.end());
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