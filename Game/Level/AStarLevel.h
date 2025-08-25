#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

#include <iostream>
#include <queue>
#include <map>

struct Node
{
	bool operator<(const Node& other) const { return fCost < other.fCost; }
	bool operator>(const Node& other) const { return fCost > other.fCost; }

	// �� ���
	// ���� ��ġ���� ��ǥ ��ġ������ ��� + ��ǥ ���������� �Ÿ� (���� ���)
	int fCost; // f = g + h

	// ���� ��ġ���� ��ǥ ��ġ������ ���
	int gCost;

	// ��� ��ġ
	Vector2 pos;
};

// A-Star �ð�ȭ ����
enum class AStarState
{
	IsBefore,	// ���� �غ� ��
	IsReady,	// ���� �����
	IsPlaying,	// ������
	IsEnded,	// ����
};

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const int currnetRenderSpeed = 3);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	// �÷��̾� & ��ǥ�� & ��ֹ� ��ġ ���� �Լ�
	void SetPlayerPosition();
	void SetDestinationPosition();
	bool SetObstaclePosition();

	// �ܰ� ��� ��� �Լ�
	void PrintOutLine();

	// A-Star ��ã�� �Լ�
	bool AStar();

	// A-Star ��ã�� �ð�ȭ �Լ�
	bool AStarVisualization(float deltaTime);

	// �ش� ��ǥ�� �� �� �ִ��� Ȯ���ϴ� �Լ�
	bool CanGo(const Vector2 goPos);

private:
	// �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� ��ã�� ���� �ӵ�
	int currnetRenderSpeed = 3;

	// �÷��̾�� ��ǥ ��ġ
	Vector2 playerPosition = Vector2(-1, -1);
	Vector2 destinationPosition = Vector2(-1, -1);

	// ��ֹ� ��ġ
	std::vector<Vector2> obstaclePosition;

	// �÷��̾�� ��ǥ�� �����Ǿ����� ����
	bool hasPlayer = false;
	bool hasTarget = false;

	// A-Star �ð�ȭ ����
	AStarState aStarState = AStarState::IsBefore;

	// ��ã�� ��� ��� ��ġ
	std::vector<Vector2> pathPosition;

	// ������ ��� ��ġ
	std::vector<Vector2> finalPath;

	// A-Star �ð�ȭ Ÿ�̸�
	Timer aStarTimer;

	// �湮�� ��ǥ (g ��, ����� ���� ������ �켱���� ť ���)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	
	// �湮�� ��ǥ
	std::vector<std::vector<bool>> closedList;

	// �� ��ǥ�� ���� ���� ���
	std::vector<std::vector<int>> best;

	// �θ� ���� �뵵
	std::map<Vector2, Vector2> parent;
};
