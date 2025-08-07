#include "WhipProjectile.h"

WhipProjectile::WhipProjectile(float damage, float area, const Vector2I Position)
	: Actor("$", Color::LightWhite, Position)
	, damage(damage)
	, area(area)
{
	//Set
}



void WhipProjectile::BeginPlay()
{
	super::BeginPlay();

	SetLifetime(1.f); // 무조건 1초동안
}

void WhipProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void WhipProjectile::Render()
{
	super::Render();
}
