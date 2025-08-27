#include "TreeNode.h"
#include "Game/Game.h"
#include "Utils/Utils.h"

TreeNode::TreeNode(const Bounds& inBounds, int inDepth, Color TreeNodeColor)
	: bounds(inBounds), depth(inDepth), color(static_cast<Color>(Utils::Random(9, 14)))
{
}

TreeNode::~TreeNode()
{
	// ���� �Լ� ȣ��
	Clear();
}

void TreeNode::Render()
{
	// ���� ��� ���
	for (int y = (int)bounds.GetY(); y < (int)bounds.MaxY(); ++y)
	{
		for (int x = (int)bounds.GetY(); x < (int)bounds.MaxX(); ++x)
		{
			// ���ҵ� ���, ���� ��輱���� ���
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

	// ���ҵ� ��� �ڼ� ���鵵 ���
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

// �ڽ� ����, ���ҵ� �ڼ��� �ִٸ� �ڼձ��� ����
void TreeNode::Clear()
{
	// ���� ��� ����
	for (TreeNode* TreeNode : points)
	{
		SafeDelete(TreeNode);
	}
	points.clear();

	// ���ҵ� ��� �ڼ� ���鵵 ����
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