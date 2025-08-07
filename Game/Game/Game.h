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
	void GoToGameOverLevel(int playerLevel, const std::vector<class Weapon*>& weapons, float playTime = 0, bool win = false);
	void ReturnToGameLevel();
	void RestartGame();

	void RenderBackgrounLevel();

	virtual void CleanUp() override;

private:
	class Level* backgroundLevel = nullptr; // 대기중인 레벨

	bool GameInBackground = false; // 만일 레벨이 3개 이상 늘어난다면 enum 으로 

	static Game* instance;
};