#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"
#include "Actor/TreeNode.h"

#include <deque>

struct SpawnObject
{
	SpawnObject(const Vector2 inPos) : character("O"), pos(inPos) {}
	const char* character;
	Vector2 pos;
};

class QuadTree : public Level
{
	RTTI_DECLARATIONS(QuadTree, Level)

public:
	QuadTree();
	virtual ~QuadTree();

	// �ܼ� â ũ�� ���� �Լ�
	void SetConsoleWindow(const int newWidth, const int newHeight);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	// TreeNode�� ������ ��û�޴� �Լ�
	void SubdivisionCall(TreeNode* node);
	
	// �ִϸ��̼� ���������� Ȯ���ϴ� �Լ�
	bool IsAnimating() const { return animating; }

	// ���� �ִϸ��̼� ��� �ð� getter
	float GetSubdivInterval() const { return subdivInterval; }

private:
	// �⺻ �ܼ� â ũ��
	int defaultWidth = 0;
	int defaultHeight = 0;

	// ���� Ʈ�� �ܼ� â ũ��
	int width = 0;
	int height = 0;

	// ���� (�ؽ�Ʈ ��¿�)
	int tempWidth = 20;

	// ȭ�� ũ�Ⱑ ����Ǹ�, ���� �������� ������ �ǳ� ��
	bool passFrame = false;

	// ���� �Է� �޴� �Լ�
	void IsTickInput();

	// ����Ű / �޴� ��� UI ���� �Լ�
	void IsRenderUI();

	// ��ȯ�� ������Ʈ ��ġ �迭
	std::vector<SpawnObject> spawnObjects;

	// ��Ʈ ���
	TreeNode* root = nullptr;

	// ���� �ִϸ��̼� Ÿ�̸�
	Timer subdivTimer;

	// ���� �ִϸ��̼� ��� �ð�
	float subdivInterval = 0.2f;

	// ���� �ִϸ��̼��� ������ ��� ���
	std::deque<TreeNode*> subdivQueue;

	// ���� �ִϸ��̼� ���������� ����
	bool animating = true;
};
