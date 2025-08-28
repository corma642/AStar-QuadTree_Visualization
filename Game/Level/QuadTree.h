#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"
#include "Actor/TreeNode.h"

// 쿼드 트리에 소환할 오브젝트
struct SpawnObject
{
	SpawnObject(const Vector2 inPos) : character("O"), pos(inPos)
	{
	}

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

	//// 전달한 객체와 겹치는 노드를 반환하는 함수 (질의-Query)
	//std::vector<TreeNode*> Query(TreeNode* queryNode);

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
};