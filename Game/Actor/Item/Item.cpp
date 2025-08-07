#include "Item.h"

#include "Engine.h"

Item::Item(const char* img, Color color, Vector2I spawnPosition, const Vector2I& cameraPostion)
	: Actor{ img, color }
	, cameraPosition{ cameraPostion }
	, worldPosition{ spawnPosition }
{
	SetSortingOrder(1);

	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);
}

void Item::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 화면 좌표계로 변환
	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);
}
