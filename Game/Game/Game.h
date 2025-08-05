#pragma once

#include "Engine.h"

class Game : public Engine
{
public:
	Game();
	~Game();

	void TriggerUpgradeLevel(); // Upgrade 화면으로 전환
	void ReturnToGameLevel();

private:
	class Level* upgradeLevel = nullptr; // 강화 레벨
	class Level* backgroundLevel = nullptr; // 대기중인 레벨

	bool showUpgrade = false; // 만일 레벨이 3개 이상 늘어난다면 enum 으로 
};