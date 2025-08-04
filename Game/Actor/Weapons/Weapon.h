#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
#include "Stats/WeaponStats.h"

class Weapon :public Actor
{
	RTTI_DECLARATIONS(Weapon, Actor)

public:
	Weapon(Vector2& playerPosition);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	inline void SetDirection(const Vector2& newDirection) { direction = newDirection; }

private:
	float speed = 18.f; // 무기의 속도 (투사체의 속도)

	Vector2 direction = { 0, 1 }; // 무기의 방향 (투사체의 방향이 될 것)
	Vector2& playerPosition; // 플레이어의 위치 (월드 좌표계에서)
	Vector2 worldPosition = Vector2::Zero; // 무기의 월드 위치
};
