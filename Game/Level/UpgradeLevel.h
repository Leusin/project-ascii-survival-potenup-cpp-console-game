#pragma once

#include "Level/Level.h"

#include <vector>
#include "Core/Core.h"
#include "Actor/Actor.h"

struct MenuItem
{
	typedef void (*OnSelected)(); // 함수 포인터 선언

	MenuItem(const char* text, OnSelected onSelected);
	~MenuItem();

	char* menuText = nullptr; // 메뉴 텍스트
	OnSelected onSelected = nullptr; // 메뉴 선택 시 실행할 동작
};


class UpgradeLevel : public Level
{
	RTTI_DECLARATIONS(UpgradeLevel, Level)

public:
	UpgradeLevel();
	~UpgradeLevel();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	int currentIndex = 0; // 현재 선택된 아이템의 인덱스

	Color selectedColor = Color::Green; // 아이템 선택 색.
	Color unselectedColor = Color::White; // 아이템 선택 안 됬을 때 색.

	std::vector<MenuItem*> items; // 아이템 배열

	int length = 0; // 메뉴 아이템 수
};