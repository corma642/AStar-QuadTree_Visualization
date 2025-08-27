#pragma once

#include "Engine.h"

class Level;

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	// A-Star 레벨 시작 함수
	void StartAStar(const int currnetRenderSpeed = 3);

	// QuadTree 레벨 시작 함수
	void StartQuadTree();

	static Game& Get();

private:
	static Game* instance;
};