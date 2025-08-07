#include "Game.h"

#include "Level/GameLevel.h"
#include "Level/UpgradeLevel.h"
#include "Level/GameOverLevel.h"
#include "Actor/Weapons/Weapon.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	// 메인 게임
	AddLevel(new GameLevel());
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
	if (GameInBackground)
	{
		return;
	}

	GameInBackground = true;

	backgroundLevel = mainLevel; // 게임 레벨을 background 로 빌어 둠

	// 매번 새로운 UpgradeLevel 객체 동적 할당
	UpgradeLevel* upgradeLevel = new UpgradeLevel(weapons);
	mainLevel = upgradeLevel; // 업그레이드가 보이게 함.
}

void Game::GoToGameOverLevel(int playerLevel, const std::vector<class Weapon*>& weapons, float playTime, bool win)
{
	if (GameInBackground)
	{
		return;
	}

	GameInBackground = true;

	backgroundLevel = mainLevel;
	mainLevel = new GameOverLevel(playerLevel, weapons, playTime, win);
}

void Game::ReturnToGameLevel()
{
	if (!GameInBackground)
	{
		return;
	}

	GameInBackground = false;

	SafeDelete(mainLevel); // 현재 메인 레벨(업그레이드 레벨) 삭제
	mainLevel = backgroundLevel; // background 로 밀어둔 게임 레벨 보이기
	backgroundLevel = nullptr;
}

void Game::RestartGame()
{
	SafeDelete(backgroundLevel);
	AddLevel(new GameLevel());
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
	SafeDelete(mainLevel);
	SafeDelete(backgroundLevel);

	Engine::CleanUp();
}

