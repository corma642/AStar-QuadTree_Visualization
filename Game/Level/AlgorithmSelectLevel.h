#include "Level/Level.h"
#include "Actor/Actor.h"

#include <vector>

struct MenuItem
{
	// �Լ� ������ ����
	typedef void(*OnSelected)();

	MenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// �޴� �ؽ�Ʈ ���ڿ� ����
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~MenuItem()
	{
		SafeDeleteArray(menuText);
	}

	// �޴� �ؽ�Ʈ
	char* menuText = nullptr;

	// �޴� ���� �� ������ ����
	OnSelected onSelected = nullptr;
};

class AlgorithmSelectLevel : public Level
{
	RTTI_DECLARATIONS(AlgorithmSelectLevel, Level)

public:
	AlgorithmSelectLevel();
	~AlgorithmSelectLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void PrintBackGround();

private:
	// ���� ���õ� �������� �ε���
	int currentIndex = 0;

	// ������ ���� ����
	Color selectedColor = Color::LightGreen;

	// ������ ���� �ȵ��� �� ����
	Color unSelectedColor = Color::LightWhite;

	// ������ �迭
	std::vector<MenuItem*> items;

	// �޴� ������ ��
	int length = 0;
};