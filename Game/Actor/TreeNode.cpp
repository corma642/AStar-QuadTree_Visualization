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
	// 정리 함수 호출
	Clear();
}

void TreeNode::Render()
{
	// 정수 좌표로 변환
	int cX = static_cast<int>(bounds.GetX());
	int cY = static_cast<int>(bounds.GetY());
	int mX = static_cast<int>(bounds.MaxX());
	int mY = static_cast<int>(bounds.MaxY());

	// 영역 크기가 0 이하이면 아무것도 그리지 않음
	if (mX <= cX || mY <= cY) return;

	// 중앙선 좌표 (절대 좌표)
	int midX = cX + (mX - cX) / 2;
	int midY = cY + (mY - cY) / 2;

	// 노드 출력
	for (int y = cY; y < mY; y++)
	{
		for (int x = cX; x < mX; x++)
		{
			// 분할된 경우에는 중앙 십자선까지 출력
			if (IsDivided())
			{
				// 분할된 경우: 외곽선 + 중앙 십자선
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

	// 분할된 경우 자손 노드들도 출력
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
	// #1: 삽입 대상이 내 영역 안에 들어와 있는지 확인
	if (!bounds.Intersects(node->GetBounds())) return;

	// #2: 분할되지 않음. 용량이 충분함. 현재 노드에 저장
	if (!IsDivided() && points.size() < capacity)
	{
		points.emplace_back(node);
		return;
	}

	// #3: 분할되지 않음. 용량이 모두 참. 분할 진행
	if (!IsDivided())
	{
		// 분할에 실패하는 경우 (최대 깊이인 경우)
		if (!Subdivide())
		{
			// 현재 노드에 저장
			points.emplace_back(node);
			return;
		}
	}

	// #4: 분할 됨. 자식에게 삽입 시도
	// 자식의 Bounds와 삽입하려는 객체의 Bounds가 교차하면, 삽입
	if (topLeft->GetBounds().Intersects(node->GetBounds())) topLeft->Insert(node);
	if (topRight->GetBounds().Intersects(node->GetBounds())) topRight->Insert(node);
	if (bottomLeft->GetBounds().Intersects(node->GetBounds())) bottomLeft->Insert(node);
	if (bottomRight->GetBounds().Intersects(node->GetBounds())) bottomRight->Insert(node);
}

void TreeNode::Query(
	const Bounds& queryBounds,
	std::vector<TreeNode*>& possibleNodes)
{
	// #1: 내 영역이 질의 영역과 겹치지 않으면 무시
	if (!bounds.Intersects(queryBounds)) return;

	// #2: 현재 노드에 있는 객체들을 후보에 추가
	for (TreeNode* point : points)
	{
		if (queryBounds.Intersects(point->GetBounds()))
		{
			possibleNodes.emplace_back(point);
		}
	}

	// #3: 분할되어 있으면 자식에게도 질의
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
	// 최대 깊이인 경우, 실패
	if (depth == Engine::Get().Depth()) return false;

	float x = bounds.GetX() + 1;
	float y = bounds.GetY() + 1;
	float halfW = bounds.GetWidth() / 2.0f - 1;
	float halfH = bounds.GetHeight() / 2.0f - 1;

	// 현재 노드의 Bounds를 4등분해서 각 자식 노드를 생성
	topLeft = new TreeNode(Bounds(x, y, halfW, halfH), depth + 1);
	topRight = new TreeNode(Bounds(x + halfW, y, halfW, halfH), depth + 1);
	bottomLeft = new TreeNode(Bounds(x, y + halfH, halfW, halfH), depth + 1);
	bottomRight = new TreeNode(Bounds(x + halfW, y + halfH, halfW, halfH), depth + 1);

	return true;
}

bool TreeNode::IsDivided()
{
	// 자식이 하나라도 있다면, 분할 되었다는 의미
	return topLeft != nullptr;
}

// 자신 포함, 분할된 자손이 있다면 자손까지 정리
void TreeNode::Clear()
{
	// 현재 노드 정리
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

	// 분할된 경우, 자손 노드들도 정리
	if (topLeft) { topLeft->Clear(); SafeDelete(topLeft); }
	if (topRight) { topRight->Clear(); SafeDelete(topRight); }
	if (bottomLeft) { bottomLeft->Clear(); SafeDelete(bottomLeft); }
	if (bottomRight) { bottomRight->Clear(); SafeDelete(bottomRight); }
}