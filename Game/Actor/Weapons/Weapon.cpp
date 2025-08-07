#include "Weapon.h"

#include "Engine.h"

Weapon::Weapon(const char* img, const char* name, const Vector2I& cameraPosition)
	: Actor(img, Color::White, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, name(name)
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

const char* Weapon::GetUpgradeDescription()
{
	int index = stats.currentLevel;

	if (index >= upgradeDescription.size())
	{
		return nullptr;
	}

	return upgradeDescription[index];
}
