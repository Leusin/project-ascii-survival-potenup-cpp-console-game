#include "GameLevel.h"

#include "Actor/Actor.h"
#include "Actor/Player/Player.h"

GameLevel::GameLevel()
{
	player = new Player();
	AddActor(player); // 플레이어 추가
}

GameLevel::~GameLevel()
{
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void GameLevel::Render()
{
	super::Render();
}
