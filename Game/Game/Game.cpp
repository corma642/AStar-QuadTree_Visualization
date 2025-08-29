#include "Game.h"
#include "Level/AlgorithmSelectLevel.h"
#include "Level/AStarLevel.h"
#include "Level/QuadTree.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	AddLevel(new AlgorithmSelectLevel());
}

Game::~Game()
{
	Engine::CleanUp();
}

void Game::ChangeAlgorithmSelectLevel()
{
	ChangeLevel(new AlgorithmSelectLevel());
}

void Game::StartAStar(const int currnetRenderSpeed)
{
	ChangeLevel(new AStarLevel(currnetRenderSpeed));
}

void Game::StartQuadTree()
{
	ChangeLevel(new QuadTree());
}

Game& Game::Get()
{
	return *instance;
}