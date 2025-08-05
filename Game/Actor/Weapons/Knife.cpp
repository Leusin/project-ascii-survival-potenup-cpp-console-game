#include "Knife.h"

#include "Engine.h"
#include "Level/GameLevel.h"
#include "Actor/Projectile/KnifeProjectile.h"

Knife::Knife(const Vector2I& cameraPosition, Vector2F& direiction)
	: Weapon(cameraPosition)
	, direction(direiction)
{
	stats.baseDamaged = 6.5f;
	stats.speed = 18.0f;
	stats.cooldown = 1.f;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);
}

void Knife::BeginPlay()
{
}

void Knife::Tick(float deltaTime)
{
	cooldownTimer.Tick(deltaTime);

	if (cooldownTimer.IsTimeout())
	{
		// TODO: 레벨에 따라 칼을 다르게 던지도록 해야 한다.
		Fire();

		// 타이머 정리
		cooldownTimer.Reset();
		//cooldownTimer.SetTargetTime(stats.cooldown);
	}
}

void Knife::Fire()
{
	GetOwner()->AddActor(new KnifeProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction));
}
