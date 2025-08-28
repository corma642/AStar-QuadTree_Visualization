#include "TreeNode.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Level/QuadTree.h"

TreeNode::TreeNode(const Bounds& inBounds, int inDepth, Color parentNodeColor, QuadTree* inOwner)
	: bounds(inBounds), depth(inDepth), owner(inOwner)
{
	// 색상: 부모 색과 다르게 랜덤 설정
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

void TreeNode::Tick(float deltaTime)
{
	// 애니메이션 타이머 갱신
	if (animState == AnimState::AnimatingSubdivision)
	{
		animTimer += deltaTime;

		// 애니메이션 경과 시간이 지속 시간을 넘은 경우
		if (animTimer >= animDuration)
		{
			// 애니메이션 종료 및 기본 상태로 변경
			animTimer = animDuration;
			animState = AnimState::Idle;

			// 애니메이션 끝난 시점에 실제 분할 실행
			SubdivideNow();
		}
	}

	// 자식 노드들도 Tick 수행
	if (IsDivided())
	{
		if (topLeft) topLeft->Tick(deltaTime);
		if (topRight) topRight->Tick(deltaTime);
		if (bottomLeft) bottomLeft->Tick(deltaTime);
		if (bottomRight) bottomRight->Tick(deltaTime);
	}
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

	// 애니메이션 진행도
	float progress = 0.0f;
	if (animState == AnimState::AnimatingSubdivision && animDuration > 0.0f)
	{
		progress = min(1.0f, animTimer / animDuration);
	}

	// 십자선 길이
	int halfW = (mX - cX) / 2;
	int halfH = (mY - cY) / 2;
	int lenX = static_cast<int>(halfW * progress);
	int lenY = static_cast<int>(halfH * progress);

	for (int y = cY; y < mY; ++y)
	{
		for (int x = cX; x < mX; ++x)
		{
			// 외곽선 항상 출력
			if (y == cY || y == mY - 1)
			{
				Engine::Get().WriteToBuffer({ x, y }, "-", color);
				continue;
			}
			if (x == cX || x == mX - 1)
			{
				Engine::Get().WriteToBuffer({ x, y }, "|", color);
				continue;
			}

			// 애니메이션 실행중인 경우: 중앙 십자선을 애니메이션 진행도만큼 출력
			if (animState == AnimState::AnimatingSubdivision)
			{
				if (y == midY)
				{
					if (x >= midX - lenX && x < midX + lenX)
					{
						Engine::Get().WriteToBuffer({ x, y }, "-", color);
						continue;
					}
				}
				if (x == midX)
				{
					if (y >= midY - lenY && y < midY + lenY)
					{
						Engine::Get().WriteToBuffer({ x, y }, "|", color);
						continue;
					}
				}
			}
			// 애니메이션 실행중이 아닌 경우
			else
			{
				// 분할된 상태라면 중앙선을 추가로 출력함 (외곽선 항상 출력)
				if (IsDivided())
				{
					if (y == midY)
					{
						Engine::Get().WriteToBuffer({ x, y }, "-", color);
						continue;
					}
					if (x == midX)
					{
						Engine::Get().WriteToBuffer({ x, y }, "|", color);
						continue;
					}
				}
			}
		}
	}

	// 자식 노드들도 출력
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
		// #4: 현재 애니메이션 모드인 경우
		if (owner && owner->IsAnimating())
		{
			// 분할을 지연하고 현재 노드에 임시 보관
			points.emplace_back(node);

			// 분할 요청
			owner->SubdivisionCall(this);
			return;
		}

		// #5: 현재 애니메이션 모드가 아닌 경우
		// 동기적으로 분할 시도
		if (!SubdivideNow())
		{
			// 분할 불가(최대 깊이 등) 할 경우 현재 노드에 임시 보관
			points.emplace_back(node);
			return;
		}
	}

	// #6: 분할되어 있다면 적절한 자식에게 삽입 (중복 삽입 방지)
	if (topLeft && topLeft->GetBounds().Intersects(node->GetBounds()))
	{
		topLeft->Insert(node);
	}
	else if (topRight && topRight->GetBounds().Intersects(node->GetBounds()))
	{
		topRight->Insert(node);
	}
	else if (bottomLeft && bottomLeft->GetBounds().Intersects(node->GetBounds()))
	{
		bottomLeft->Insert(node);
	}
	else if (bottomRight && bottomRight->GetBounds().Intersects(node->GetBounds()))
	{
		bottomRight->Insert(node);
	}
	else
	{
		// 경계 판정 문제로 어느 자식에도 들어가지 않는다면 안전하게 현재 노드에 임시 보관
		points.emplace_back(node);
	}
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
	// 분할 요청 시, 애니메이션 재생
	float subdivideAnimationDuration = owner->GetSubdivInterval();
	BeginSubdivisionAnimation(subdivideAnimationDuration);
	return true;
}

