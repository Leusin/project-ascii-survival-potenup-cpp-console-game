#pragma once

#include "Actor/Actor.h"

#include "Math/Vector2.h"
#include "Actor/Component/CharacterStats.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	CharacterStats stats;
	Vector2 moveInput = { 0, 0 };

	// 마지막으로 이동한 방향
	// 투사체 의 방향이 될 것
	Vector2 direction = { 0, -1 };
};
