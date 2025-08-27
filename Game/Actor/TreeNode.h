#pragma once

#include "Utils/Bounds.h"
#include "Math/Color.h"

#include <vector>

class TreeNode
{
public:
	TreeNode(const Bounds& inBounds, int inDepth = 0, Color nodeColor = Color::LightWhite);
	virtual ~TreeNode();

	void Render();

	// ����
	void Insert(TreeNode* node);

	// ����
	void Query(const Bounds& queryBounds, std::vector<TreeNode*>& possibleNodes);

	// Getter
	Bounds GetBounds() const { return bounds; }

	// ���� ��忡 ���Ե� ��� (�迭)
	const std::vector<TreeNode*>& GetPoints() const { return points; }

	// 4�и� ���� �Լ�
	bool Subdivide();

	// ���ҵƴ��� Ȯ���ϴ� �Լ�
	bool IsDivided();

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

	// ���� ��忡 ���Ե� ��� (�迭)
	std::vector<TreeNode*> points;

	// ��� ����
	Color color;

	// �ڽ� ���
	TreeNode* topLeft = nullptr;
	TreeNode* topRight = nullptr;
	TreeNode* bottomLeft = nullptr;
	TreeNode* bottomRight = nullptr;
};

