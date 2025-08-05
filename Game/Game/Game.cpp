#include "Game.h"

#include "Level/GameLevel.h"
#include "Level/UpgradeLevel.h"

Game::Game()
{
	// 메인 게임
	AddLevel(new GameLevel());

	upgradeLevel = new UpgradeLevel();
}

Game::~Game()
{
	if (showUpgrade)
	{
		//게임 레벨 재거
		SafeDelete(backgroundLevel);
		mainLevel = nullptr;
	}
	else
	{
		backgroundLevel = nullptr;
	}

	SafeDelete(upgradeLevel);

	Engine::CleanUp();
}

void Game::TriggerUpgradeLevel()
{
	showUpgrade = true;

	backgroundLevel = mainLevel; // 게임 레벨을 background 로 빌어 둠
	mainLevel = upgradeLevel; // 업그레이드가 보이게 함.
}

void Game::ReturnToGameLevel()
{
	showUpgrade = false;

	mainLevel = backgroundLevel; // background 로 밀어둔 게임 레벨 보이기
}

