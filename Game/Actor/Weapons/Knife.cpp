#include "Knife.h"

#include "Engine.h"
#include "Level/GameLevel.h"
#include "Actor/Projectile/KnifeProjectile.h"

Knife::Knife(const Vector2I& cameraPosition, Vector2F& direiction)
	: Weapon(cameraPosition)
	, direction(direiction)
{
	icon = '-';
	SetImage("Knife");

	stats.currentLevel = 1;
	stats.baseDamaged = 6.5f;
	stats.speed = 18.0f;
	stats.cooldown = 1.f;
	stats.amount = 1;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;
}

void Knife::Tick(float deltaTime)
{
	// 발사 중이 아닐 때만 쿨다운 타이머를 틱
	if (!isFiring)
	{
		cooldownTimer.Tick(deltaTime);
	}

	// 발사 시작
	if (cooldownTimer.IsTimeout() && !isFiring)
	{
		isFiring = true;
		projectilesToFire  = stats.amount;
		cooldownTimer.Reset();
		cooldownTimer.SetTargetTime(stats.cooldown);
	}

	// 발사 중일 때
	if (isFiring)
	{
		fireIntervalTimer.Tick(deltaTime);

		if (fireIntervalTimer.IsTimeout())
		{
			Fire();
			projectilesToFire--;

			if (projectilesToFire <= 0)
			{
				isFiring = false; // 발사 종료
			}
			else
			{
				fireIntervalTimer.Reset(); // 다음 발사를 위해 간격 타이머 재설정
				fireIntervalTimer.SetTargetTime(stats.projectileInterval);
			}
		}
	}
}

void Knife::Render()
{
}

void Knife::LevelUp()
{
	super::LevelUp();

	if (stats.currentLevel == 1)
	{
		stats.baseDamaged = 6.5f;
		stats.speed = 18.0f;
		stats.cooldown = 1.f;
		stats.amount = 1;
		stats.projectileInterval = 0.1f;
	}
	else if (stats.currentLevel == 2)
	{
		stats.baseDamaged = 6.5f;
		stats.speed = 18.0f;
		stats.cooldown = 0.8f;
		stats.amount = 3;
		stats.projectileInterval = 0.09f;
	}
	else if (stats.currentLevel == 3)
	{
		stats.baseDamaged = 6.5f;
		stats.speed = 18.0f;
		stats.cooldown = 0.8f;
		stats.amount = 6;
		stats.projectileInterval = 0.09f;
	}
	else if (stats.currentLevel == 4)
	{
		stats.baseDamaged = 10.f;
		stats.speed = 20.0f;
		stats.cooldown = 0.6f;
		stats.amount = 10;
		stats.projectileInterval = 0.06f;
	}
}

void Knife::Fire()
{
	GetOwner()->AddActor(new KnifeProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction));
}
