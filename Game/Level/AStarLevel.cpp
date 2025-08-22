#include "AStarLevel.h"
#include "math/vector2.h"
#include "game/game.h"
#include "utils/utils.h"
#include "level/level.h"

#include <iostream>

AStarLevel::AStarLevel(const int currnetRenderSpeed)
{
	width = Engine::Get().Width();
	height = Engine::Get().Height();
}

void AStarLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (bIsEnded)
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

	Vector2 mousePos = Vector2::Zero;


	

	if (bIsEnded)
	{
		// ��ã�� ���� ���� ���
		char buffer1[25]{ "[ !- ��ã�� ���� -! ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 1, width + 2), buffer1, Color::Green);

		// ����� ���� ���
		char buffer2[50]{ "[ ����� >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(height - 0, width + 2), buffer2, Color::White);
	}
}

void AStarLevel::StartAStar()
{
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
