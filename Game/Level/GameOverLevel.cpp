#include "GameOverLevel.h"

#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Actor/Player/Player.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Weapons/Weapon.h"

GameOverLevel::GameOverLevel(int playerLevel, const std::vector<class Weapon*>& weapons, float playTime, bool win)
	: playerLevel(playerLevel)
	, weapons(weapons)
	, playTime(playTime)
	, isWin(win)
{
	items.emplace_back(new MenuItem("> Quit Game", []() { Game::Get().Quit(); }));
	items.emplace_back(new MenuItem("> New Game", []() { Game::Get().RestartGame(); }));

	if (win)
	{
		renderColor = Color::White;
	}
	else
	{
		selectedColor = Color::Red;
		renderColor = Color::Red;
	}
}

GameOverLevel::~GameOverLevel()
{
	for (MenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void GameOverLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	int length = (int)items.size();

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
		items[currentIndex]->onSelected();
	}
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		//Game::Get().ToggleMeneu();

		currentIndex = 0;
	}
}

void GameOverLevel::Render()
{
	super::Render();

	Game::Get().RenderBackgrounLevel();

	DrawRIPFrame();

	DrawRIPData(Enemy::GetKiledCount() , playTime);

	// 메뉴 아이템 렌더링.
	int frameHeight = 20;
	Vector2I center = Engine::Get().ScreenCenter();
	int startY = center.y - frameHeight / 2;

	// Start drawing the menu below the RIP frame
	int menuStartY = startY + frameHeight - 2;

	for (int ix = 0; ix < (int)items.size(); ++ix)
	{
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;

		// Use the Engine's buffer to draw the menu text
		Engine::Get().WriteToBuffer({ center.x - 5, menuStartY - ix }, items[ix]->menuText, textColor, sortingOrder);
	}
}

void GameOverLevel::DrawRIPFrame()
{
	Vector2I center = Engine::Get().ScreenCenter();

	// 테두리 문자들
	const char* hLine = "-";
	const char* vLine = "|";
	const char* corner = "+";
	const char* space = " ";

	int frameWidth = 40;
	int frameHeight = 20;
	int startX = center.x - frameWidth / 2;
	int startY = center.y - frameHeight / 2;

	// 공백으로 채우기
	for (int y = 1; y < frameHeight; ++y)
	{
		for (int x = 1; x < frameWidth; ++x)
		{
			Engine::Get().WriteToBuffer({ startX + x, startY + y }, space, renderColor, sortingOrder);
		}
	}

	// 모서리
	Engine::Get().WriteToBuffer({ startX, startY }, corner, renderColor, sortingOrder);
	Engine::Get().WriteToBuffer({ startX + frameWidth, startY }, corner, renderColor, sortingOrder);
	Engine::Get().WriteToBuffer({ startX, startY + frameHeight }, corner, renderColor, sortingOrder);
	Engine::Get().WriteToBuffer({ startX + frameWidth, startY + frameHeight }, corner, renderColor, sortingOrder);

	// 가로선
	for (int i = 1; i < frameWidth; ++i)
	{
		Engine::Get().WriteToBuffer({ startX + i, startY }, hLine, renderColor, sortingOrder);
		Engine::Get().WriteToBuffer({ startX + i, startY + frameHeight }, hLine, renderColor, sortingOrder);
	}

	// 세로선
	for (int i = 1; i < frameHeight; ++i)
	{
		Engine::Get().WriteToBuffer({ startX, startY + i }, vLine, renderColor, sortingOrder);
		Engine::Get().WriteToBuffer({ startX + frameWidth, startY + i }, vLine, renderColor, sortingOrder);
	}

	// 제목
	if (isWin)
	{
		const char* title = "You Survive!";
		int titleX = startX + (frameWidth - (int)strlen(title)) / 2;
		Engine::Get().WriteToBuffer({ titleX, startY + 2 }, title, renderColor, sortingOrder);
	}
	else
	{
		const char* title = "You died.";
		int titleX = startX + (frameWidth - (int)strlen(title)) / 2;
		Engine::Get().WriteToBuffer({ titleX, startY + 2 }, title, renderColor, sortingOrder);
	}
}

void GameOverLevel::DrawRIPData(int enemiesKilled, float playTime)
{
	Vector2I center = Engine::Get().ScreenCenter();

	int frameWidth = 40;
	int frameHeight = 20;
	int startX = center.x - frameWidth / 2;
	int startY = center.y - frameHeight / 2;

	// 플레이어 레벨
	char levelBuffer[50];
	sprintf_s(levelBuffer, "Player Level: %d", playerLevel);
	Engine::Get().WriteToBuffer({ startX + 5, startY + 5 }, levelBuffer, renderColor, sortingOrder);

	// 처치한 적 수
	char killedBuffer[50];
	sprintf_s(killedBuffer, "Killed enemies: %d", enemiesKilled);
	Engine::Get().WriteToBuffer({ startX + 5, startY + 6 }, killedBuffer, renderColor, sortingOrder);

	// 플레이 시간
	char timeBuffer[50];
	int minutes = static_cast<int>(playTime) / 60;
	int seconds = static_cast<int>(playTime) % 60;
	sprintf_s(timeBuffer, "Play Time: %02d:%02d", minutes, seconds);
	Engine::Get().WriteToBuffer({ startX + 5, startY + 7 }, timeBuffer, renderColor, sortingOrder);

	// 최종 무기
	// 이 부분은 UpgradeLevel::Render() 코드와 유사하게 구현하면 됩니다.
	char weaponBuffer[50];
	sprintf_s(weaponBuffer, "[Final Weapon] ");
	Engine::Get().WriteToBuffer({ startX + 5, startY + 9 }, weaponBuffer, renderColor, sortingOrder);

	int offsetCount = 0;
	for (int i = 0; i < weapons.size(); i++)
	{
		Weapon* w = weapons[i];
		if (w->GetLevel() == 0)
		{
			continue;
		}
		char weaponDetail[50];
		sprintf_s(weaponDetail, sizeof(weaponDetail), "Lv. %d %s", w->GetLevel(), w->name);

		Engine::Get().WriteToBuffer({ startX + 5, startY + 10 + offsetCount++ }, weaponDetail, renderColor, sortingOrder);
	}

}