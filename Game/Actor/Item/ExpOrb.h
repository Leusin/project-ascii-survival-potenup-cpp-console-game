#pragma once

#include "Actor/Item/Item.h"

class ExpOrb: public Item
{
	RTTI_DECLARATIONS(ExpOrb, Item)

public:
	ExpOrb(Vector2I spawnPosition, const Vector2I& cameraPostion, float exp = 1.0f);
	//virtual ~ExpOrb();

	//virtual void BeginPlay() override;
	//virtual void Tick(float deltaTime) override;
	//virtual void Render() override;

public:
	void OnCollected(class Player* player) override;

private:
	float exp;
};
