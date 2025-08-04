#include "Knife.h"

#include "Engine.h"

Knife::Knife(Vector2& playerPosition, Vector2& direiction)
	: Weapon(playerPosition)
	, direction(direiction)
{
	stats.baseDamaged = 6.5f;
	stats.speed = 18.0f;
}

void Knife::BeginPlay()
{
}

void Knife::Tick(float deltaTime)
{
	// 무기의 월드 위치 업데이트
	worldPosition = worldPosition + direction.Normalize() * stats.speed * deltaTime;

	// 화면 좌표계로 변환
	Vector2 screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, playerPosition);
	SetPosition(screenPosition);
}

void Knife::Render()
{
	//
	// 대각선 방향을 먼저 판별합니다.
	//

	// 우측 상단
	if (direction.x > 0 && direction.y < 0)
	{
		//Engine::Get().WriteToBuffer(Position(), "\\", Color::Red, GetSortingOrder());
		SetImage("\\");
	}
	// 좌측 상단
	else if (direction.x < 0 && direction.y < 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		SetPosition(renderPosition);
		//Engine::Get().WriteToBuffer(renderPosition, "/", Color::Red, GetSortingOrder());
		SetImage("/");
	}
	// 우측 하단
	else if (direction.x > 0 && direction.y > 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		SetPosition(renderPosition);
		//Engine::Get().WriteToBuffer(renderPosition, "/", Color::Red, GetSortingOrder());
		SetImage("/");
	}
	// 좌측 하단
	else if (direction.x < 0 && direction.y > 0)
	{
		//Engine::Get().WriteToBuffer(Position(), "\\", Color::Red, GetSortingOrder());
		SetImage("\\");
	}

	//
	// 상하좌우 방향을 나중에 판별합니다
	//

	// 우측 방향
	else if (direction.x > 0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		//Engine::Get().WriteToBuffer(Position(), "-", Color::Red, GetSortingOrder());
		SetImage("-");
	}
	// 좌측 방향
	else if (direction.x < -0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		//Engine::Get().WriteToBuffer(Position(), "-", Color::Red, GetSortingOrder());
		SetImage("-");
	}
	// 위쪽 방향 (y축이 위로 갈수록 커지는 직교 좌표계 기준)
	else if (direction.y > 0.5f && direction.x > -0.5f && direction.x < 0.5f)
	{
		//Engine::Get().WriteToBuffer(Position(), "|", Color::Red, GetSortingOrder());
		SetImage("|");
	}
	// 아래쪽 방향 (y축이 아래로 갈수록 커지는 좌표계 기준)
	else if (direction.y < -0.5f && direction.x > -0.5f && direction.x < 0.5f)
	{
		//Engine::Get().WriteToBuffer(Position(), "|", Color::Red, GetSortingOrder());
		SetImage("|");
	}

	super::Render();
}
