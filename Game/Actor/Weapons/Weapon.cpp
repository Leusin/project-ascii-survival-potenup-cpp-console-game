#include "Weapon.h"

#include "Engine.h"

Weapon::Weapon(Vector2& playerPosition)
	: Actor("-", Color::Red)
	, playerPosition(playerPosition)
	, worldPosition(playerPosition)
{
	// 무기의 초기 화면 위치는 플레이어와 동일하게 화면 중앙입니다.
	SetPosition({ Engine::Get().Width() / 2, Engine::Get().Height() / 2 }); 
	SetSortingOrder(5);
}

void Weapon::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 무기의 월드 위치 업데이트
	worldPosition = worldPosition + direction.Normalize() * speed * deltaTime;

	// 화면 좌표계로 변환
	//screenPosition.x = (worldPosition.x - playerPosition.x) + Engine::Get().Width() / 2;
	//screenPosition.y = -(worldPosition.y - playerPosition.y) + Engine::Get().Height() / 2;
	Vector2 screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, playerPosition);

	SetPosition(screenPosition);
}

void Weapon::Render()
{
	//super::Render();

	/// 대각선 방향을 먼저 판별합니다.
	if (direction.y < -0.5f && direction.x > -0.5f && direction.x < 0.5f)
	{
		Engine::Get().WriteToBuffer(Position(), "|", Color::Red, GetSortingOrder());
	}
	// 우측 상단
	else if (direction.x > 0 && direction.y < 0)
	{
		Engine::Get().WriteToBuffer(Position(), "\\", Color::Red, GetSortingOrder());
	}
	// 좌측 상단
	else if (direction.x < 0 && direction.y < 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		Engine::Get().WriteToBuffer(renderPosition, "/", Color::Red, GetSortingOrder());
	}
	// 우측 하단
	else if (direction.x > 0 && direction.y > 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		Engine::Get().WriteToBuffer(renderPosition, "/", Color::Red, GetSortingOrder());
	}
	// 좌측 하단
	else if (direction.x < 0 && direction.y > 0)
	{
		Engine::Get().WriteToBuffer(Position(), "\\", Color::Red, GetSortingOrder());
	}

	// 상하좌우 방향을 나중에 판별합니다

	// 우측 방향
	else if (direction.x > 0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		Engine::Get().WriteToBuffer(Position(), "-", Color::Red, GetSortingOrder());
	}
	// 좌측 방향
	else if (direction.x < -0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		Engine::Get().WriteToBuffer(Position(), "-", Color::Red, GetSortingOrder());
	}
	// 위쪽 방향 (y축이 위로 갈수록 커지는 직교 좌표계 기준)
	else if (direction.y > 0.5f && direction.x > -0.5f && direction.x < 0.5f) 
	{
		Engine::Get().WriteToBuffer(Position(), "|", Color::Red, GetSortingOrder());
	}
	// 아래쪽 방향 (y축이 아래로 갈수록 커지는 좌표계 기준)
	
}
