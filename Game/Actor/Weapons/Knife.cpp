#include "Knife.h"

#include "Level/GameLevel.h"
#include "Actor/Projectile/KnifeProjectile.h"

Knife::Knife(const Vector2I& cameraPosition, Vector2I& direiction)
	: Weapon("-", "Knife", cameraPosition)
	, direction(direiction)
{
	color = Color::LightWhite;

	stats.currentLevel = 0;
	stats.baseDamaged = 6.5f;
	stats.speed = 18.0f;
	stats.cooldown = 1.f;
	stats.amount = 1;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;

	upgradeDescription =
	{
		"Fires quickly in the faced direction",
		"damage up by 2 / speed up by 2 / amount up by 2",
		"speed up by 2 / colldown down by 0.4 / amount 5",
		"amount up by 6 / speed up by 2 / interval down by 0.2"
	};
}

void Knife::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (stats.currentLevel < 1)
	{
		return;
	}

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

	// Fires quickly in the faced direction
	if (stats.currentLevel == 2)
	{
		stats.baseDamaged = 2.5f;
		stats.speed = 14.0f;
		stats.cooldown = 1.4f;
		stats.amount = 1;
		stats.projectileInterval = 0.12f;
	}
	// damage up by 2 / speed up by 2 / amount up by 2
	else if (stats.currentLevel == 2)
	{
		stats.baseDamaged = 4.5f;
		stats.speed = 16.0f;
		stats.amount = 2; // * 2 = 4
	}
	// speed up by 2 / colldown down by 0.4 / amount 5
	else if (stats.currentLevel == 3)
	{
		stats.speed = 18.0f;
		stats.cooldown = 1.0f;
		stats.amount = 3; // * 3 = 9
	}
	// speed up by 2 / amount up by 6 / interval down by 0.2
	else if (stats.currentLevel == 4)
	{
		stats.speed = 20.0f;
		stats.amount = 5; // * 3 = 15
		stats.projectileInterval = 0.1f;
	}
}

void Knife::Fire()
{
	// 오프셋 값 (각도를 벌리는 정도를 조절)
	float offsetMagnitude = 0.2f;

	if (stats.currentLevel >= 3)
	{
		// 왼쪽으로 약간 회전
		Vector2I sideOffset = { static_cast<int>(-direction.y), static_cast<int>(direction.x) };
		GetOwner()->AddActor(new KnifeProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction, sideOffset));
	}

	if (stats.currentLevel >= 2)
	{
		// 오른쪽으로 약간 회전
		Vector2I sideOffset = { static_cast<int>(direction.y), static_cast<int>(-direction.x) };
		GetOwner()->AddActor(new KnifeProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction, sideOffset));
	}

	GetOwner()->AddActor(new KnifeProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction));
}
