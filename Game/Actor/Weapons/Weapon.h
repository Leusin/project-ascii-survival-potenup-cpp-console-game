#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2F.h"
#include "Stats/WeaponStats.h"

class Weapon :public Actor
{
	RTTI_DECLARATIONS(Weapon, Actor)

public:
	Weapon(const Vector2I& cameraPosition);
	virtual ~Weapon() = default;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

protected:
	inline const Vector2I& GetCameraPosition() const { return cameraPosition; };

	WeaponStats stats;
	const Vector2I& cameraPosition; // 플레이어의 위치 (월드 좌표계에서)
};
