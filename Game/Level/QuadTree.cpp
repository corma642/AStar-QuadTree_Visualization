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

	// ���� �Է� �ޱ� (���� Ʈ�� ��� �߰��� ���� ����)
	IsTickInput();
}

void QuadTree::Render()
{
	super::Render();

	// ����Ű / �޴� ��� UI ����
	IsRenderUI();

	// ���� Ʈ�� ����
	root->Render();

	// ��ȯ�� ������Ʈ ����
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
	// ���� �Է�
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().Quit();
	}

	// ��Ŭ��: ������Ʈ ��ȯ ��ġ ����
	if (Input::Get().GetMouseLeftButton())
	{
		Vector2 mousePos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (mousePos.x > 0 && mousePos.x < width - 1 &&
			mousePos.y > 0 && mousePos.y < height - 1)
		{
			// �ش� ��ġ�� �̹� ������Ʈ�� ��ȯ�� ��� �н�
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
	// ����Ű ���� ���
	char buffer1[50]{ "[ ������Ʈ ��ġ ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ ���� ]" };
	char buffer4[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, 1), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 2), buffer2, Color::LightGreen);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer4, Color::LightRed);
}