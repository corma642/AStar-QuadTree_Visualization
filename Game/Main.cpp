#include <iostream>
#include "Game/Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// std::cout 비동기 해제
	// c stdio와의 동기화를 끊어, C++의 << 호출 비용을 절약
	std::ios::sync_with_stdio(false);
	std::cout.tie(nullptr);

	Game game;
	game.Run();
}