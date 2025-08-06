#pragma once

#include "Actor/Actor.h"

class Item :public Actor
{
	RTTI_DECLARATIONS(Item, Actor)

public:
	Item(const char* img, Color color, Vector2I spawnPosition, const Vector2I& cameraPostion);
	virtual ~Item() = default;

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	//virtual void Render() override;

	virtual void OnCollected(class Player* player) abstract;

private:
	Vector2I worldPosition;
	const Vector2I& cameraPosition;
};
