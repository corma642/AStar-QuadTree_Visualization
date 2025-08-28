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

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	// TreeNode가 분할을 요청받는 함수
	void SubdivisionCall(TreeNode* node);
	
	// 애니메이션 실행중인지 확인하는 함수
	bool IsAnimating() const { return animating; }

	// 분할 애니메이션 재생 시간 getter
	float GetSubdivInterval() const { return subdivInterval; }

private:
	// 콘솔 창 크기
	int width = 0;
	int height = 0;

	// 여백 (텍스트 출력용)
	int tempWidth = 20;

	// 조작 입력 받는 함수
	void IsTickInput();

	// 조작키 / 메뉴 등등 UI 렌더 함수
	void IsRenderUI();

	// 소환된 오브젝트 위치 배열
	std::vector<SpawnObject> spawnObjects;

	// 루트 노드
	TreeNode* root = nullptr;

	// 분할 애니메이션 타이머
	Timer subdivTimer;

	// 분할 애니메이션 재생 시간
	float subdivInterval = 0.2f;

	// 분할 애니메이션을 수행할 노드 목록
	std::deque<TreeNode*> subdivQueue;

	// 현재 애니메이션 진행중인지 여부
	bool animating = true;
};