bool TreeNode::SubdivideNow()
{
	// 이미 분할되어 있으면 분할 성공
	if (IsDivided()) return true;

	// 최대 깊이인 경우 실패
	if (depth == Engine::Get().Depth()) return false;

	// 정수 좌표화(렌더와 동일 규칙)
	int x0 = static_cast<int>(bounds.GetX());
	int y0 = static_cast<int>(bounds.GetY());
	int x1 = static_cast<int>(bounds.MaxX()); // exclusive
	int y1 = static_cast<int>(bounds.MaxY()); // exclusive

	int w = x1 - x0;
	int h = y1 - y0;
	if (w <= 1 || h <= 1) return false;

	int midX = x0 + w / 2;
	int midY = y0 + h / 2;

	// 자식 생성 (owner 전달)
	topLeft = new TreeNode(Bounds((float)x0, (float)y0, (float)(midX - x0), (float)(midY - y0)), depth + 1, color, owner);
	topRight = new TreeNode(Bounds((float)midX, (float)y0, (float)(x1 - midX), (float)(midY - y0)), depth + 1, color, owner);
	bottomLeft = new TreeNode(Bounds((float)x0, (float)midY, (float)(midX - x0), (float)(y1 - midY)), depth + 1, color, owner);
	bottomRight = new TreeNode(Bounds((float)midX, (float)midY, (float)(x1 - midX), (float)(y1 - midY)), depth + 1, color, owner);


	// 기존 points를 자식에게 재분배 (소유권이 자식에게 이동)
	if (!points.empty())
	{
		for (TreeNode* p : points)
		{
			if (!p) continue;

			if (topLeft->GetBounds().Intersects(p->GetBounds()))      topLeft->Insert(p);
			else if (topRight->GetBounds().Intersects(p->GetBounds())) topRight->Insert(p);
			else if (bottomLeft->GetBounds().Intersects(p->GetBounds())) bottomLeft->Insert(p);
			else if (bottomRight->GetBounds().Intersects(p->GetBounds())) bottomRight->Insert(p);
			else topLeft->Insert(p); // 안전 대처
		}
		// 부모는 더 이상 이 포인터들을 소유하지 않음. 삭제하지 않고 벡터만 비움
		points.clear();
	}

	return true;
}

bool TreeNode::IsDivided()
{
	// 자식이 하나라도 있다면, 분할 되었다는 의미
	return topLeft != nullptr;
}

void TreeNode::BeginSubdivisionAnimation(float duration)
{
	// 이미 분할 애니메이션이 진행중이면 반환
	if (animState == AnimState::AnimatingSubdivision) return;

	// 애니메이션 지속 시간 초기화 및 애니메이션 상태 설정
	animDuration = duration;
	animTimer = 0.0f;
	animState = AnimState::AnimatingSubdivision;
}

void TreeNode::Clear()
{
	// 현재 노드에 포함된 노드 제거
	if (!points.empty())
	{
		for (TreeNode*& point : points)
		{
			if (point)
			{
				SafeDelete(point);
			}
		}
		points.clear();
	}

	// 자식 노드들이 있으면 재귀 정리 및 삭제
	if (topLeft) { topLeft->Clear(); SafeDelete(topLeft); }
	if (topRight) { topRight->Clear(); SafeDelete(topRight); }
	if (bottomLeft) { bottomLeft->Clear(); SafeDelete(bottomLeft); }
	if (bottomRight) { bottomRight->Clear(); SafeDelete(bottomRight); }
}