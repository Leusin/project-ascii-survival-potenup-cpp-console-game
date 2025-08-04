#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2F.h"
#include "Stats/WeaponStats.h"

class Weapon :public Actor
{
	RTTI_DECLARATIONS(Weapon, Actor)

public:
	Weapon(Vector2F& playerPosition);
	virtual ~Weapon() = default;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

protected:
	inline Vector2F& GetPlayerPosition() const { return playerPosition; };

	WeaponStats stats;
	Vector2F& playerPosition; // 플레이어의 위치 (월드 좌표계에서)
};
