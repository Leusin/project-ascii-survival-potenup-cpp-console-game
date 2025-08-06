#include "Game.h"

#include "Level/GameLevel.h"
#include "Level/UpgradeLevel.h"
#include "Actor/Weapons/Weapon.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	// 메인 게임
	AddLevel(new GameLevel());

	upgradeLevel = new UpgradeLevel();
}

Game::~Game()
{
	CleanUp();
}

Game& Game::Get()
{
	return *instance;
}

void Game::GoToUpgradeLevel(const std::vector<class Weapon*>& weapons)
{
	if (showUpgrade)
	{
		return;
	}

	showUpgrade = true;

	upgradeLevel->Initialize(weapons);

	backgroundLevel = mainLevel; // 게임 레벨을 background 로 빌어 둠
	mainLevel = upgradeLevel; // 업그레이드가 보이게 함.
}

void Game::ReturnToGameLevel()
{
	if (!showUpgrade)
	{
		return;
	}

	showUpgrade = false;

	mainLevel = backgroundLevel; // background 로 밀어둔 게임 레벨 보이기
	backgroundLevel = nullptr;
}

void Game::RenderBackgrounLevel()
{
	if (!backgroundLevel)
	{
		return;
	}

	backgroundLevel->Render();
}

void Game::CleanUp()
{
	if (showUpgrade)
	{
		SafeDelete(backgroundLevel);
		// mainLevel 에 있던 upgradeLevel 는 Engine 이 정리해준다.
	}
	else
	{
		backgroundLevel = nullptr;
		SafeDelete(upgradeLevel);
		// mainLevel 에 있던 GameLevel 은 Engine 이 정리해준다.
	}

	Engine::CleanUp();
}

