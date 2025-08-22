#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char* image, Color color, const Vector2& position)
	: color(color), position(position)
{
	width = static_cast<int>(strlen(image));

	this->image = new char[width + 1];

	strcpy_s(this->image, width + 1, image);
}

Actor::~Actor()
{
	SafeDeleteArray(image);
}

void Actor::BeginPlay()
{
	hasBeganPlay = true;
}

void Actor::Tick(float deltaTiem)
{
}

void Actor::Render()
{
	Engine::Get().WriteToBuffer(position, image, color, static_cast<int>(sortingOrder));
}

void Actor::SetPosition(const Vector2& newPosition)
{
	// 같은 위치면 업데이트 안함
	if (position == newPosition)
	{
		return;
	}

	// 위치 갱신
	position = newPosition;
}

void Actor::Destroy()
{
	// 증복 삭제 방지
	if (isExpired)
	{
		return;
	}

	// 삭제 요청되었다고 설정
	isExpired = true;
	owner->DestroyActor(this);

	// 삭제 요청 알림
	OnDestroy();
}

void Actor::ChangeImage(const char* newImage)
{
	// 기존 이미지 메모리 해제.
	SafeDelete(image);

	// 새로운 문자열 복사.
	size_t length = strlen(newImage) + 1;
	image = new char[length];
	strcpy_s(image, length, newImage);
}

void Actor::OnDestroy()
{
}

void Actor::SetLifetime(float newLifeTime)
{
	// 입력값 확인
	if (newLifeTime <= 0.0f)
	{
		return;
	}

	// 수명 주기 설정
	lifeTime = newLifeTime;

	// 자동 제거 옵션 활성화
	autoDestroy = true;
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}
