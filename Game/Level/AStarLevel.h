#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

#include <iostream>
#include <queue>
#include <map>

struct Node
{
	Node(double inFCost = 0.0, double inGCost = 0.0, Vector2 inPos = Vector2::Out)
		: fCost(inFCost), gCost(inGCost), pos(inPos)
	{
	}

	// ���� ��ġ���� ��ǥ ��ġ������ ��� + ��ǥ ���������� �Ÿ�
	double fCost; // f = g + h

	// ���� ��ġ���� ��ǥ ��ġ������ ���
	double gCost;

	// ��� ��ġ
	Vector2 pos;
};

// fCost�� ���� ���� top���� ������ �� �Լ�
struct CompareNode
{
	bool operator()(const Node& a, const Node& b) const
	{
		return a.fCost > b.fCost;
	}
};

// �޸���ƽ (��Ŭ���� ��� ��� / �밢�� ���)
inline double Heuristic(const Vector2& a, const Vector2& b, double orthCost, double diagCost)
{
	// ���� ��ǥ ����
	double dx = static_cast<double>(a.x - b.x);
	double dy = static_cast<double>(a.y - b.y);

	// ��Ŭ���� �Ÿ�
	double dist = std::sqrt(dx * dx + dy * dy);

	// ������ �̵� ��� vs �밢�� ����� ���̷� ���� ��
	double cost = min(orthCost, diagCost / std::sqrt(2.0));

	return dist * cost;
}

// A-Star �ð�ȭ ����
enum class AStarState
{
	IsBefore,		// ���� �غ� ��
	IsPlaying,		// ������
	IsRenderComputepath,	// ����� ��� ������
	IsRenderFinalPath,		// ���� ��� ������
	IsEnded,		// ����. ���� �Է� �����
	IsFailed,		// ��θ� ��ã��
};

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const int currnetRenderSpeed = 3);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	// �ܰ� ��� ��� �Լ�
	void PrintOutLine();

	// �÷��̾� & ��ǥ�� & ��ֹ� ��ġ ���� �Լ�
	void SetPlayerPosition();
	void SetDestinationPosition();
	bool SetObstaclePosition();

	// �÷��̾� & ��ǥ�� & ��ֹ� ��� �Լ�
	void PrintPlayer();
	void PrintDestination();
	void PrintObstacle();

	// ���º� Tick ���� �Լ�
	void IsTickBefore();
	void IsTickEnded();

	// ���º� Render ���� �Լ�
	void IsRenderBefore();
	void IsRenderPlaying();
	void IsRenderComputepath();
	void IsRenderFinalPath();
	void IsRenderEnded();

	// ��� ���� �ӵ� ���� �Լ�
	void SetRenderSpeed();
	void PrintRenderSpeed();

	// A-Star ��ã�� �Լ�
	void AStar();

	// �ش� ��ǥ�� �� �� �ִ��� Ȯ���ϴ� �Լ�
	bool CanGo(const Vector2& from, const Vector2& next);

private:
	// �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� (�ؽ�Ʈ ��¿�)
	int tempWidth = 30;

	// ���� ��ã�� ���� �ӵ�
	int currnetRenderSpeed;

	// �÷��̾�� ��ǥ�� ��ġ�� �����Ǿ����� ����
	bool hasPlayer = false;
	bool hasTarget = false;

	// �÷��̾�� ��ǥ ��ġ
	Vector2 playerPosition = Vector2::Out;
	Vector2 destinationPosition = Vector2::Out;

	// ��ֹ� ��ġ
	std::vector<Vector2> obstaclePosition;

	// A-Star �ð�ȭ ����
	AStarState aStarState = AStarState::IsBefore;

	// A-Star �ð�ȭ Ÿ�̸�
	Timer aStarTimer;

	// �湮�� ��ǥ (f, �޸���ƽ �������� ���� ������ �켱���� ť ���)
	std::priority_queue<Node, std::vector<Node>, CompareNode> openList;

	// �湮�� ��ǥ
	std::vector<std::vector<bool>> closedList;

	// �� ��ǥ�� ���� ���� ���
	std::vector<std::vector<double>> best;

	// ���� ��� ���� �뵵
	std::map<Vector2, Vector2> parent;

	// ��ã�� ��� ��� ���� �迭
	std::vector<Vector2> pathPosition;

	// ���� ��� ���� �迭
	std::vector<Vector2> finalPath;

	// ��� ��� �ε���
	int renderComputePathIndex = 0;
	int renderFinalPathIndex = 0;
};
