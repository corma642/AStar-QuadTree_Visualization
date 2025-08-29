#pragma once

#include "Engine.h"

class Level;

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	// �˰��� ���� ���� ���� �Լ�
	void ChangeAlgorithmSelectLevel();

	// A-Star ���� ���� �Լ�
	void StartAStar(const int currnetRenderSpeed = 3);

	// QuadTree ���� ���� �Լ�
	void StartQuadTree();

	static Game& Get();

private:
	static Game* instance;
};