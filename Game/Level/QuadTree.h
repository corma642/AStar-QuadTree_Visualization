#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"
#include "Actor/TreeNode.h"

// ���� Ʈ���� ��ȯ�� ������Ʈ
struct SpawnObject
{
	SpawnObject(const Vector2 inPos) : character("O"), pos(inPos)
	{
	}

	const char* character;
	Vector2 pos;
};

class QuadTree : public Level
{
	RTTI_DECLARATIONS(QuadTree, Level)

public:
	QuadTree();
	virtual ~QuadTree();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	//// ������ ��ü�� ��ġ�� ��带 ��ȯ�ϴ� �Լ� (����-Query)
	//std::vector<TreeNode*> Query(TreeNode* queryNode);

private:
	// �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� (�ؽ�Ʈ ��¿�)
	int tempWidth = 20;

	// ���� �Է� �޴� �Լ�
	void IsTickInput();

	// ����Ű / �޴� ��� UI ���� �Լ�
	void IsRenderUI();

	// ��ȯ�� ������Ʈ ��ġ �迭
	std::vector<SpawnObject> spawnObjects;

	// ��Ʈ ���
	TreeNode* root = nullptr;
};