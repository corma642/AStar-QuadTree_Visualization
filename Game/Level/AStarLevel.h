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
//	// �� ���� �б� �Լ�
//	void ReadMapFile();
//
//	// �� ������ ������� �̷� �����ϴ� �Լ�
//	void CreateMaze();
//
//	// A-Star ��ã�� ���� �Լ�
//	void StartAStar();
//
//private:
//	// ���� ��ã�� ���� �ӵ�
//	int currnetRenderSpeed = 3;
//
//	// A-Star ��ã�� �������� ����
//	bool bIsStart = false;
//
//	// �������� �����ߴ��� �˻��ϴ� ����
//	bool bIsArrived = false;
//
//	// �̷� ������
//	Vector2
//	std::vector<std::vector<char>> maze(30, 50);
//};
