#include "Game.h"
#include "Level/AStarLevel.h"
#include "Level/QuadTree.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	//AddLevel(new AStarLevel(5));
	AddLevel(new QuadTree);
}

Game::~Game()
{
	Engine::CleanUp();
}

void Game::StartAStar(const int currnetRenderSpeed)
{
	Level* newAStarLevel = new AStarLevel(currnetRenderSpeed);
	ChangeLevel(newAStarLevel);
}

void Game::StartQuadTree()
{
	Level* newQuadTreeLevel = new QuadTree();
	ChangeLevel(newQuadTreeLevel);
}

Game& Game::Get()
{
	return *instance;
}