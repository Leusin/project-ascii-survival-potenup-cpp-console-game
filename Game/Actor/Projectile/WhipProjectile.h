#pragma once

#include "Actor/Actor.h"

#include "Utils/Timer.h"

/// <summary>
/// 물리적으로 나아가진 않지만
/// 알아서 없어지게 만들어야 한다.
/// </summary>
class WhipProjectile :public Actor
{
	RTTI_DECLARATIONS(WhipProjectile, Actor)

public:
	WhipProjectile(float damage, unsigned int area, const Vector2I position);
	//virtual ~WhipProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	float damage;
	unsigned int length;

	float targetTime = 0.4f;
	Timer cooldownTimer;
};
