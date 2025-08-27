#include "QuadTree.h"
#include "game/game.h"
#include "utils/utils.h"
#include "Utils/Bounds.h"

QuadTree::QuadTree()
{
	width = Engine::Get().Width() - tempWidth;
	height = Engine::Get().Height();

	root = new TreeNode(Bounds(0, 0, (float)width, (float)height), maxDepth);
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

std::vector<TreeNode*> QuadTree::Query(TreeNode* queryNode)
{
	return std::vector<TreeNode*>();
}

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
		Vector2 mousePos = Input::Get().GetMousePosition();

		// 마우스 위치가 유효한 범위인지 확인
		if (mousePos.x > 0 && mousePos.x < width - 1 &&
			mousePos.y > 0 && mousePos.y < height - 1)
		{
			// 해당 위치에 이미 오브젝트가 소환된 경우 패스
			for (const auto& i : spawnObjects)
			{
				if (mousePos == i.pos) return;
			}

			spawnObjects.emplace_back(SpawnObject(mousePos));
			

			//root = new TreeNode(Bounds(0, 0, (float)width, (float)height), maxDepth);
			//Insert()
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