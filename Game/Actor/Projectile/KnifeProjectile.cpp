#include "KnifeProjectile.h"

#include "Engine.h"

KnifeProjectile::KnifeProjectile(float damage, float speed, Vector2& playerPosition, Vector2& direiction)
	: Actor("-", Color::White, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, speed(speed)
	, damage(damage)
	, direction(direiction)
	, playerPosition(playerPosition)
	, projectilePosition(playerPosition)
{
	SetSortingOrder(5);

	// 방향에 따라 다른 아스키 표시
	SetImafeByDirection();
}

void KnifeProjectile::BeginPlay()
{
	super::BeginPlay();
}

void KnifeProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 무기의 월드 위치 업데이트
	projectilePosition = projectilePosition + direction.Normalize() * speed * deltaTime;

	// 화면 좌표계로 변환
	Vector2 screenPosition = Engine::Get().OrthogonalToScreenCoords(projectilePosition, playerPosition);
	SetPosition(screenPosition);
}

void KnifeProjectile::Render()
{
	// NOTE
	// "/" 를 그리는 방향에서는 왜인지 x 축으로 -1 한 칸 밀려서 스폰된다.
	// 그래서 그거 보정
	if (strcmp(GetImage(), "/") == 0)
	{
		// 렌더링에만 사용할 임시 위치 변수를 만듭니다.
		Vector2 renderPosition = Position();
		renderPosition.x += 1;

		// SetPosition()을 사용하지 않고, 바로 Draw 함수를 호출합니다.
		Engine::Get().WriteToBuffer(renderPosition, GetImage(), GetColor(), GetSortingOrder());

		return;
	}

	super::Render();
}

void KnifeProjectile::SetImafeByDirection()
{
	// 우측 상단
	if (direction.x > 0 && direction.y < 0)
	{
		SetImage("\\");
	}
	// 좌측 상단
	else if (direction.x < 0 && direction.y < 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		SetPosition(renderPosition);
		SetImage("/");
	}
	// 우측 하단
	else if (direction.x > 0 && direction.y > 0)
	{
		Vector2 renderPosition = Position();
		renderPosition.x += 1;
		SetPosition(renderPosition);
		SetImage("/");
	}
	// 좌측 하단
	else if (direction.x < 0 && direction.y > 0)
	{
		SetImage("\\");
	}

	/// 상하좌우 방향을 나중에 판별

	// 우측 방향
	else if (direction.x > 0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		SetImage("-");
	}
	// 좌측 방향
	else if (direction.x < -0.5f && direction.y > -0.5f && direction.y < 0.5f)
	{
		SetImage("-");
	}
	// 위쪽 방향 (y축이 위로 갈수록 커지는 직교 좌표계 기준)
	else if (direction.y > 0.5f && direction.x > -0.5f && direction.x < 0.5f)
	{
		SetImage("|");
	}
	// 아래쪽 방향 (y축이 아래로 갈수록 커지는 좌표계 기준)
	else if (direction.y < -0.5f && direction.x > -0.5f && direction.x < 0.5f)
	{
		SetImage("|");
	}
}
