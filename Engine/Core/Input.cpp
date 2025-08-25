#include "Input.h"
#include "Utils/Utils.h"
#include "Engine.h"

#include <cstring>
#include <Windows.h>

// static 변수 정의
Input* Input::instance = nullptr;

Input::Input()
{
	// 싱글톤 실행을 위해 instance 변수 설정
	instance = this;
}

Input& Input::Get()
{
	return *instance;
}

bool Input::GetKey(int keyCode)
{
	// 키를 누르고 있을 때
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	// 이전에 누르지 않고 있다가, 처음 키를 누를 떄
	return keyStates[keyCode].isKeyDown && !keyStates[keyCode].previousKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	// 키를 누르고 있다가 키를 땟을 때
	return !keyStates[keyCode].isKeyDown && keyStates[keyCode].previousKeyDown;
}

Vector2 Input::GetMousePosition() const
{
	return mousePosition;
}

void Input::ProcessInput()
{
	// 키 입력 확인
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].isKeyDown = GetAsyncKeyState(i) & 0x8000;
	}
}

void Input::ProcessMouseInput()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// 콘솔 입력 모드 초기화
	static bool initialized = false;
	static DWORD prevMode = 0;
	if (!initialized)
	{
		if (!GetConsoleMode(hIn, &prevMode))
		{
			return;
		}

		DWORD newMode = prevMode;
		newMode |= ENABLE_EXTENDED_FLAGS;    // 먼저 확장 플래그 켜기
		newMode &= ~ENABLE_QUICK_EDIT_MODE;  // QuickEdit 끔 (마우스 이벤트 가로채는 걸 방지)
		newMode |= ENABLE_MOUSE_INPUT;       // 마우스 입력 켬

		SetConsoleMode(hIn, newMode);
		initialized = true;
	}

	DWORD numEvents = 0;
	if (!GetNumberOfConsoleInputEvents(hIn, &numEvents) || numEvents == 0)
	{
		return;
	}

	INPUT_RECORD rec;
	DWORD read = 0;
	while (numEvents-- > 0)
	{
		if (!ReadConsoleInput(hIn, &rec, 1, &read))
		{
			break; // 읽기 실패 시 탈출
		}

		if (rec.EventType != MOUSE_EVENT) continue;

		const MOUSE_EVENT_RECORD& mer = rec.Event.MouseEvent;

		// dwEventFlags == 0 : 버튼 상태 변화 (press/release)
		// dwEventFlags == MOUSE_MOVED : 마우스 이동(무시해도 됨)
		if (mer.dwEventFlags == 0)
		{
			// 왼쪽 버튼 눌림 검사
			if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				MouseStates[0] = true;

				COORD pos = mer.dwMousePosition;
				mousePosition.x = static_cast<int>(pos.X);
				mousePosition.y = static_cast<int>(pos.Y);
			}
			// 오른쪽 버튼 눌림 검사
			else if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
			{
				MouseStates[1] = true;

				COORD pos = mer.dwMousePosition;
				mousePosition.x = static_cast<int>(pos.X);
				mousePosition.y = static_cast<int>(pos.Y);
			}
		}
	}
}

void Input::SavePreviousKeyStates()
{
	// 현재 프레임의 키 입력을 기록
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previousKeyDown = keyStates[i].isKeyDown;
	}

	// 현재 프레임의 마우스 입력 기록
	MouseStates[0] = false;
	MouseStates[1] = false;
	mousePosition = Vector2(-1, -1);
}