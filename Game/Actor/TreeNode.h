#pragma once

#include "Utils/Bounds.h"
#include "Math/Color.h"
#include "Utils/Timer.h"

#include <vector>

// �ִϸ��̼� ����
enum class AnimState
{
	Idle,
	AnimatingSubdivision
};

class QuadTree;

class TreeNode
{
public:
	TreeNode(
		const Bounds& inBounds,
		int inDepth = 0,
		Color parentNodeColor = Color::LightWhite,
		QuadTree* owner = nullptr
	);

	virtual ~TreeNode();

	// ���� �� ƽ �Լ�
	void Tick(float deltaTime);
	void Render();

	// ����
	void Insert(TreeNode* node);

	// ����. Ư�� ������ ��ġ�� ��� ��� ��ȯ �Լ�
	void Query(const Bounds& queryBounds, std::vector<TreeNode*>& possibleNodes);

	// Getter
	Bounds GetBounds() const { return bounds; }

	// ���� ��忡 ���Ե� ��� (�迭)
	const std::vector<TreeNode*>& GetPoints() const { return points; }

	// 4�и� ���� �Լ� (ȣ�� ��, ���� �ִϸ��̼� ���)
	bool Subdivide();

	// 4�и� ���� �Լ� (ȣ�� ��, ���� ���� ����)
	bool SubdivideNow();

	// ���ҵƴ��� Ȯ���ϴ� �Լ�
	bool IsDivided();

	// ���� �ִϸ��̼� ���� �Լ� (duration �� ���� �ð�ȭ).
	void BeginSubdivisionAnimation(float duration = 0.18f);

	// ���� ��û �÷��� ���� �Լ�
	void SetSubdivideRequested(bool v) { subdivideRequested = v; }
	bool IsSubdivideRequested() const { return subdivideRequested; }

	// ���� �Լ�
	void Clear();

	// �ڽ� ���
	TreeNode* GetTopLeft() const { return topLeft; }
	TreeNode* GetTopRight() const { return topRight; }
	TreeNode* GetBottomLeft() const { return bottomLeft; }
	TreeNode* GetBottomRight() const { return bottomRight; }

private:
	// ����
	int depth = 0;

	// ����
	Bounds bounds;

	// �뷮. �� ���� ��尡 ���� �� �ִ� ���(��ü) ��
	const int capacity = 1;

	// ���� ��忡 ���Ե� ��� (�迭)
	std::vector<TreeNode*> points;

	// ��� ����
	Color color;

	// �ڽ� ���
	TreeNode* topLeft = nullptr;
	TreeNode* topRight = nullptr;
	TreeNode* bottomLeft = nullptr;
	TreeNode* bottomRight = nullptr;

	// ���� Ʈ�� ����
	QuadTree* owner = nullptr;

	// �ִϸ��̼� ����
	AnimState animState = AnimState::Idle;

	// �ִϸ��̼� ��� �ð� �� �ִϸ��̼� ���� �ð�
	float animTimer = 0.0f;
	float animDuration = 0.18f;

	// �ߺ� ���� ȣ�� ���� �÷���
	bool subdivideRequested = false;
};
