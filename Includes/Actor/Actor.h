#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

/// <summary>
/// "물체가 뭘해야할까?" 를 정의한다
/// - 위치 점령
/// - 콘솔 창에 그리기(어떻케)
/// - 엔진의 이벤트 함수 호출: BeginPlay / Tick / Draw
/// </summary>
class Engine_API Actor : public RTTI
{
	RTTI_DECLARATIONS(Actor, RTTI)

public:
	Actor(const char* image = "", Color color = Color::White, const Vector2& position = Vector2::Zero);
	virtual ~Actor();

	virtual void BeginPlay(); // 단 한번만 호출. (초기화가 필요한 작업)
	virtual void Tick(float deltaTime); // 매 프레임 호출. (반복성 작업/지속성이 필요한 작업)
	virtual void Render(); // 그리기

	bool TestIntersect(const Actor* const other);

	void Destroy(); // 삭제 요청
	void QuitGame();

	Vector2 Position() const;
	void SetPosition(const Vector2& newPosition);

	unsigned int GetSortingOrder() const;
	void SetSortingOrder(unsigned int sortingOrder);

	class Level* GetOwner() const;
	void SetOwner(class Level* owner);

	inline int Width() const { return width; };
	inline bool HasBegonPlay() const { return hasBegonPlay; }
	inline bool IsActive() const { return isActive; };
	inline bool IsExpired() const { return isExpired; };

private:
	Vector2 position; // 개체의 위치

	char* image = nullptr;

	int width = 0; // 문자열 길이

	Color color = Color::White;// = Color::Blue | Color::Green | Color::Red;

	bool hasBegonPlay = false; // 호출이 되었는지 확인

	unsigned int sortingOrder = 0; // 정렬 순서

	bool isActive = true;// 액터가 활성 상태인지

	bool isExpired = false;	// 삭제 요청됐는지 

	class Level* owner = nullptr; // 소유 레벨
};
