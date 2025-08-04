#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
#include "Stats/WeaponStats.h"

class Weapon :public Actor
{
	RTTI_DECLARATIONS(Weapon, Actor)

public:
	Weapon(Vector2& playerPosition);
	virtual ~Weapon() = default;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

protected:
	WeaponStats stats;
	Vector2& playerPosition; // 플레이어의 위치 (월드 좌표계에서)
	Vector2 worldPosition = Vector2::Zero; // 무기의 월드 위치
};
