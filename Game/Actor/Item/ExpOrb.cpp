#include "ExpOrb.h"

#include "Engine.h"
#include "Utils/Utils.h"
#include "Actor/Player/Player.h"

ExpOrb::ExpOrb(Vector2F spawnPosition, Vector2F& cameraPostion, float exp)
	: Actor("*", Color::LightCyan)
	, exp(exp)
	, cameraPosition(cameraPostion)
	, worldPosition{ (int)spawnPosition.x, (int)spawnPosition.y }
{
	SetSortingOrder(3);

	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);

	//worldPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);

	if (exp > 25.f)
	{
		color = Color::Red;
	}
	else if (exp > 10.f)
	{
		color = Color::LightBlue;
	}
}

void ExpOrb::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 화면 좌표계로 변환
	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);
}

void ExpOrb::OnCollected(Player* player)
{
	player->AddExp(exp);

	Destroy();
}
