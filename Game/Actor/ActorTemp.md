#pragma once

#include "Actor/Actor.h"

class Temp :public Actor
{
	RTTI_DECLARATIONS(Temp, Actor)

public:
	Temp();
	virtual ~Temp();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;
};
