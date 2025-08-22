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
//		// 도착지 도착 문구 출력
//		char buffer1[25]{ "[ !- 도착 -! ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Green);
//
//		// 재시작 문구 출력
//		char buffer2[50]{ "[ 재시작 >> \"Enter\" ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::White);
//
//		// 메인 메뉴로 가기 문구 출력
//		char buffer3[50]{ "[ 메인 메뉴로 가기 >> \"ESC\" ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::White);
//	}
//	else
//	{
//		char buffer1[25]{ "[ !- 미도착 -! ]" };
//		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Red);
//	}
//}
//
//void AStarLevel::ReadMapFile()
//{
//	// 최종 에셋 경로 완성
//	char filePath[256]{};
//	sprintf_s(filePath, 256, "../Assets/%s", "AStarMap.txt");
//
//	FILE* mapFile = nullptr;
//	fopen_s(&mapFile, filePath, "rb");
//
//	// 예외 처리
//	if (!mapFile)
//	{
//		__debugbreak();
//		return;
//	}
//
//	// 파싱(Parcing, 해석)
//	fseek(mapFile, 0, SEEK_END);
//	size_t fileSize = ftell(mapFile);
//	rewind(mapFile);
//
//	// 확인한 파일 크기를 활용해 버퍼 할당
//	char* buffer = new char[fileSize + 1];
//	memset(buffer, 0, fileSize + 1);
//	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);
//
//	int index = 0; // 배열 순회를 위한 인덱스 변수
//	int size = static_cast<int>(readSize); // 문자열 길이 값
//
//	// 문자 배열 순회
//	while (index < size)
//	{
//		// 맵 문자 확인
//		char mapCharacter = buffer[index++];
//
//		// 개행 문자 처리
//		if (mapCharacter == '\n')
//		{
//			// 다음 줄로 넘기면서, x 좌표 초기화
//			stagePos.x = 0;
//			++stagePos.y;
//			continue;
//		}
//
//		maze[y][x] = mapCharacter
//
//		// 각 문자별 처리
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
//		// x좌표 증가 처리
//		stagePos.x++;
//	}
//
//	// 데이터 정리
//	delete[] buffer;
//	fclose(mapFile);
//}