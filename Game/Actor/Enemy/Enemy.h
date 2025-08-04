#pragma once

#include "Actor/Actor.h"

class Enemy :public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(Vector2& cameraPostion);
	//virtual ~Enemy();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	Vector2 worldPosition;
	Vector2& cameraPostion;
};
