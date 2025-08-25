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
	// �÷��̾�/��ǥ�� ��ġ ���� �Լ�
	void SetPlayerPosition();
	void SetDestinationPosition();

	// �ܰ� ��� ��� �Լ�
	void PrintOutLine();

	// A-Star ��ã�� ���� �Լ�
	void StartAStar();

private:
	// �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� ��ã�� ���� �ӵ�
	int currnetRenderSpeed = 3;

	// �÷��̾�� ��ǥ ��ġ
	Vector2 playerPosition = Vector2(-1, -1);
	Vector2 destinationPosition = Vector2(-1, -1);

	// �÷��̾�� ��ǥ�� �����Ǿ����� ����
	bool hasPlayer = false;
	bool hasTarget = false;

	// A-Star ��ã�� �������� ����
	bool bIsStart = false;

	// A-Star �ð�ȭ�� ��� �������� ����
	bool bIsEnded = false;
};
