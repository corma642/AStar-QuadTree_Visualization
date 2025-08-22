//#pragma once
//
//#include "Level/Level.h"
//#include "Utils/Timer.h"
//
//#include <vector>
//
//class AStarLevel : public Level
//{
//	RTTI_DECLARATIONS(AStarLevel, Level)
//
//public:
//	AStarLevel(const int currnetRenderSpeed = 3);
//
//	virtual void Tick(float deltaTime) override;
//	virtual void Render() override;
//
//private:
//	// 멥 파일 읽기 함수
//	void ReadMapFile();
//
//	// 맵 파일을 기반으로 미로 생성하는 함수
//	void CreateMaze();
//
//	// A-Star 길찾기 시작 함수
//	void StartAStar();
//
//private:
//	// 현재 길찾기 렌더 속도
//	int currnetRenderSpeed = 3;
//
//	// A-Star 길찾기 시작할지 여부
//	bool bIsStart = false;
//
//	// 목적지에 도착했는지 검사하는 변수
//	bool bIsArrived = false;
//
//	// 미로 데이터
//	Vector2
//	std::vector<std::vector<char>> maze(30, 50);
//};
