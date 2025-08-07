#include "Whip.h"

#include "Engine.h"
#include "Level/GameLevel.h"
#include "Actor/Projectile/WhipProjectile.h"

Whip::Whip(const Vector2I& cameraPosition, Vector2I& direiction)
	: Weapon("~", "Whip", cameraPosition)
	, direction(direiction)
{
	stats.currentLevel = 0;
	stats.baseDamaged = 1.f;
	stats.area = 100;
	stats.cooldown = 1.35f;
	stats.amount = 1;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);
	projectilesToFire = stats.amount;

	upgradeDescription =
	{
		"Attacks horizontally",
		"Increases damage and adds one more projectile",
		"Increases damage and range",
		"Increases damage, range, and speed"
	};
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

	// 수평으로 공격합니다.
	if (stats.currentLevel == 1)
	{
		stats.baseDamaged = 1.f;
		stats.cooldown = 3.f;
		stats.amount = 1;
		stats.area = 60;
		stats.projectileInterval = 0.1f;
	}
	// 데미지가 늘고 투사체가 하나 더 늘어납니다.
	else if (stats.currentLevel == 2)
	{
		stats.baseDamaged = 3.3f;
		stats.amount = 2;
	}
	// 데미지와 범위가 강화됩니다.
	else if (stats.currentLevel == 3)
	{
		stats.baseDamaged += 6.f;
		stats.area = 80;
	}
	// 데미지와 범위가 그리고 속도가 강화됩니다.
	else if (stats.currentLevel == 4)
	{
		stats.baseDamaged += 8.0f;
		stats.cooldown = 2.35f;
		stats.area = 100;
	}
}

void Whip::Fire()
{
	// 'Whip'이 처음 발사될 때만 방향을 결정
	static bool isInitialFire = true;
	static bool fireLeft = false; // 기본은 오른쪽으로 시작

	if (isInitialFire)
	{
		Vector2I Up = { 0, 1 };
		int crossProductZ = Up.x * direction.y - Up.y * direction.x;

		if (crossProductZ > 0)
		{
			FireLeft();
			fireLeft = true;
		}
		else
		{
			FireRight();
			fireLeft = false;
		}
		isInitialFire = false;
	}
	else // 첫 발사 이후에는 번갈아가며 발사
	{
		if (fireLeft)
		{
			FireRight();
		}
		else
		{
			FireLeft();
		}
		fireLeft = !fireLeft; // 발사 방향 전환
	}
}

void Whip::FireRight()
{
	float finalLength = baseLength * (float)stats.area / 100.f;

	// 루프를 돌며 각 채찍을 생성
	Vector2I screenCenter = Engine::Get().ScreenCenter();
	for (size_t i = 0; i < lengthOffsets.size(); ++i)
	{
		Vector2I spawnPos = screenCenter + spawnOffsets[i];
		float length = finalLength * lengthOffsets[i];

		GetOwner()->AddActor(new WhipProjectile(stats.baseDamaged, (int)length, spawnPos));
	}

	// 위쪽 하나 더 생성
	//if (stats.currentLevel >= 4)
	//{
	//	for (size_t i = 0; i < lengthOffsets.size(); ++i)
	//	{
	//		Vector2I spawnPos = screenCenter + spawnOffsets[i] + Vector2I{ 0, -1 };
	//		float length = finalLength * lengthOffsets[i];
	//
	//		GetOwner()->AddActor(new WhipProjectile(stats.baseDamaged, (int)length, spawnPos));
	//	}
	//}
}

void Whip::FireLeft()
{
	float finalLength = baseLength * (float)stats.area / 100.f;

	//
	// 왼쪽 기준 오프셋 데이터으로 변환
	//

	for (size_t i = 0; i < lengthOffsets.size(); ++i)
	{
		int length = (int)(finalLength * lengthOffsets[i]);
		spawnOffsets[i].x += length;
		spawnOffsets[i].x *= -1;
	}

	// 루프를 돌며 각 채찍을 생성
	Vector2I screenCenter = Engine::Get().ScreenCenter();
	for (size_t i = 0; i < lengthOffsets.size(); ++i)
	{
		Vector2I spawnPos = screenCenter + spawnOffsets[i];
		float length = finalLength * lengthOffsets[i];

		GetOwner()->AddActor(new WhipProjectile(stats.baseDamaged, (int)length, spawnPos));
	}

	// 위쪽 하나 더 생성
	if (stats.currentLevel >= 4)
	{
		for (size_t i = 0; i < lengthOffsets.size(); ++i)
		{
			Vector2I spawnPos = screenCenter + spawnOffsets[i] + Vector2I{ 0, -1 };
			float length = finalLength * lengthOffsets[i];

			GetOwner()->AddActor(new WhipProjectile(stats.baseDamaged, (int)length, spawnPos));
		}
	}
}
