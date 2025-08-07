#include "Whip.h"

Whip::Whip(const Vector2I& cameraPosition, Vector2I& direiction)
	: Weapon("$", "Whip", cameraPosition)
	, direction(direiction)
{
	stats.currentLevel = 0;
	stats.baseDamaged = 10.f;
	stats.area = 100;
	stats.cooldown = 1.35f;
	stats.amount = 1;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;
}

void Whip::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 0 레벨이라면 활성하지 않음
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

void Whip::Render()
{
	// 부모에서 랜더한는 것을 의도적으로 막음
}

void Whip::LevelUp()
{
	super::LevelUp();
	if (stats.currentLevel == 1)
	{
		stats.currentLevel = 0;
		stats.baseDamaged = 10.f;
		stats.cooldown = 1.35f;
		stats.amount = 1;
		stats.projectileInterval = 0.1f;
	}
	else if (stats.currentLevel == 2)
	{
		// 투사체를 1 더, 공격력 5 증가
		stats.amount = 2;
		stats.baseDamaged += 5.f;
	}
	else if (stats.currentLevel == 3)
	{
		// 공격력 5 증가, 범위 10% 증가
		stats.baseDamaged += 5.f;
		stats.area += 10;
	}
	else if (stats.currentLevel == 4)
	{
		// 공격력 5 증가, 범위 10% 증가
		stats.baseDamaged += 5.f;
		stats.area += 10;
	}
}

void Whip::Fire()
{

}