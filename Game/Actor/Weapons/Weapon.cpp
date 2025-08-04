#include "Weapon.h"

#include "Engine.h"

Weapon::Weapon(Vector2& playerPosition)
	: Actor("-", Color::White, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, playerPosition(playerPosition)
{
	SetSortingOrder(5);
}

void Weapon::BeginPlay()
{
	super::BeginPlay();
}

void Weapon::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void Weapon::Render()
{
	super::Render();
}
