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

	// 총 비용
	// 시작 위치에서 목표 위치까지의 비용 + 목표 위지까지의 거리 (직선 계산)
	int fCost; // f = g + h

	// 시작 위치에서 목표 위치까지의 비용
	int gCost;

	// 노드 위치
	Vector2 pos;
};

// A-Star 시각화 상태
enum class AStarState
{
	IsBefore,	// 시작 준비 전
	IsReady,	// 시작 대기중
	IsPlaying,	// 진행중
	IsEnded,	// 끝남
};

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const int currnetRenderSpeed = 3);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	// 플레이어 & 목표점 & 장애물 위치 설정 함수
	void SetPlayerPosition();
	void SetDestinationPosition();
	bool SetObstaclePosition();

	// 외곽 경계 출력 함수
	void PrintOutLine();

	// A-Star 길찾기 함수
	bool AStar();

	// A-Star 길찾기 시각화 함수
	bool AStarVisualization(float deltaTime);

	// 해당 좌표로 갈 수 있는지 확인하는 함수
	bool CanGo(const Vector2 goPos);

private:
	// 콘솔 창 크기
	int width = 0;
	int height = 0;

	// 현재 길찾기 렌더 속도
	int currnetRenderSpeed = 3;

	// 플레이어와 목표 위치
	Vector2 playerPosition = Vector2(-1, -1);
	Vector2 destinationPosition = Vector2(-1, -1);

	// 장애물 위치
	std::vector<Vector2> obstaclePosition;

	// 플레이어와 목표가 설정되었는지 여부
	bool hasPlayer = false;
	bool hasTarget = false;

	// A-Star 시각화 상태
	AStarState aStarState = AStarState::IsBefore;

	// 길찾기 계산 경로 위치
	std::vector<Vector2> pathPosition;

	// 최적의 경로 위치
	std::vector<Vector2> finalPath;

	// A-Star 시각화 타이머
	Timer aStarTimer;

	// 방문할 좌표 (g 즉, 비용이 적은 순서로 우선순위 큐 사용)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
	
	// 방문한 좌표
	std::vector<std::vector<bool>> closedList;

	// 각 좌표의 가장 좋은 비용
	std::vector<std::vector<int>> best;

	// 부모 추적 용도
	std::map<Vector2, Vector2> parent;
};
