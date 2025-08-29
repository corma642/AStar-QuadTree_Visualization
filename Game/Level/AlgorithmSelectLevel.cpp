#include "AlgorithmSelectLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

AlgorithmSelectLevel::AlgorithmSelectLevel()
{
	// 메뉴 아이템 추가
	items.push_back(new MenuItem(
		">> A* Algorithm",
		[]() { Game::Get().StartAStar(); }
	));

	// 메뉴 아이템 추가
	items.push_back(new MenuItem(
		">> QuadTree Algorithm",
		[]() { Game::Get().StartQuadTree(); }
	));

	// 메뉴 아이템 추가
	items.push_back(new MenuItem(
		">> QUIT",
		[]() { Game::Get().Quit(); }
	));

	// 아이템 수 미리 저장
	length = static_cast<int>(items.size());
}

AlgorithmSelectLevel::~AlgorithmSelectLevel()
{
	for (MenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void AlgorithmSelectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리
	if (Input::Get().GetKeyDown(VK_UP) || Input::Get().GetKeyDown('W'))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN) || Input::Get().GetKeyDown('S'))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	// Enter Key 입력
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장하고 있는 함수 호출
		items[currentIndex]->onSelected();
	}
}

void AlgorithmSelectLevel::Render()
{
	super::Render();

	PrintBackGround();

	// 메뉴 아이템 렌더링
	for (int i = 0, yPos = 4; i < length; ++i, yPos += 6)
	{
		// 아이템 색상 확인
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// 메뉴 텍스트 출력
		Engine::Get().WriteToBuffer(Vector2(2, yPos), items[i]->menuText, textColor);
	}
}

void AlgorithmSelectLevel::PrintBackGround()
{
	const char* buffer1{ "[ * AStar & QuadTree Visualization * ]" };

	Engine::Get().WriteToBuffer(Vector2(0, 0), buffer1, Color::LightSkyBlue);


	char buffer2[10][50]{
		{ "+----------------------------------+" },
		{ "|                                  |" },
		{ "|                                  |" },
		{ "|                                  |" },
		{ "+----------------------------------+" }
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, 2 + i), buffer2[i], Color::LightBlue);
	}


	char buffer3[10][50]{
		{ "+----------------------------------+" },
		{ "|                                  |" },
		{ "|                                  |" },
		{ "|                                  |" },
		{ "+----------------------------------+" }
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, 8 + i), buffer3[i], Color::LightBlue);
	}


	char buffer4[10][50]{
	{ "+----------------------------------+" },
	{ "|                                  |" },
	{ "|                                  |" },
	{ "|                                  |" },
	{ "+----------------------------------+" }
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, 14 + i), buffer4[i], Color::LightBlue);
	}
}