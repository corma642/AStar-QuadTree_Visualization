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

	// 플레이어와 목표점 위치 설정
	if (!bIsStart)
	{
		SetPlayerPosition();
		SetDestinationPosition();
	}




	// 시각화가 모두 끝난 경우
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

	// 외곽 경계 출력
	PrintOutLine();

	if (hasPlayer)
	{
		Engine::Get().WriteToBuffer(playerPosition, "P", Color::Green);
	}
	if (hasTarget)
	{
		Engine::Get().WriteToBuffer(destinationPosition, "D", Color::Red);
	}

	if (bIsEnded)
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
			destinationPosition = mousePos;
			hasTarget = true;
		}
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
