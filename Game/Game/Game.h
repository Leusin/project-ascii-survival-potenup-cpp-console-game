#pragma once

#include "Engine.h"

#include <vector>

class Game : public Engine
{
public:
	Game();
	~Game();

	static Game& Get();

	void GoToUpgradeLevel(const std::vector<class Weapon*>& weapons); // Upgrade 화면으로 전환
	void ReturnToGameLevel();

	void RenderBackgrounLevel();

private:
	class UpgradeLevel* upgradeLevel = nullptr; // 강화 레벨
	class Level* backgroundLevel = nullptr; // 대기중인 레벨

	bool showUpgrade = false; // 만일 레벨이 3개 이상 늘어난다면 enum 으로 

	static Game* instance;
};