#include "MagicWandProjectile.h"

#include "Engine.h"
#include "level/GameLevel.h"
#include "Actor/Enemy/Enemy.h"

WagicWandProjectile::WagicWandProjectile(float damage, float speed, const Vector2I& cameraPosition, const Vector2F direiction)
	: Actor("o", Color::LightWhite, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, speed(speed)
	, damage(damage)
	, direction(direiction)
	, cameraPosition(cameraPosition)
	, projectilePosition(cameraPosition)
{
	SetSortingOrder(5);
}

void WagicWandProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 다음 위치 계산
	Vector2F movement = direction.Normalize() * speed * deltaTime; // 이동할 크기
	Vector2F nextPosition = projectilePosition + movement; // 이동될 위치
	Vector2I nextScreenPos = Engine::Get().OrthogonalToScreenCoords(nextPosition, cameraPosition); // 다음에 이동할 화면 위치

	// 계산위치 실제로 적용
	projectilePosition = nextPosition;

	// 화면 좌표계로 변환
	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(projectilePosition, cameraPosition);

	// 화면 좌표계 가장자리에 있을 경우 제거
	if (screenPosition.x < -1 || screenPosition.x > Engine::Get().Width() - 1 ||
		screenPosition.y < -1 || screenPosition.y > Engine::Get().Height() - 1)
	{
		Destroy();
	}

	// 화면 좌표계에 실제로 적용
	SetPosition(screenPosition);

	//
	// 현재 위치에 적이 있는지 검사 
	// 
	std::vector<Actor*> actors = GetOwner()->GetActors();

	for (Actor* actor : actors)
	{
		if (!actor->As<Enemy>())
		{
			continue;
		}

		if (actor->Position() != screenPosition)
		{
			continue;
		}

		// TODO: 데미지 처리
		IDamageable* enemyDamageable = dynamic_cast<IDamageable*> (actor);
		if (enemyDamageable)
		{
			enemyDamageable->TakeDamage(damage);
			Destroy();
		}
	}
}

void WagicWandProjectile::Render()
{
	super::Render();
}
