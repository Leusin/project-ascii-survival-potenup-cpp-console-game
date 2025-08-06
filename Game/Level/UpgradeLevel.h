#pragma once

#include "Level/Level.h"

#include <vector>
#include <functional>
#include "Core/Core.h"
#include "Actor/Actor.h"


using OnSelected = std::function<void()>;

struct upgradeItem
{
	upgradeItem(const char* icon, const char* name, const char* description, OnSelected onSelected);
	~upgradeItem();

	char* icon = nullptr; // 아이템 아이콘
	char* name = nullptr; // 아이템 이름
	char* description = nullptr; // 아이템 설명
	OnSelected onSelected = nullptr; // 메뉴 선택 시 실행할 동작
};


class UpgradeLevel : public Level
{
	RTTI_DECLARATIONS(UpgradeLevel, Level)

public:
	UpgradeLevel();
	~UpgradeLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;
	
public:
	void SetTitleText(const char* text);
	void Initialize(const std::vector<class Weapon*>& weapons);

private:
	void ClenupItems();

private:
	char* title;
	int currentIndex = 0; // 현재 선택된 아이템의 인덱스

	Color selectedColor = Color::LightGreen; // 아이템 선택 색.
	Color unselectedColor = Color::LightWhite; // 아이템 선택 안 됬을 때 색.

	std::vector<upgradeItem*> items; // 아이템 배열

	const int maxItemCount = 3;
};