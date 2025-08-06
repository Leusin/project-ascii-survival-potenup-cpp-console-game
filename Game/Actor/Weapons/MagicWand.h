#pragma once

#include "Weapon.h"
#include "Utils/Timer.h"

/// <summary>
/// 현재 가진 무기 스텟 데이터를 바탕으로
/// 인정 간격 직선으로 나아가는 투사체를 발사시킨다.
/// </summary>
class MagicWand : public Weapon
{
	RTTI_DECLARATIONS(MagicWand, Weapon)

public:
	MagicWand(const Vector2I& cameraPosition);

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	//virtual void Render() override; // 랜더 할 필요 없다

	virtual void LevelUp() override;

private:
	void Fire();

	bool isFiring = false;
	int projectilesToFire = 0;

	Timer cooldownTimer;
	Timer fireIntervalTimer;
};