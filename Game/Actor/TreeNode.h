#pragma once

#include "Utils/Bounds.h"
#include "Math/Color.h"
#include "Utils/Timer.h"

#include <vector>

// 애니메이션 상태
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

	// 렌더 및 틱 함수
	void Tick(float deltaTime);
	void Render();

	// 삽입
	void Insert(TreeNode* node);

	// 질의. 특정 영역과 겹치는 모든 노드 반환 함수
	void Query(const Bounds& queryBounds, std::vector<TreeNode*>& possibleNodes);

	// Getter
	Bounds GetBounds() const { return bounds; }

	// 현재 노드에 포함된 노드 (배열)
	const std::vector<TreeNode*>& GetPoints() const { return points; }

	// 4분면 분할 함수 (호출 시, 분할 애니메이션 재생)
	bool Subdivide();

	// 4분면 분할 함수 (호출 시, 실제 구조 분할)
	bool SubdivideNow();

	// 분할됐는지 확인하는 함수
	bool IsDivided();

	// 분할 애니메이션 시작 함수 (duration 초 동안 시각화).
	void BeginSubdivisionAnimation(float duration = 0.18f);

	// 분할 요청 플래그 제어 함수
	void SetSubdivideRequested(bool v) { subdivideRequested = v; }
	bool IsSubdivideRequested() const { return subdivideRequested; }

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

	// 용량. 각 쿼리 노드가 가질 수 있는 노드(객체) 수
	const int capacity = 1;

	// 현재 노드에 포함된 노드 (배열)
	std::vector<TreeNode*> points;

	// 노드 색상
	Color color;

	// 자식 노드
	TreeNode* topLeft = nullptr;
	TreeNode* topRight = nullptr;
	TreeNode* bottomLeft = nullptr;
	TreeNode* bottomRight = nullptr;

	// 쿼드 트리 레벨
	QuadTree* owner = nullptr;

	// 애니메이션 상태
	AnimState animState = AnimState::Idle;

	// 애니메이션 경과 시간 및 애니메이션 지속 시간
	float animTimer = 0.0f;
	float animDuration = 0.18f;

	// 중복 분할 호출 방지 플래그
	bool subdivideRequested = false;
};
