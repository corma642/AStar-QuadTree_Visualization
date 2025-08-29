#include "QuadTree.h"
#include "game/game.h"
#include "utils/utils.h"
#include "Utils/Bounds.h"

QuadTree::QuadTree()
{
	width = 130;
	height = 47;

	//// 기본 콘솔 창 크기 저장
	//defaultWidth = Engine::Get().Width();
	//defaultHeight = Engine::Get().Height();

	//// 엔진의 창 크기를 쿼드 트리 콘솔 창 크기로 변경
	//width = defaultWidth + 50;
	//height = defaultHeight + 17;

	//// 콘솔 창 크기 설정
	//SetConsoleWindow(width, height);

	// 시작 depth는 0, owner에 QuadTreeLevel 전달
	root = new TreeNode(Bounds(0, 0, (float)width, (float)height), 0, Color::LightWhite, this);

	// 애니메이션 초기화
	subdivTimer.SetTargetTime(subdivInterval);
	animating = true;
}

QuadTree::~QuadTree()
{
	// 애니메이션 데이터 초기화
	animating = false;
	subdivQueue.clear();

	// 루트 노드부터 재귀적으로 쿼드 트리 제거
	SafeDelete(root);
}

void QuadTree::SetConsoleWindow(const int newWidth, const int newHeight)
{
	// 버퍼 크기 변경 요청
	Engine::Get().ChangeBufferSizeRequset(newWidth, newHeight);

	// UI 출력할 여백만큼 창 크기 줄임
	width -= tempWidth;
}

void QuadTree::SubdivisionCall(TreeNode* node)
{
	if (!node) return;

	// 이미 분할 요청된 노드의 경우, 중복 요청 방지
	if (node->IsSubdivideRequested()) return;

	// 분할 애니메이션 수행 목록에 추가 및 호출 요청 처리
	subdivQueue.push_back(node);
	node->SetSubdivideRequested(true);
}

void QuadTree::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리 (오브젝트 생성 및 데이터 삽입)
	IsTickInput();

	// 애니메이션 처리
	if (animating && !subdivQueue.empty())
	{
		subdivTimer.Tick(deltaTime);

		if (subdivTimer.IsTimeout())
		{
			// 큐에서 하나씩 꺼내 애니메이션/분할 처리
			TreeNode* node = subdivQueue.front();
			subdivQueue.pop_front();

			// 분할 처리될 노드의 분할 요청 완료 처리
			node->SetSubdivideRequested(false);

			// 분할 애니메이션 시작 호출
			// 실제 분할은 node 내부에서 애니메이션이 끝날 때 수행됨
			node->BeginSubdivisionAnimation(subdivInterval);

			// 애니메이션 타이머 초기화
			subdivTimer.Reset();
		}
	}

	// 모든 노드의 애니메이션 타이머 업데이트
	root->Tick(deltaTime);
}

void QuadTree::Render()
{
	super::Render();

	// UI 렌더
	IsRenderUI();

	// 쿼드 트리 렌더
	if (root) root->Render();

	// 소환된 오브젝트 렌더
	for (const SpawnObject& obj : spawnObjects)
	{
		Engine::Get().WriteToBuffer(obj.pos, obj.character, Color::LightWhite);
	}
}

void QuadTree::IsTickInput()
{
	// 종료 입력
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 콘솔 창 기본 크기로 설정
		//SetConsoleWindow(defaultWidth, defaultHeight);

		Game::Get().ChangeAlgorithmSelectLevel();
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

			// Bounds 범위를 1x1로 잡음
			Bounds objBounds((float)spawnPos.x, (float)spawnPos.y, 1.0f, 1.0f);

			// 객체를 TreeNode로 래핑해서, 루트 노드에 삽입 (QuadTreeLevel 전달)
			TreeNode* objNode = new TreeNode(objBounds, 0, Color::LightWhite, this);
			root->Insert(objNode);
		}
	}
}

void QuadTree::IsRenderUI()
{
	char buffer1[50]{ "[ 오브젝트 설치 ]" };
	char buffer2[50]{ "> \"Left Click\"" };

	char buffer3[50]{ "[ 종료 ]" };
	char buffer4[50]{ "> \"ESC\"" };

	Engine::Get().WriteToBuffer(Vector2(width + 1, 1), buffer1, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, 2), buffer2, Color::LightGreen);

	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 3), buffer3, Color::LightWhite);
	Engine::Get().WriteToBuffer(Vector2(width + 1, height - 2), buffer4, Color::LightRed);
}
