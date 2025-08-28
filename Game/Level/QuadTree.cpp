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

//std::vector<TreeNode*> QuadTree::Query(TreeNode* queryNode)
//{
//	return std::vector<TreeNode*>();
//}

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
		Vector2 spawnPos = Input::Get().GetMousePosition();

		// ���콺 ��ġ�� ��ȿ�� �������� Ȯ��
		if (spawnPos.x > 0 && spawnPos.x < width - 1 &&
			spawnPos.y > 0 && spawnPos.y < height - 1)
		{
			// �ش� ��ġ�� �̹� ������Ʈ�� ��ȯ�� ��� �н�
			for (const auto& i : spawnObjects)
			{
				if (spawnPos == i.pos) return;
			}

			// ���ο� ��ü ���� �� �迭�� �߰�
			spawnObjects.emplace_back(SpawnObject(spawnPos));

			// ���� ���ڸ� �ް� ������, Bounds ������ 1x1�� ����
			Bounds objBounds((float)spawnPos.x, (float)spawnPos.y, 1.0f, 1.0f);

			// ��ü�� TreeNode�� ���ο���, ��Ʈ ��忡 ����
			TreeNode* objNode = new TreeNode(objBounds);
			root->Insert(objNode);
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