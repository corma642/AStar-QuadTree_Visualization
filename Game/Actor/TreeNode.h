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

	// 삽입
	void Insert(TreeNode* node);

	// 질의
	void Query(const Bounds& queryBounds, std::vector<TreeNode*>& possibleNodes);

	// Getter
	Bounds GetBounds() const { return bounds; }

	// 현재 노드에 포함된 노드 (배열)
	const std::vector<TreeNode*>& GetPoints() const { return points; }

	// 4분면 분할 함수
	bool Subdivide();

	// 분할됐는지 확인하는 함수
	bool IsDivided();

	// 정리 함수
	void Clear();

	// 자식 노드
	TreeNode* GetTopLeft() const { return topLeft; }
	TreeNode* GetTopRight() const { return topRight; }
	TreeNode* GetBottomLeft() const { return bottomLeft; }
	TreeNode* GetBottomRight() const { return bottomRight; }

private:
	// 깊이
	int depth = 0;

	// 영역
	Bounds bounds;

	// 현재 노드에 포함된 노드 (배열)
	std::vector<TreeNode*> points;

	// 노드 색상
	Color color;

	// 자식 노드
	TreeNode* topLeft = nullptr;
	TreeNode* topRight = nullptr;
	TreeNode* bottomLeft = nullptr;
	TreeNode* bottomRight = nullptr;
};

