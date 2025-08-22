#pragma once

#include "Core.h"

class ENGINE_API Input
{
	// friend 선언
	friend class Engine;

	// 키 입력 관리 구조체
	struct KeyState
	{
		// 현재 프레임에 키가 눌렸는지 여부
		bool isKeyDown = false;

		// 이전 프레임에 키가 눌렸는지 여부
		bool previousKeyDown = false;
	};

public:
	Input();

	// 키 입력 확인 함수
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static Input& Get();

private:
	void ProcessInput();
	void SavePreviousKeyStates();

private:
	static Input* instance;

	// 키 입력 정보 관리 변수
	KeyState keyStates[255]{ };
};