#include "Whip.h"

#include "Engine.h"
#include "Level/GameLevel.h"
#include "Actor/Projectile/WhipProjectile.h"

Whip::Whip(const Vector2I& cameraPosition, Vector2I& direiction)
	: Weapon("~", "Whip", cameraPosition)
	, direction(direiction)
{
	color = Color::LightMagenta;

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
		"cooldown down by 0.2 / amount up by 1",
		"damage up by 1 / area up by 20",
		"damage up by 1 / cooldown down by 0.2 / area up by 20"
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

		// 최초 방향 판단
		Vector2I Up = { 0, 1 };
		int crossProductZ = Up.x * direction.y - Up.y * direction.x;

		initialDirectionLeft = (crossProductZ > 0);
		fireLeftThisTime = initialDirectionLeft; // 첫 방향
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
		stats.baseDamaged = 3.f;
		stats.cooldown = 2.4f;
		stats.amount = 1;
		stats.area = 60;
		stats.projectileInterval = 0.1f;
	}
	// 데미지가 늘고 투사체가 하나 더 늘어납니다.
	// cooldown down by 0.2 / amount up by 1
	else if (stats.currentLevel == 2)
	{
		stats.cooldown = 2.2f;
		stats.amount = 2;
	}
	// damage up by 1 / area up by 20
	else if (stats.currentLevel == 3)
	{
		stats.baseDamaged = 4.f;
		stats.area = 80;
	}
	// damage up by 1 / cooldown down by 0.2 / area up by 20
	else if (stats.currentLevel == 4)
	{
		stats.baseDamaged = 5.0f;
		stats.cooldown = 2.0f;
		stats.area = 100;
	}
}

void Whip::Fire()
{
	if (fireLeftThisTime)
	{
		FireLeft();
	}
	else
	{
		FireRight();
	}

	// 방향 반전 (번갈아가며 발사)
	fireLeftThisTime = !fireLeftThisTime;
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
}

void Whip::FireLeft()
{
	float finalLength = baseLength * (float)stats.area / 100.f;

	//
	// 왼쪽 기준 오프셋 데이터으로 변환
	//

	std::vector<Vector2I> modifiedOffsets = spawnOffsets; // 복사

	for (size_t i = 0; i < modifiedOffsets.size(); ++i)
	{
		int length = (int)(finalLength * lengthOffsets[i]);
		modifiedOffsets[i].x += length - 1;
		modifiedOffsets[i].x *= -1;
	}

	Vector2I screenCenter = Engine::Get().ScreenCenter();
	for (size_t i = 0; i < modifiedOffsets.size(); ++i)
	{
		Vector2I spawnPos = screenCenter + modifiedOffsets[i];
		float length = finalLength * lengthOffsets[i];

		GetOwner()->AddActor(new WhipProjectile(stats.baseDamaged, (int)length, spawnPos));
	}
}
