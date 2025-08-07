#include "HealOrb.h"

#include "Actor/Player/Player.h"

HealOrb::HealOrb(Vector2I spawnPosition, const Vector2I& cameraPostion, float amount)
	: Item("+", Color::LightGreen, spawnPosition, cameraPostion)
	, amount(amount)
{
	SetSortingOrder(4);
}

void HealOrb::OnCollected(Player* player)
{
	player->AddHp(amount);

	Destroy();
}
