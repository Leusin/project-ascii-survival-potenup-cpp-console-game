#pragma once

#include "Weapon.h"
#include "Utils/Timer.h"

class Garlic :public Weapon
{
	RTTI_DECLARATIONS(Garlic, Weapon)

public:
	Garlic();
	//virtual ~Garlic();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual void LevelUp() override;

private:
	int GetRadious();

private:
	Timer cooldownTimer;
	int totalRadious = 20; // 반지름
	float ratio = 1.8f; // 원 비율(가로:세로)
};
