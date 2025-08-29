#include "QuadTree.h"
#include "game/game.h"
#include "utils/utils.h"
#include "Utils/Bounds.h"

QuadTree::QuadTree()
{
	width = 130;
	height = 47;

	//// �⺻ �ܼ� â ũ�� ����
	//defaultWidth = Engine::Get().Width();
	//defaultHeight = Engine::Get().Height();

	//// ������ â ũ�⸦ ���� Ʈ�� �ܼ� â ũ��� ����
	//width = defaultWidth + 50;
	//height = defaultHeight + 17;

	//// �ܼ� â ũ�� ����
	//SetConsoleWindow(width, height);

	// ���� depth�� 0, owner�� QuadTreeLevel ����
	root = new TreeNode(Bounds(0, 0, (float)width, (float)height), 0, Color::LightWhite, this);

	// �ִϸ��̼� �ʱ�ȭ
	subdivTimer.SetTargetTime(subdivInterval);
	animating = true;
}

QuadTree::~QuadTree()
{
	// �ִϸ��̼� ������ �ʱ�ȭ
	animating = false;
	subdivQueue.clear();

	// ��Ʈ ������ ��������� ���� Ʈ�� ����
	SafeDelete(root);
}

void QuadTree::SetConsoleWindow(const int newWidth, const int newHeight)
{
	// ���� ũ�� ���� ��û
	Engine::Get().ChangeBufferSizeRequset(newWidth, newHeight);

	// UI ����� ���鸸ŭ â ũ�� ����
	width -= tempWidth;
}

void QuadTree::SubdivisionCall(TreeNode* node)
{
	if (!node) return;

	// �̹� ���� ��û�� ����� ���, �ߺ� ��û ����
	if (node->IsSubdivideRequested()) return;

	// ���� �ִϸ��̼� ���� ��Ͽ� �߰� �� ȣ�� ��û ó��
	subdivQueue.push_back(node);
	node->SetSubdivideRequested(true);
}

void QuadTree::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �Է� ó�� (������Ʈ ���� �� ������ ����)
	IsTickInput();

	// �ִϸ��̼� ó��
	if (animating && !subdivQueue.empty())
	{
		subdivTimer.Tick(deltaTime);

		if (subdivTimer.IsTimeout())
		{
			// ť���� �ϳ��� ���� �ִϸ��̼�/���� ó��
			TreeNode* node = subdivQueue.front();
			subdivQueue.pop_front();

			// ���� ó���� ����� ���� ��û �Ϸ� ó��
			node->SetSubdivideRequested(false);

			// ���� �ִϸ��̼� ���� ȣ��
			// ���� ������ node ���ο��� �ִϸ��̼��� ���� �� �����
			node->BeginSubdivisionAnimation(subdivInterval);

			// �ִϸ��̼� Ÿ�̸� �ʱ�ȭ
			subdivTimer.Reset();
		}
	}

	// ��� ����� �ִϸ��̼� Ÿ�̸� ������Ʈ
	root->Tick(deltaTime);
}

void QuadTree::Render()
{
	super::Render();

	// UI ����
	IsRenderUI();

	// ���� Ʈ�� ����
	if (root) root->Render();

	// ��ȯ�� ������Ʈ ����
	for (const SpawnObject& obj : spawnObjects)
	{
		Engine::Get().WriteToBuffer(obj.pos, obj.character, Color::LightWhite);
	}
}

void QuadTree::IsTickInput()
{
	// ���� �Է�
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// �ܼ� â �⺻ ũ��� ����
		//SetConsoleWindow(defaultWidth, defaultHeight);

		Game::Get().ChangeAlgorithmSelectLevel();
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

			// Bounds ������ 1x1�� ����
			Bounds objBounds((float)spawnPos.x, (float)spawnPos.y, 1.0f, 1.0f);

			// ��ü�� TreeNode�� �����ؼ�, ��Ʈ ��忡 ���� (QuadTreeLevel ����)
			TreeNode* objNode = new TreeNode(objBounds, 0, Color::LightWhite, this);
			root->Insert(objNode);
		}
	}
}

void QuadTree::IsRenderUI()
{
	char buffer1[50]{ "[ ������Ʈ ��ġ ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ ���� ]" };
	char buffer4[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, 1), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 2), buffer2, Color::LightGreen);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer4, Color::LightRed);
}
