#include "Actor.h"

#include <iostream>
#include "Engine.h"
#include "Utils/Utils.h"
#include "Level/Level.h"

Actor::Actor(const char* image, Color color, const Vector2& position)
	: color(color), position(position)
{
	// iamge 초기화
	width = (int)strlen(image); // 문자열 길이
	this->image = new char[width + 1]; // 문자열 메모리 할당
	strcpy_s(this->image, width + 1, image); // 문자열 복사
}

Actor::~Actor()
{
	SafeDeleteArray(image); // 문자열 메모리 해제
}

void Actor::BeginPlay()
{
	hasBegonPlay = true;
}

void Actor::Tick(float deltaTime)
{
}

void Actor::Render()
{
	// Win32 API.
	// 커서위치 이동.
	//static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 출력 제어 핸들 얻어오기

	// 커서 이동.
	Utils::SetConsolePosition(position);

	// 색상 설정
	Utils::SetConsoleTextColor(color);

	printf("%s", image); // 그리기
}

bool Actor::TestIntersect(const Actor* const other)
{
	// AABB(Axis Aligned Bounding Box)
	// Note: 현재 엑터 구조상 세로는 크기가 없음.
	//		따라서 가로 위치만 고려

	int xMin = Position().x;
	int xMax = Position().x + width - 1;

	// 충돌 비교할 다른 액터의 x 좌표 정보
	int otherXMin = other->Position().x;
	int otherXMax = other->Position().x + other->width - 1;

	// 안 겹치는 조건
	if (otherXMin > xMax) // 다른 액터의 왼쪽 경계가 내 오른쪽 경계보다 더 오른쪽
	{
		return false;
	}

	if (otherXMax < xMin) // 다른 액터의 오른쪽 경계가 내 왼쪽 경계보다 더 왼쪽
	{
		return false;
	}

	// TODO: 기본값 변경

	return Position().y == other->Position().y;
}

void Actor::Destroy()
{
	if(isExpired == true)
	{
		return;
	}

	isExpired = true; // 삭제 요청 설정
	owner->DestroyActor(this);
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}

Vector2 Actor::Position() const
{
	return position;
}

void Actor::SetPosition(const Vector2& newPosition)
{
	if (position == newPosition)
	{
		return;
	}

	// 오른쪽 가장자리 혹은 맨 위로 벗어났는가
	if (newPosition.x < 0 || newPosition.y < 0)
	{
		return;
	}
	// 왼쪽 가장자리로 벗어났는가
	if (newPosition.x + width - 1 > Engine::Get().Width())
	{
		return;
	}
	// 아래 범위를 벗어났는가
	if (newPosition.y + 1 > Engine::Get().Height())
	{
		return;
	}

	Vector2 direction = newPosition - position; // 지울 위치 확인

	position.x = (direction.x >= 0) ? position.x : position.x + width - 1;
	
	Utils::SetConsolePosition(position); // 커서 이동.

	printf(" "); // 지우기

	position = newPosition;
}

unsigned int Actor::GetSortingOrder() const
{
	return sortingOrder;
}

void Actor::SetSortingOrder(unsigned int sortingOrder)
{
	this->sortingOrder = sortingOrder;
}

Level* Actor::GetOwner() const
{
	return owner;
}

void Actor::SetOwner(Level* owner)
{
	this->owner = owner;
}


