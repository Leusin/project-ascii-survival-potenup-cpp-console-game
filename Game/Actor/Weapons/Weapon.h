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
	inline Vector2& GetPlayerPosition() const { return playerPosition; };

	WeaponStats stats;
	Vector2& playerPosition; // 플레이어의 위치 (월드 좌표계에서)
};
