#include "Weapon.h"

#include "Engine.h"

Weapon::Weapon(const Vector2I& cameraPosition)
	: Actor("-", Color::White, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, cameraPosition(cameraPosition)
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

	if (stats.currentLevel < 1)
	{
		return;
	}
}

void Weapon::Render()
{
	super::Render();
}

void Weapon::LevelUp()
{
	if (stats.currentLevel + 1 > stats.maxLevel)
	{
		return;
	}

	++stats.currentLevel;
}
