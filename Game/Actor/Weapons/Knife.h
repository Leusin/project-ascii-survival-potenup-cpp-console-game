#pragma once

#include "Weapon.h"

class Knife : public Weapon
{
	RTTI_DECLARATIONS(Knife, Weapon)

public:
	Knife(Vector2& playerPosition, Vector2& direiction);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	/// <summary>
	/// 투척 방향
	/// </summary>
	Vector2 direction = Vector2::Right;
};