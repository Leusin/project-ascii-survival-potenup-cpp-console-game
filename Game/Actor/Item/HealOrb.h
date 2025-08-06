#pragma once

#include "Actor/Item/Item.h"

class HealOrb : public Item
{
	RTTI_DECLARATIONS(HealOrb, Item)

public:
	HealOrb(Vector2I spawnPosition, const Vector2I& cameraPostion, float amount = 10.f);

	//virtual void BeginPlay() override;
	//virtual void Tick(float deltaTime) override;
	//virtual void Render() override;

	virtual void OnCollected(class Player* player) override;

private:
	float amount;
};
