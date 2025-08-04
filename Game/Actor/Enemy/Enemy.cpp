#include "Enemy.h"

#include "Engine.h"
#include "Utils/Utils.h"

#include <iostream>

Enemy::Enemy(Vector2& cameraPostion)
	: Actor("E", Color::Blue)
	, cameraPostion(cameraPostion)
{

	SetSortingOrder(5);

	//
	// 스폰 위치 가장 자리 중 랜덤으로
	// 

	int PosType = Utils::Random(0, 3);

	Vector2 spawnPostion;
	if (PosType == 0)
	{
		// 위 쪽이라면 (x, 1)
		spawnPostion = { Utils::Random(0, Engine::Get().Width()) , 0 };
	}
	else if (PosType == 1)
	{
		// 왼 쪽이라면 (1, y)
		spawnPostion = { 0, Utils::Random(1, Engine::Get().Height()) };
	}
	else if (PosType == 2)
	{
		// 아래 쪽이라면 (x, hight)
		spawnPostion = { Utils::Random(0, Engine::Get().Width()) , Engine::Get().Height() };
	}
	else if (PosType == 1)
	{
		// 오른 쪽이라면 (width, y)
		spawnPostion = { Engine::Get().Width(), Utils::Random(0, Engine::Get().Height()) };
	}

	SetPosition(spawnPostion);

	//
	// 월드 포지션도 갖게 해야함
	//

	worldPosition = Engine::Get().ScreenToOrthogonalCoords(spawnPostion);
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);


	//
	// 화면 이탈 확인 및 월드 좌표 조정
	//

	Vector2 screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPostion);

	/// 좌우 화면 처리

	// 화면 왼쪽을 벗어남 -> 월드 좌표를 화면 너비만큼 오른쪽으로 이동
	if (screenPosition.x < 0)
	{
		worldPosition.x += Engine::Get().Width();
	}
	// 화면 오른쪽을 벗어남 -> 월드 좌표를 화면 너비만큼 왼쪽으로 이동
	else if (screenPosition.x >= Engine::Get().Width())
	{
		worldPosition.x -= Engine::Get().Width();
	}

	/// 상하 화면 처리

	if (screenPosition.y < 0)
	{
		worldPosition.y -= Engine::Get().Height(); // 월드 좌표를 화면 높이만큼 아래로 이동
	}
	// 화면 아래쪽을 벗어났을 때 (y가 화면 높이 이상)
	else if (screenPosition.y >= Engine::Get().Height())
	{
		worldPosition.y += Engine::Get().Height(); // 월드 좌표를 화면 높이만큼 위로 이동
	}

	screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPostion);

	SetPosition(screenPosition);
}

void Enemy::Render()
{
	super::Render();
}
