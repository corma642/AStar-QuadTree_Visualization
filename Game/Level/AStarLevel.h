#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"

#include <iostream>

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const int currnetRenderSpeed = 3);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	// A-Star 길찾기 시작 함수
	void StartAStar();

	// 외곽 경계 출력 함수
	void PrintOutLine();

private:
	// 콘솔 창 크기
	int width = 0;
	int height = 0;

	// 현재 길찾기 렌더 속도
	int currnetRenderSpeed = 3;

	// A-Star 길찾기 시작할지 여부
	bool bIsStart = false;

	// A-Star 시각화가 모두 끝났는지 여부
	bool bIsEnded = false;
};
