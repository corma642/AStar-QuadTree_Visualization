#pragma once

#include "Core.h"
#include "Math/Vector2.h"

class ENGINE_API Input
{
	// friend ����
	friend class Engine;

	// Ű �Է� ���� ����ü
	struct KeyState
	{
		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool isKeyDown = false;

		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool previousKeyDown = false;
	};

public:
	Input();

	// Ű �Է� Ȯ�� �Լ�
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	// ���콺 �Է� Ȯ�� �Լ�
	Vector2 GetMousePosition() const;
	bool GetMouseLeftButton() const { return MouseStates[0]; }
	bool GetMouseRightButton() const { return MouseStates[1]; }

	static Input& Get();

private:
	void ProcessInput();
	void ProcessMouseInput();
	void SavePreviousKeyStates();

private:
	static Input* instance;

	// Ű �Է� ���� ���� ����
	KeyState keyStates[255]{ };

	// ���콺 �Է� ���� ���� ����
	// Left		= 0
	// Right	= 1
	bool MouseStates[2]{};

	// ���콺 ��ġ
	Vector2 mousePosition = Vector2(-1, -1);
};