#pragma once

#include "Weapon.h"

#include <vector>
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
	void FireRight();
	void FireLeft();

	bool isFiring = false;
	int projectilesToFire;

	/// <summary>
	/// 투척 방향
	/// </summary>
	Vector2I& direction = Vector2I::Right;

	Timer cooldownTimer;
	Timer fireIntervalTimer;

	float baseLength = 8.f; // 기본값
	// 채찍의 길이 오프셋 
	std::vector<float> lengthOffsets = { 0.8f, 1.2f, 1.4f , 0.6f };
	// 채찍 스폰 위치 오프셋
	std::vector<Vector2I> spawnOffsets = {
		{ 1, 0 },
		{ (int)(baseLength * 0.4f), -1 },
		{ (int)(baseLength * 1.0f), 0 },
		{ (int)(baseLength * 1.7f), -1  }
	};
};