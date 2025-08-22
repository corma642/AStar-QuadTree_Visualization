#include "Game.h"
#include "Level/AStarLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	AddLevel(new AStarLevel());
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

Game& Game::Get()
{
	return *instance;
}