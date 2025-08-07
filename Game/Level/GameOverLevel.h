#pragma once

#include "Level/Level.h"

#include "MenuItem.h"
#include "Math/Color.h"

class GameOverLevel : public Level
{
	RTTI_DECLARATIONS(GameOverLevel, Level)

public:
	GameOverLevel(int playerLevel, const std::vector<class Weapon*>& weapons, float playTime = 0.f, bool win = false);
	~GameOverLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void DrawRIPFrame();
	void DrawRIPData(int enemiesKilled, float playTime = 0);

private:
	int currentIndex = 0; // 현재 선택된 아이템의 인덱스

	Color selectedColor = Color::Green; // 아이템 선택 색.
	Color unselectedColor = Color::Intensity; // 아이템 선택 안 됬을 때 색.
	std::vector<MenuItem*> items; // 아이템 배열

	int playerLevel = 0;
	const std::vector<class Weapon*>& weapons;
	float playTime = 0;
	bool isWin = false;

	Color renderColor = Color::White;
	int sortingOrder = 100;
};