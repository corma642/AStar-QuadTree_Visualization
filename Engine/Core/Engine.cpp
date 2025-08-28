#include "Engine.h"
#include "Level\Level.h"
#include "Utils/Utils.h"
#include "Render/ScreenBuffer.h"

#include <iostream>

// 정적 변수 초기화
Engine* Engine::instance = nullptr;

BOOL WINAPI ConsoleMessageProcedure(DWORD CtrlType)
{
	switch (CtrlType)
	{
	case CTRL_CLOSE_EVENT:
		// Engine의 메모리 해제
		Engine::Get().CleanUp();
		return false;
	}

	return false;
}

Engine::Engine()
{
	instance = this;

	// 엔진 설정 로드
	LoadEngineSettings();

	// 이미지 버퍼 생성 / 콘솔에 보낼 버퍼 생성
	Vector2 screenSize(settings.width, settings.height);
	imageBuffer = new ImageBuffer((screenSize.x + 1) * screenSize.y + 1);

	// 이미지 버퍼 초기화
	ClearImageBuffer();

	// 두 개의 버퍼 생성
	renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), screenSize);
	renderTargets[1] = new ScreenBuffer(screenSize);

	// 버퍼 교환.
	Present();

	// cls 호출.
	system("cls");
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::OnConsoleInitialize()
{
	// 콘솔 커서 정보
	CONSOLE_CURSOR_INFO info;
	info.bVisible = FALSE; // 보기 끄기
	info.dwSize = 1; // 커서 사이즈 1

	// 콘솔 커서 끄기
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	// 콘솔 창 이벤트 등록
	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE);
}

void Engine::Run()
{
	LARGE_INTEGER currentTime; // 현재 시간
	LARGE_INTEGER previousTime; // 이전 시간
	QueryPerformanceCounter(&currentTime); // RDTSC
	previousTime = currentTime;

	// 하드웨어 시계의 정밀도(주파수) 가져오기.
	// 나중에 초로 변환하기 위해
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 타겟 프레임
	float targetFrameRate =
		(settings.framerate == 0.0f) ? 60.f : settings.framerate;

	// 타겟 한 프레임 시간.
	float oneFrameTime = 1.0f / targetFrameRate;

	// 게임 루프
	while (true)
	{
		// 엔진 종료 여부 확인.
		if (isQuit)
		{
			break;
		}

		// 프레임 시간 계산
		// (현재 시간 - 이전 시간) / 주파수 = 초 단위.
		QueryPerformanceCounter(&currentTime);

		// 프레임 시간.
		float deltaTime =
			(currentTime.QuadPart - previousTime.QuadPart)
			/ static_cast<float>(frequency.QuadPart);

		// 입력 처리 [키보드/마우스]
		input.ProcessInput();
		input.ProcessMouseInput();

		// 고정 프레임.
		if (deltaTime >= oneFrameTime)
		{
			// 엔진 이벤트 처리
			BeginPlay();
			Tick(deltaTime);
			Render();

			// 제목에 FPS 출력하기
			char title[50]{};
			sprintf_s(title, 50, "FPS: %f", (1.0f / deltaTime));
			SetConsoleTitleA(title);

			// 시간 업데이트
			previousTime = currentTime;

			// 현재 프레임의 키 입력을 기록
			input.SavePreviousKeyStates();

			if (mainLevel)
			{
				// 이전 프레임에 추가 및 삭제 요청된 액터 처리
				mainLevel->ProcessAddAndDestroyActors();
			}

			// 레벨 변경 요청이 었었으면 처리.
			if (levelChangeRequested && changeRequestedLevel)
			{
				AddLevel(changeRequestedLevel);
				changeRequestedLevel = nullptr;
				levelChangeRequested = false;
			}
		}
	}

	// 게임 루프 종료 후, 정리
	// 콘솔 텍스트를 흰색으로 변경
	Utils::SetConsoleTextColor(Color::White);
}

