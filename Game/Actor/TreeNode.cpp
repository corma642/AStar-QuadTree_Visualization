#include "TreeNode.h"
#include "Game/Game.h"
#include "Utils/Utils.h"

TreeNode::TreeNode(const Bounds& inBounds, int inDepth, Color TreeNodeColor)
	: bounds(inBounds), depth(inDepth), color(static_cast<Color>(Utils::Random(9, 14)))
{
}

TreeNode::~TreeNode()
{
	// 정리 함수 호출
	Clear();
}

void TreeNode::Render()
{
	// 현재 노드 출력
	for (int y = (int)bounds.GetY(); y < (int)bounds.MaxY(); ++y)
	{
		for (int x = (int)bounds.GetY(); x < (int)bounds.MaxX(); ++x)
		{
			// 분할된 경우, 십자 경계선까지 출력
			if (IsDivided())
			{
				if (y == (int)bounds.GetY() ||
					x == (int)bounds.GetX() ||
					y == (int)bounds.MaxY() - 1 ||
					x == (int)bounds.MaxX() - 1 ||
					y == (int)(bounds.MaxY() - bounds.GetY()) / 2 ||
					x == (int)(bounds.MaxX() - bounds.GetX()) / 2)
				{
					Engine::Get().WriteToBuffer({ x, y }, "*", color);
				}
			}
			else
			{
				if (y == (int)bounds.GetY() ||
					x == (int)bounds.GetX() ||
					y == (int)bounds.GetHeight() - 1 ||
					x == (int)bounds.GetWidth() - 1)
				{
					Engine::Get().WriteToBuffer({ x, y }, "*", color);
				}
			}
		}
	}

	// 분할된 경우 자손 노드들도 출력
	if (IsDivided())
	{
		topLeft->Render();
		topRight->Render();
		bottomLeft->Render();
		bottomRight->Render();
	}
}

void TreeNode::Insert(TreeNode* TreeNode)
{

}

void TreeNode::Query(
	const Bounds& queryBounds,
	std::vector<TreeNode*>& possibleTreeNodes)
{
}

bool TreeNode::Subdivide()
{
	return false;
}

bool TreeNode::IsDivided()
{
	return topLeft != nullptr;
}

// 자신 포함, 분할된 자손이 있다면 자손까지 정리
void TreeNode::Clear()
{
	// 현재 노드 정리
	for (TreeNode* TreeNode : points)
	{
		SafeDelete(TreeNode);
	}
	points.clear();

	// 분할된 경우 자손 노드들도 정리
	if (IsDivided())
	{
		topLeft->Clear();
		topRight->Clear();
		bottomLeft->Clear();
		bottomRight->Clear();

		SafeDelete(topLeft);
		SafeDelete(topRight);
		SafeDelete(bottomLeft);
		SafeDelete(bottomRight);
	}
}