#include "MagicWand.h"

#include "Level/Level.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Projectile/MagicWandProjectile.h"

MagicWand::MagicWand(const Vector2I& cameraPosition)
	: Weapon("o", "Magic Wand", cameraPosition)
{
	stats.currentLevel = 0;
	stats.baseDamaged = 0.f;
	stats.speed = 0.f;
	stats.cooldown = 0.f;
	stats.amount = 0;
	stats.projectileInterval = 0.0f;

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;

	upgradeDescription =
	{
		"Fires at the nearest enemy",
		"speed up by 2 / cooldown down by 0.4 / amount up by 2",
		"damage up by 1.5 / amount up by 3 / interval down by 0.2",
		"speed up by 4 / cooldown down by 0.4 / amount up by 10"
	};
}

void MagicWand::Tick(float deltaTime)
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
		projectilesToFire = stats.amount;
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

void MagicWand::Render()
{
}

void MagicWand::LevelUp()
{
	super::LevelUp();

	// Fires at the nearest enemy
	if (stats.currentLevel == 1)
	{
		stats.baseDamaged = 5.0f;
		stats.speed = 10.0f;
		stats.cooldown = 2.4f;
		stats.amount = 1;
		stats.projectileInterval = 0.5f;
	}
	// speed up by 2 / cooldown down by 0.4 / amount up by 2
	else if (stats.currentLevel == 2)
	{
		stats.speed = 12.0f;
		stats.cooldown = 2.0f;
		stats.amount = 3;
	}
	// damage up by 1.5 / amount up by 3 / interval down by 0.2
	else if (stats.currentLevel == 3)
	{
		stats.baseDamaged = 6.4f;
		stats.amount = 6;
		stats.projectileInterval = 0.3f;
	}
	//  speed up by 4 / cooldown down by 0.4 / amount up by 10
	else if (stats.currentLevel == 4)
	{
		stats.speed = 16.0f;
		stats.cooldown = 1.6f;
		stats.amount = 10;
	}

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;
}

void MagicWand::Fire()
{
	//
	// 가장 가까운 적을 찾음
	//

	Actor* nearestEnemy = nullptr;
	float nearestDistanceSq = FLT_MAX; // 가장 가까운 거리를 추적할 변수

	std::vector<Actor*> actors = GetOwner()->GetActors();
	for (Actor* actor : actors)
	{
		if (!actor->As<Enemy>())
		{
			continue;
		}

		Vector2F toEnemy = actor->Position() - Position();
		float distSq = toEnemy.SqrMagnitude();

		// 현재 적이 기존에 찾은 적보다 더 가깝다면 갱신
		if (distSq < nearestDistanceSq)
		{
			nearestEnemy = actor;
			nearestDistanceSq = distSq;
		}

	}

	// 기본 값.
	Vector2I direction = Vector2I::Right;

	if (nearestEnemy)
	{
		// 적을 향하는 방향 벡터 계산
		// 여기서 정규화할 필요 없음: 투사체 내부에서 정규화 하기 때문에 
		direction = nearestEnemy->Position() - Position();
		direction.y *= -1;
	}

	// 발사
	GetOwner()->AddActor(new WagicWandProjectile(stats.baseDamaged, stats.speed, GetCameraPosition(), direction));
}
