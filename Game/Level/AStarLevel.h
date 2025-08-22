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
	// A-Star ��ã�� ���� �Լ�
	void StartAStar();

	// �ܰ� ��� ��� �Լ�
	void PrintOutLine();

private:
	// �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� ��ã�� ���� �ӵ�
	int currnetRenderSpeed = 3;

	// A-Star ��ã�� �������� ����
	bool bIsStart = false;

	// A-Star �ð�ȭ�� ��� �������� ����
	bool bIsEnded = false;
};
