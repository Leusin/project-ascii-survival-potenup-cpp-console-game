#pragma once

#include "Weapon.h"
#include "Utils/Timer.h"

/// <summary>
/// 좌우로만 공격
/// </summary>
class Whip : public Weapon
{
	RTTI_DECLARATIONS(Whip, Weapon)

public:
	Whip(const Vector2I& cameraPosition, Vector2I& direiction);

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override; // 랜더 할 필요 없다

	virtual void LevelUp() override;

private:
	void Fire();

	bool isFiring = false;
	int projectilesToFire;

	/// <summary>
	/// 투척 방향
	/// </summary>
	Vector2I& direction = Vector2I::Right;

	Timer cooldownTimer;
	Timer fireIntervalTimer;
};