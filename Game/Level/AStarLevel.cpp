//#include "AStarLevel.h"
//#include "math/vector2.h"
//#include "game/game.h"
//#include "utils/utils.h"
//#include "level/level.h"
//
//#include <iostream>
//
//AStarLevel::AStarLevel(const int currnetRenderSpeed)
//{
//	ReadMapFile();
//}
//
//void AStarLevel::Tick(float deltaTime)
//{
//	super::Tick(deltaTime);
//
//	if (bIsArrived)
//	{
//		if (Input::Get().GetKey(VK_RETURN))
//		{
//			IsStageClear = false;
//			Game::Get().StartAStar(currnetRenderSpeed);
//		}
//
//		if (Input::Get().GetKey(VK_ESCAPE))
//		{
//			IsStageClear = false;
//			Game::Get().ChangeMainMenu();
//		}
//	}
//}
//
//void AStarLevel::Render()
//{
//	super::Render();
//
//	if (bIsArrived)
//	{
//		// ������ ���� ���� ���
//		char buffer1[25]{ "[ !- ���� -! ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Green);
//
//		// ����� ���� ���
//		char buffer2[50]{ "[ ����� >> \"Enter\" ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::White);
//
//		// ���� �޴��� ���� ���� ���
//		char buffer3[50]{ "[ ���� �޴��� ���� >> \"ESC\" ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::White);
//	}
//	else
//	{
//		char buffer1[25]{ "[ !- �̵��� -! ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Red);
//	}
//}
//
//void AStarLevel::ReadMapFile()
//{
//	// ���� ���� ��� �ϼ�
//	char filePath[256]{};
//	sprintf_s(filePath, 256, "../Assets/%s", "AStarMap.txt");
//
//	FILE* mapFile = nullptr;
//	fopen_s(&mapFile, filePath, "rb");
//
//	// ���� ó��
//	if (!mapFile)
//	{
//		__debugbreak();
//		return;
//	}
//
//	// �Ľ�(Parcing, �ؼ�)
//	fseek(mapFile, 0, SEEK_END);
//	size_t fileSize = ftell(mapFile);
//	rewind(mapFile);
//
//	// Ȯ���� ���� ũ�⸦ Ȱ���� ���� �Ҵ�
//	char* buffer = new char[fileSize + 1];
//	memset(buffer, 0, fileSize + 1);
//	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);
//
//	int index = 0; // �迭 ��ȸ�� ���� �ε��� ����
//	int size = static_cast<int>(readSize); // ���ڿ� ���� ��
//
//	// ���� �迭 ��ȸ
//	while (index < size)
//	{
//		// �� ���� Ȯ��
//		char mapCharacter = buffer[index++];
//
//		// ���� ���� ó��
//		if (mapCharacter == '\n')
//		{
//			// ���� �ٷ� �ѱ�鼭, x ��ǥ �ʱ�ȭ
//			stagePos.x = 0;
//			++stagePos.y;
//			continue;
//		}
//
//		maze[y][x] = mapCharacter
//
//		// �� ���ں� ó��
//		switch (mapCharacter)
//		{
//		case '8':
//			maze[y][x] = '8';
//			wall.push_back({ stagePos.y, stagePos.x });
//			break;
//		case '.':
//			//AddActor(new Dark(pos));
//			AddActor(new Score(stagePos));
//			remainingScore++;
//			break;
//		}
//
//		// x��ǥ ���� ó��
//		stagePos.x++;
//	}
//
//	// ������ ����
//	delete[] buffer;
//	fclose(mapFile);
//}