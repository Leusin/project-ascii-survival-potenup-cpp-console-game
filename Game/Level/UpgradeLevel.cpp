#include "UpgradeLevel.h"

#include <random>
#include "Utils/Utils.h"
#include "Game/Game.h"
#include "Actor/Weapons/Weapon.h"

upgradeItem::upgradeItem(const char* icon, const char* name, const char* description, OnSelected onSelected)
	: onSelected(onSelected)
{
	size_t len = (int)strlen(icon) + 1;
	this->icon = new char[len];
	strcpy_s(this->icon, len, icon);

	len = (int)strlen(name) + 1;
	this->name = new char[len];
	strcpy_s(this->name, len, name);

	len = (int)strlen(description) + 1;
	this->description = new char[len];
	strcpy_s(this->description, len, description);
}

upgradeItem::~upgradeItem()
{
	SafeDeleteArray(icon);
	SafeDeleteArray(name);
	SafeDeleteArray(description);
}

UpgradeLevel::UpgradeLevel()
{
	const char* source = ">>> Level Up! <<<";
	size_t len = strlen(source) + 1;
	this->title = new char[len];
	strcpy_s(this->title, len, source);
}

UpgradeLevel::~UpgradeLevel()
{
	SafeDeleteArray(title);

	ClenupItems();
}

void UpgradeLevel::SetTitleText(const char* text)
{
	SafeDeleteArray(title);

	size_t len = (int)strlen(title) + 1;
	this->title = new char[len];
	strcpy_s(this->title, len, "LevelUp");
}

void UpgradeLevel::Initialize(const std::vector<class Weapon*>& weapons)
{
	ClenupItems();

	std::vector<Weapon*> upgradeCandidates;
	for (Weapon* weapon : weapons)
	{
		if (weapon->GetLevel() < weapon->GetMaxLevel())
		{
			upgradeCandidates.emplace_back(weapon);
		}
	}

	// 후보 목록을 무작위로 섞음
	// std::shuffle은 <random> 헤더 필요
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(upgradeCandidates.begin(), upgradeCandidates.end(), g);

	for (int i = 0; i < upgradeCandidates.size() && i < maxItemCount; i++)
	{
		Weapon* candidate = upgradeCandidates[i];

		char iconArray[] = { candidate->icon, '\0' };
		char description[10];
		if ((candidate->GetLevel() + 1) == candidate->GetMaxLevel())
		{
			sprintf_s(description, sizeof(description), "Lv. MAX");
		}
		else
		{
			sprintf_s(description, sizeof(description), "Lv. %d", candidate->GetLevel() + 1);
		}

		auto upgrade = [candidate]() {candidate->LevelUp(); };

		items.emplace_back(new upgradeItem(iconArray, candidate->GetImage(), description, upgrade));
	}
}

void UpgradeLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 벡터가 비어있다면 아무것도 하지 않음
	int length = static_cast<int>(items.size());
	if (length == 0)
	{
		Game::Get().ReturnToGameLevel();
		return;
	}

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
			Game::Get().ReturnToGameLevel();
		}
	}
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().Quit();

		currentIndex = 0;
	}
}

