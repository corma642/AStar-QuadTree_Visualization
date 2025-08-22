#include "Input.h"

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

void Input::ProcessInput()
{
	// 키 입력 확인
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].isKeyDown = GetAsyncKeyState(i) & 0x8000;
	}
}

void Input::SavePreviousKeyStates()
{
	// 현재 프레임의 키 입력을 기록
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previousKeyDown = keyStates[i].isKeyDown;
	}
}