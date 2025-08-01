#pragma once

#include "Actor/Actor.h"

class Temp :public Actor
{
	RTTI_DECLARATIONS(Temp, Actor)

public:
	Temp();
	~Temp();

	virtual void Tick(float deltaTime) override;
};
