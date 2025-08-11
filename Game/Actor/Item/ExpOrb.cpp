#include "ExpOrb.h"

#include "Engine.h"
#include "Utils/Utils.h"
#include "Actor/Player/Player.h"

ExpOrb::ExpOrb(Vector2I spawnPosition, const Vector2I& cameraPostion, float exp)
	: Item("*", Color::LightCyan, spawnPosition, cameraPostion)
	, exp(exp)
{
	if (exp > 7.777f)
	{
		color = Color::Red;
	}
	else if (exp > 4.444f)
	{
		color = Color::Cyan;
	}
}

void ExpOrb::OnCollected(Player* player)
{
	player->AddExp(exp);

	Destroy();
}
