#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

#include <iostream>
#include <queue>
#include <map>

struct Node
{
	// 시작 위치에서 목표 위치까지의 비용 + 목표 위지까지의 거리
	double fCost; // f = g + h

	// 시작 위치에서 목표 위치까지의 비용
	double gCost;

	// 노드 위치
	Vector2 pos;
};

// fCost가 작은 것이 top으로 오도록 비교 함수
struct CompareNode
{
	bool operator()(const Node& a, const Node& b) const
	{
		return a.fCost > b.fCost;
	}
};

// 휴리스틱 (유클리드 기법 사용 / 대각선 고려)
inline double Heuristic(const Vector2& a, const Vector2& b, double orthCost, double diagCost)
{
	// 정수 좌표 차이
	double dx = static_cast<double>(a.x - b.x);
	double dy = static_cast<double>(a.y - b.y);

	// 유클리드 거리
	double dist = std::sqrt(dx * dx + dy * dy);

	// 단일축 이동 비용 vs 대각선 비용을 길이로 나눈 값
	double cost = min(orthCost, diagCost / std::sqrt(2.0));

	return dist * cost;
}

// A-Star 시각화 상태
enum class AStarState
{
	IsBefore,		// 시작 준비 전
	IsPlaying,		// 진행중
	IsRenderComputepath,	// 계산한 좌표 렌더중
	IsRenderFinalPath,		// 최종 경로 렌더중
	IsEnded,		// 끝남. 다음 입력 대기중
	IsFailed,		// 경로를 못찾음
};

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const int currnetRenderSpeed = 3);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	// 외곽 경계 출력 함수
	void PrintOutLine();

	// 플레이어 & 목표점 & 장애물 위치 설정 함수
	void SetPlayerPosition();
	void SetDestinationPosition();
	bool SetObstaclePosition();

	// 플레이어 & 목표점 & 장애물 출력 함수
	void PrintPlayer();
	void PrintDestination();
	void PrintObstacle();

	// 상태별 Tick 수행 함수
	void IsBefore();
	void IsEnded();

	// 상태별 Render 수행 함수
	void IsRenderComputepath();
	void IsRenderFinalPath();
	void IsRenderEnded();

	// A-Star 길찾기 함수
	void AStar();

	// 해당 좌표로 갈 수 있는지 확인하는 함수
	bool CanGo(const Vector2& from, const Vector2& next);

private:
	// 콘솔 창 크기
	int width = 0;
	int height = 0;

	// 현재 길찾기 렌더 속도
	int currnetRenderSpeed = 3;

	// 플레이어와 목표의 위치가 설정되었는지 여부
	bool hasPlayer = false;
	bool hasTarget = false;

	// 플레이어와 목표 위치
	Vector2 playerPosition = Vector2(-1, -1);
	Vector2 destinationPosition = Vector2(-1, -1);

	// 장애물 위치
	std::vector<Vector2> obstaclePosition;

	// A-Star 시각화 상태
	AStarState aStarState = AStarState::IsBefore;

	// A-Star 시각화 타이머
	Timer aStarTimer;

	// 방문할 좌표 (f, 휴리스틱 추정값이 작은 순서로 우선순위 큐 사용)
	std::priority_queue<Node, std::vector<Node>, CompareNode> openList;

	// 방문한 좌표
	std::vector<std::vector<bool>> closedList;

	// 각 좌표의 가장 좋은 비용
	std::vector<std::vector<double>> best;

	// 최종 경로 추적 용도
	std::map<Vector2, Vector2> parent;

	// 길찾기 계산 경로 저장 배열
	std::vector<Vector2> pathPosition;

	// 최종 경로 저장 배열
	std::vector<Vector2> finalPath;
};
