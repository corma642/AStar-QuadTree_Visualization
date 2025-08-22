#pragma once

#include "Core.h"
#include "Math\Vector2.h"
#include "Math/Color.h"
#include "RTTI.h"

// 화면에 그려질 정렬 순서 열거형 (높을수록 위)
enum class SortingOrder : unsigned __int8
{
	None = 0,
	Ground,
	Path,
	Destination,
	Player,
};

// 레벨 전방 선언
class Level;

class ENGINE_API Actor : public RTTI
{
	// 레벨에서는 액터에 접근할 수 있도록 friend 선언
	friend class Level;

	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero
	);

	virtual ~Actor();

	// 이벤트 함수
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	// BeginPlay 호출 여부 확인
	inline bool HasBeganPlay() const { return hasBeganPlay; }

	// 위치 설정 및 값 읽는 함수
	void SetPosition(const Vector2& newPosition);
	FORCEINLINE Vector2 Position() const { return position; }

	// 정렬 순서 Getter/Setter
	FORCEINLINE SortingOrder GetSortingOrder() const { return sortingOrder; ; }
	FORCEINLINE void SetSortingOrder(SortingOrder sortingOrder)
	{
		this->sortingOrder = sortingOrder;
	}

	// 오너십 Getter/Setter
	FORCEINLINE Level* GetOwner() const { return owner; }
	FORCEINLINE void SetOwner(Level* newOwner) { owner = newOwner; }

	// 액터 색상 변경 함수
	FORCEINLINE void SetColor(const Color newColor) { color = newColor; }

	// 객체 삭제 함수
	void Destroy();

	// 액터의 이미지 값 변경 함수
	void ChangeImage(const char* newImage);

	// 객체가 삭제되기 직전에 호출되는 함수
	virtual void OnDestroy();

	// 생명 주기 설정 함수
	void SetLifetime(float newLifeTime);

	// 게임 종료 요청 함수
	void QuitGame();

protected:
	// 개체의 위치
	Vector2 position;

	// 그릴 값
	char* image = nullptr;

	// 문자열 길이
	int width = 0;

	// 텍스트 색상 값
	Color color;

	// BeginPlay 호출이 되었는지 확인
	bool hasBeganPlay = false;

	// 정렬 순서
	SortingOrder sortingOrder = SortingOrder::None;

	// 액터가 삭제 요청되었는지 확인하는 변수
	bool isExpired = false;

	// 수명 주기 (단위: 초)
	float lifeTime = 0.0f;

	// 이 플래그를 true로 설정하면 수명 주기를 사용해 자동 제거
	bool autoDestroy = false;

	// 소유 레벨(오너십)
	Level* owner = nullptr;
};