void UpgradeLevel::Render()
{
	super::Render();

	Game::Get().RenderBackgrounLevel();

	int length = static_cast<int>(items.size());
	if (length == 0)
	{
		Game::Get().ReturnToGameLevel();
		return;
	}

	// 랜더 순서
	int sortingOrder = 100;

	// 사용될 문자들
	char underbar[2] = { '_', '\0' };
	char slash[2] = { '/', '\0' };
	char gap[2] = { ' ', '\0' };
	char backslash[2] = { '\\', '\0' };

	int xOffset = Engine::Get().halfWidth() - ((int)strlen(title) + 1) / 2;
	int YOffset = 4;

	// 메뉴 타이틀 (LEVEL UP)
	Engine::Get().WriteToBuffer({ xOffset, YOffset }, title, Color::Yellow, sortingOrder);


	// 루프 밖에서 고정된 값들을 선언
	xOffset = 24;
	int yStartOffset = 6; // 첫 번째 아이템이 시작될 y 좌표
	int screenWidth = Engine::Get().Width();
	int lineLength = screenWidth - (2 * xOffset);
	int frameHeight = 5; // 프레임의 총 높이 (5줄)
	int textOffset = 4;

	// 메뉴 아이템 렌더링.
	for (int ix = 0; ix < length; ++ix)
	{
		// 메뉴 색상 설정
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;

		// 현재 아이템의 y 좌표
		int currentYOffset = yStartOffset + (ix * (frameHeight + 1)); // 1은 아이템 간의 간격

		// 0. 윗 프레임
		for (int j = 0; j < lineLength; j++)
		{
			Engine::Get().WriteToBuffer({ xOffset + j, currentYOffset }, underbar, textColor, sortingOrder);
		}

		// 1. 두 번째 줄 프레임
		Engine::Get().WriteToBuffer(Vector2I(xOffset - 1, currentYOffset + 1), slash, textColor, sortingOrder);
		for (int j = 0; j < lineLength; j++)
		{
			Engine::Get().WriteToBuffer(Vector2I(xOffset + j, currentYOffset + 1), gap, textColor, sortingOrder);
		}
		Engine::Get().WriteToBuffer(Vector2I(xOffset + lineLength, currentYOffset + 1), backslash, textColor, sortingOrder);

		// 2. 세 번째 줄 프레임
		Engine::Get().WriteToBuffer(Vector2I(xOffset - 2, currentYOffset + 2), slash, textColor, sortingOrder);
		for (int j = 0; j < lineLength + 2; j++)
		{
			Engine::Get().WriteToBuffer(Vector2I(xOffset + j - 1, currentYOffset + 2), gap, textColor, sortingOrder);
		}
		Engine::Get().WriteToBuffer(Vector2I(xOffset + lineLength + 1, currentYOffset + 2), backslash, textColor, sortingOrder);

		// 3. 네 번째 줄 프레임
		Engine::Get().WriteToBuffer(Vector2I(xOffset - 2, currentYOffset + 3), backslash, textColor, sortingOrder);
		for (int j = 0; j < lineLength + 2; j++)
		{
			Engine::Get().WriteToBuffer(Vector2I(xOffset + j - 1, currentYOffset + 3), gap, textColor, sortingOrder);
		}
		Engine::Get().WriteToBuffer(Vector2I(xOffset + lineLength + 1, currentYOffset + 3), slash, textColor, sortingOrder);

		// 4. 다섯 번째 줄 프레임
		Engine::Get().WriteToBuffer(Vector2I(xOffset - 1, currentYOffset + 4), backslash, textColor, sortingOrder);
		for (int j = 0; j < lineLength; j++)
		{
			Engine::Get().WriteToBuffer(Vector2I(xOffset + j, currentYOffset + 4), underbar, textColor, sortingOrder);
		}
		Engine::Get().WriteToBuffer(Vector2I(xOffset + lineLength, currentYOffset + 4), slash, textColor, sortingOrder);


		// 아이콘 (세 번째 줄)
		char itemBuffer[10] = {};
		sprintf_s(itemBuffer, 10, "'%s'", items[ix]->icon);
		Engine::Get().WriteToBuffer(Vector2I(xOffset + textOffset, currentYOffset + 2), itemBuffer, textColor, sortingOrder);

		// 이름 (세 번째 줄, 아이콘 옆)
		char namebuffer[100] = {};
		sprintf_s(namebuffer, 100, "%s", items[ix]->name);
		Engine::Get().WriteToBuffer(Vector2I(xOffset + textOffset + 4, currentYOffset + 2), namebuffer, textColor, sortingOrder);

		// 설명 (네 번째 줄)
		char descriptionbuffer[100] = {};
		sprintf_s(descriptionbuffer, 100, "%s", items[ix]->description);
		Engine::Get().WriteToBuffer(Vector2I(xOffset + textOffset + 4, currentYOffset + 3), descriptionbuffer, textColor, sortingOrder);
	}

}

void UpgradeLevel::ClenupItems()
{
	if (items.empty())
	{
		return;
	}

	for (upgradeItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}
