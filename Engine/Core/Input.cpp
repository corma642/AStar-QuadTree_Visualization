#include "Input.h"
#include "Utils/Utils.h"
#include "Engine.h"

#include <cstring>
#include <Windows.h>

// static ���� ����
Input* Input::instance = nullptr;

Input::Input()
{
	// �̱��� ������ ���� instance ���� ����
	instance = this;
}

Input& Input::Get()
{
	return *instance;
}

bool Input::GetKey(int keyCode)
{
	// Ű�� ������ ���� ��
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	// ������ ������ �ʰ� �ִٰ�, ó�� Ű�� ���� ��
	return keyStates[keyCode].isKeyDown && !keyStates[keyCode].previousKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	// Ű�� ������ �ִٰ� Ű�� ���� ��
	return !keyStates[keyCode].isKeyDown && keyStates[keyCode].previousKeyDown;
}

Vector2 Input::GetMousePosition() const
{
	return mousePosition;
}

void Input::ProcessInput()
{
	// Ű �Է� Ȯ��
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

	// �ܼ� �Է� ��� �ʱ�ȭ
	static bool initialized = false;
	static DWORD prevMode = 0;
	if (!initialized)
	{
		if (!GetConsoleMode(hIn, &prevMode))
		{
			return;
		}

		DWORD newMode = prevMode;
		newMode |= ENABLE_EXTENDED_FLAGS;    // ���� Ȯ�� �÷��� �ѱ�
		newMode &= ~ENABLE_QUICK_EDIT_MODE;  // QuickEdit �� (���콺 �̺�Ʈ ����ä�� �� ����)
		newMode |= ENABLE_MOUSE_INPUT;       // ���콺 �Է� ��

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
			break; // �б� ���� �� Ż��
		}

		if (rec.EventType != MOUSE_EVENT) continue;

		const MOUSE_EVENT_RECORD& mer = rec.Event.MouseEvent;

		// dwEventFlags == 0 : ��ư ���� ��ȭ (press/release)
		// dwEventFlags == MOUSE_MOVED : ���콺 �̵�(�����ص� ��)
		if (mer.dwEventFlags == 0)
		{
			// ���� ��ư ���� �˻�
			if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				MouseStates[0] = true;

				COORD pos = mer.dwMousePosition;
				mousePosition.x = static_cast<int>(pos.X);
				mousePosition.y = static_cast<int>(pos.Y);
			}
			// ������ ��ư ���� �˻�
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
	// ���� �������� Ű �Է��� ���
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previousKeyDown = keyStates[i].isKeyDown;
	}

	// ���� �������� ���콺 �Է� ���
	MouseStates[0] = false;
	MouseStates[1] = false;
	mousePosition = Vector2(-1, -1);
}