void Engine::WriteToBuffer(const Vector2& position, const char* image, Color color, int sortingOrder)
{
	// 문자열 길이.
	int length = static_cast<int>(strlen(image));

	for (int i = 0; i < length; ++i)
	{
		// 기록할 문자 위치
		int index = (position.y * (settings.width)) + position.x + i;

		// 현재 위치에 그려진 이미지의 정렬 순서가 더 높으면 반환
		if (imageBuffer->sortingOrderArray[index] > sortingOrder)
		{
			return;
		}

		// 버퍼에 문자/색상 기록.
		imageBuffer->charInfoArray[index].Char.AsciiChar = image[i];
		imageBuffer->charInfoArray[index].Attributes = (WORD)color;
		imageBuffer->sortingOrderArray[index] = sortingOrder;
	}
}

void Engine::AddLevel(Level* newLevel)
{
	// 기존에 있던 레벨 제거
	SafeDelete(mainLevel);

	mainLevel = newLevel;
}

void Engine::ChangeLevel(Level* newLevel)
{
	// 레벨 변경 요청 플래그 설정.
	levelChangeRequested = true;

	// 변경할 레벨 정보.
	changeRequestedLevel = newLevel;
}

void Engine::CleanUp()
{
	// 레벨 제거
	SafeDelete(mainLevel);

	// 문자 버퍼 삭제.
	SafeDelete(imageBuffer);

	// 렌더 타겟 삭제.
	SafeDelete(renderTargets[0]);
	SafeDelete(renderTargets[1]);
}

void Engine::Quit()
{
	// 종료 플래그 설정.
	isQuit = true;
}

Engine& Engine::Get()
{
	return *instance;
}

void Engine::BeginPlay()
{
	// 레벨 시작
	if (mainLevel)
	{
		mainLevel->BeginPlay();
	}
}

void Engine::Tick(float deltaTime)
{
	// 레벨 업데이트
	if (mainLevel)
	{
		mainLevel->Tick(deltaTime);
	}
}

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Render()
{
	// 화면 지우기.
	Clear();

	// 레벨 렌더링
	if (mainLevel)
	{
		mainLevel->Render();
	}

	// 백버퍼에 데이터 쓰기.
	GetRenderer()->Render(imageBuffer->charInfoArray);

	// 버퍼 교환.
	Present();
}

void Engine::Present()
{
	// 버퍼 교환.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);

	// 인덱스 뒤집기. 1->0, 0->1.
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

ScreenBuffer* Engine::GetRenderer() const
{
	return renderTargets[currentRenderTargetIndex];
}

void Engine::ClearImageBuffer()
{
	// 글자 버퍼 덮어쓰기.
	for (int y = 0; y < settings.height; ++y)
	{
		for (int x = 0; x < settings.width; ++x)
		{
			int index = (y * (settings.width)) + x;
			CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
			imageBuffer->sortingOrderArray[index] = -1;
		}

		// 각 줄 끝에 개행 문자 추가.
		int index = (y * (settings.width)) + settings.width;
		CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
		imageBuffer->sortingOrderArray[index] = -1;
	}

	// 마지막에 널 문자 추가.
	int index = (settings.width) * settings.height + 1;
	CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
	imageBuffer->sortingOrderArray[index] = -1;
}

void Engine::LoadEngineSettings()
{
	// 엔진 설정 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");

	if (!file)
	{
		std::cout << "Failed to load engine settings." << "\n";
		__debugbreak();
		return;
	}

	// 로드
	// FP(File Position) 포인터를 가장 뒤로 옮기기
	fseek(file, 0, SEEK_END);

	// 이 위치 구하기
	size_t fileSize = ftell(file);

	// 다시 첫 위치로 옮기기
	rewind(file);

	// 파일 내용을 저장할 버퍼 할당
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	// 파일 내용 읽기
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	// 파싱(Parcing, 구문 해석 -> 필요한 정보를 얻는 과정)
	char* context = nullptr;
	char* token = nullptr;

	token = strtok_s(buffer, "\n", &context);

	// 구문 분석
	while (token)
	{
		// 키/값 분리
		char header[10]{};

		// sscanf_s이 제대로 동작하려면 키와 값 사이의 빈칸이 있어야 함
		sscanf_s(token, "%s", header, 10);

		// 헤더 문자열 비교
		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}
		else if (strcmp(header, "depth") == 0)
		{
			sscanf_s(token, "depth = %d", &settings.depth);
		}

		// 그 다음줄 분리
		token = strtok_s(nullptr, "\n", &context);
	}

	// 버퍼 메모리 해제
	SafeDeleteArray(buffer);

	// 파일 닫기
	fclose(file);
}
