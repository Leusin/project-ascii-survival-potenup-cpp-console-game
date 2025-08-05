#pragma once

#include "Actor/Actor.h"

class ExpOrb: public Actor
{
	RTTI_DECLARATIONS(ExpOrb, Actor)

public:
	ExpOrb(Vector2I spawnPosition, const Vector2I& cameraPostion, float exp = 1.0f);
	//virtual ~ExpOrb();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	//virtual void Render() override;

public:
	void OnCollected(class Player* player);

private:
	float exp;
	Vector2I worldPosition;
	const Vector2I& cameraPosition;
};
