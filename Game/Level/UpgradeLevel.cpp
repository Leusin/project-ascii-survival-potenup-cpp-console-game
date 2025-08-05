#include "UpgradeLevel.h"

#include "Utils/Utils.h"
#include "Game/Game.h"

MenuItem::MenuItem(const char* text, OnSelected onSelected)
	: onSelected(onSelected)
{
	size_t len = (int)strlen(text) + 1;
	menuText = new char[len];
	strcpy_s(menuText, len, text);
}

MenuItem::~MenuItem()
{
	SafeDeleteArray(menuText);
}

UpgradeLevel::UpgradeLevel()
{
	// 메뉴 아이템 추가
	items.emplace_back(new MenuItem("Resum Game", []() { Game::Get().ReturnToGameLevel(); }));
	items.emplace_back(new MenuItem("Quit Game", []() { Game::Get().Quit(); }));

	length = static_cast<int>(items.size());
}

UpgradeLevel::~UpgradeLevel()
{
	for (MenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void UpgradeLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	if (Input::Get().GetKeyDown(VK_UP))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}
	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1 + length) % length;
	}
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		if (items[currentIndex]->onSelected != nullptr)
		{
			items[currentIndex]->onSelected();
		}
	}
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ReturnToGameLevel();

		currentIndex = 0;
	}
}

void UpgradeLevel::Render()
{
	super::Render();
	
	Game::Get().RenderBackgrounLevel();

	// 색상 & 좌표 정리.
	//Utils::SetConsolePosition(COORD{ 0, 0 });
	//Utils::SetConsoleTextColor(static_cast<WORD>(unselectedColor));

	// 메뉴 아이템 렌더링.
	for (int ix = 0; ix < length; ++ix)
	{
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor; // 아이템 색상 확인.

		char buffer[100] = {};
		sprintf_s(buffer, 100, "%s", items[ix]->menuText);
		Engine::Get().WriteToBuffer(Vector2I(8, 8 + ix), buffer, textColor);
	}

}
