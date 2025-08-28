#include "QuadTree.h"
#include "game/game.h"
#include "utils/utils.h"
#include "Utils/Bounds.h"

QuadTree::QuadTree()
{
	// width = 150
	// height = 47

	width = Engine::Get().Width() - tempWidth;
	height = Engine::Get().Height();

	root = new TreeNode(Bounds(0, 0, (float)width, (float)height), 0);
}

QuadTree::~QuadTree()
{
	SafeDelete(root);
}

void QuadTree::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 조작 입력 받기 (쿼드 트리 노드 추가도 같이 구현)
	IsTickInput();
}

void QuadTree::Render()
{
	super::Render();

	// 조작키 / 메뉴 등등 UI 렌더
	IsRenderUI();

	// 쿼드 트리 렌더
	root->Render();

	// 소환된 오브젝트 렌더
	for (const SpawnObject& obj : spawnObjects)
	{
		Engine::Get().WriteToBuffer(obj.pos, obj.character, Color::LightWhite);
	}
}

//std::vector<TreeNode*> QuadTree::Query(TreeNode* queryNode)
//{
//	return std::vector<TreeNode*>();
//}

void QuadTree::IsTickInput()
{
	// 종료 입력
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().Quit();
	}

	// 좌클릭: 오브젝트 소환 위치 설정
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 spawnPos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (spawnPos.x > 0 && spawnPos.x < width - 1 &&
			spawnPos.y > 0 && spawnPos.y < height - 1)
		{
			// 해당 위치에 이미 오브젝트가 소환된 경우 패스
			for (const auto& i : spawnObjects)
			{
				if (spawnPos == i.pos) return;
			}

			// 새로운 객체 생성 및 배열에 추가
			spawnObjects.emplace_back(SpawnObject(spawnPos));

			// 단일 문자를 받고 있으니, Bounds 범위를 1x1로 잡음
			Bounds objBounds((float)spawnPos.x, (float)spawnPos.y, 1.0f, 1.0f);

			// 객체를 TreeNode로 래핑에서, 루트 노드에 삽입
			TreeNode* objNode = new TreeNode(objBounds);
			root->Insert(objNode);
		}
	}
}

void QuadTree::IsRenderUI()
{
	// 조작키 문구 출력
	char buffer1[50]{ "[ 오브젝트 설치 ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ 종료 ]" };
	char buffer4[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, 1), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 2), buffer2, Color::LightGreen);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer4, Color::LightRed);
}