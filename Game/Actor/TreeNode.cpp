#include "TreeNode.h"
#include "Game/Game.h"
#include "Utils/Utils.h"

TreeNode::TreeNode(const Bounds& inBounds, int inDepth, Color parentNodeColor)
	: bounds(inBounds), depth(inDepth)
{
	//color = parentNodeColor;
	while (true)
	{
		color = (static_cast<Color>(Utils::Random(9, 14)));
		if (color != parentNodeColor) break;
	}
}

TreeNode::~TreeNode()
{
	// ���� �Լ� ȣ��
	Clear();
}

void TreeNode::Render()
{
	// ���� ��ǥ�� ��ȯ
	int cX = static_cast<int>(bounds.GetX());
	int cY = static_cast<int>(bounds.GetY());
	int mX = static_cast<int>(bounds.MaxX());
	int mY = static_cast<int>(bounds.MaxY());

	// ���� ũ�Ⱑ 0 �����̸� �ƹ��͵� �׸��� ����
	if (mX <= cX || mY <= cY) return;

	// �߾Ӽ� ��ǥ (���� ��ǥ)
	int midX = cX + (mX - cX) / 2;
	int midY = cY + (mY - cY) / 2;

	// ��� ���
	for (int y = cY; y < mY; y++)
	{
		for (int x = cX; x < mX; x++)
		{
			// ���ҵ� ��쿡�� �߾� ���ڼ����� ���
			if (IsDivided())
			{
				// ���ҵ� ���: �ܰ��� + �߾� ���ڼ�
				if (y == cY || y == mY - 1 || y == midY)
				{
					Engine::Get().WriteToBuffer({ x, y }, "-", color);
				}
				else if (x == cX || x == mX - 1 || x == midX)
				{
					Engine::Get().WriteToBuffer({ x, y }, "|", color);
				}
			}
			else
			{
				if (y == cY || y == mY - 1)
				{
					Engine::Get().WriteToBuffer({ x, y }, "-", color);
				}
				else if (x == cX || x == mX - 1)
				{
					Engine::Get().WriteToBuffer({ x, y }, "|", color);
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

void TreeNode::Insert(TreeNode* node)
{
	// #1: ���� ����� �� ���� �ȿ� ���� �ִ��� Ȯ��
	if (!bounds.Intersects(node->GetBounds())) return;

	// #2: ���ҵ��� ����. �뷮�� �����. ���� ��忡 ����
	if (!IsDivided() && points.size() < capacity)
	{
		points.emplace_back(node);
		return;
	}

	// #3: ���ҵ��� ����. �뷮�� ��� ��. ���� ����
	if (!IsDivided())
	{
		// ���ҿ� �����ϴ� ��� (�ִ� ������ ���)
		if (!Subdivide())
		{
			// ���� ��忡 ����
			points.emplace_back(node);
			return;
		}
	}

	// #4: ���� ��. �ڽĿ��� ���� �õ�
	// �ڽ��� Bounds�� �����Ϸ��� ��ü�� Bounds�� �����ϸ�, ����
	if (topLeft->GetBounds().Intersects(node->GetBounds())) topLeft->Insert(node);
	if (topRight->GetBounds().Intersects(node->GetBounds())) topRight->Insert(node);
	if (bottomLeft->GetBounds().Intersects(node->GetBounds())) bottomLeft->Insert(node);
	if (bottomRight->GetBounds().Intersects(node->GetBounds())) bottomRight->Insert(node);
}

void TreeNode::Query(
	const Bounds& queryBounds,
	std::vector<TreeNode*>& possibleNodes)
{
	// #1: �� ������ ���� ������ ��ġ�� ������ ����
	if (!bounds.Intersects(queryBounds)) return;

	// #2: ���� ��忡 �ִ� ��ü���� �ĺ��� �߰�
	for (TreeNode* point : points)
	{
		if (queryBounds.Intersects(point->GetBounds()))
		{
			possibleNodes.emplace_back(point);
		}
	}

	// #3: ���ҵǾ� ������ �ڽĿ��Ե� ����
	if (IsDivided())
	{
		topLeft->Query(queryBounds, possibleNodes);
		topRight->Query(queryBounds, possibleNodes);
		bottomLeft->Query(queryBounds, possibleNodes);
		bottomRight->Query(queryBounds, possibleNodes);
	}
}

bool TreeNode::Subdivide()
{
	// �ִ� ������ ���, ����
	if (depth == Engine::Get().Depth()) return false;

	float x = bounds.GetX() + 1;
	float y = bounds.GetY() + 1;
	float halfW = bounds.GetWidth() / 2.0f - 1;
	float halfH = bounds.GetHeight() / 2.0f - 1;

	// ���� ����� Bounds�� 4����ؼ� �� �ڽ� ��带 ����
	topLeft = new TreeNode(Bounds(x, y, halfW, halfH), depth + 1);
	topRight = new TreeNode(Bounds(x + halfW, y, halfW, halfH), depth + 1);
	bottomLeft = new TreeNode(Bounds(x, y + halfH, halfW, halfH), depth + 1);
	bottomRight = new TreeNode(Bounds(x + halfW, y + halfH, halfW, halfH), depth + 1);

	return true;
}

bool TreeNode::IsDivided()
{
	// �ڽ��� �ϳ��� �ִٸ�, ���� �Ǿ��ٴ� �ǹ�
	return topLeft != nullptr;
}

// �ڽ� ����, ���ҵ� �ڼ��� �ִٸ� �ڼձ��� ����
void TreeNode::Clear()
{
	// ���� ��� ����
	if (!points.empty())
	{
		for (TreeNode*& point : points)
		{
			if (point)
			{
				SafeDelete(point);
			}
		}
	}
	points.clear();

	// ���ҵ� ���, �ڼ� ���鵵 ����
	if (topLeft) { topLeft->Clear(); SafeDelete(topLeft); }
	if (topRight) { topRight->Clear(); SafeDelete(topRight); }
	if (bottomLeft) { bottomLeft->Clear(); SafeDelete(bottomLeft); }
	if (bottomRight) { bottomRight->Clear(); SafeDelete(bottomRight); }
}