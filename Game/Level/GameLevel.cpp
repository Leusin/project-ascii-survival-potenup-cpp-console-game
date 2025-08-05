#include "GameLevel.h"

#include "Engine.h"
#include "Game/Game.h"
#include "Utils/DebugManager.h"
#include "Actor/Actor.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"

GameLevel::GameLevel()
{
	player = new Player();
	AddActor(player); // 플레이어 추가

	// 타일맵 로드 후 배경에 출력
	ReadTileMapFile("TileMap.txt");

	AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST
	AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST
	AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST
	AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST
	AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST

	// TODO: 개발 완료 시 아래 함수 지우기
	DebugManager::Get().ToggleDebugMode();
}

GameLevel::~GameLevel()
{
	SafeDeleteArray(tileMap);
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float deltaTime)
{
	ProcessDebuge();

	//
	// 디버그 모드이고, 일시 정지 일 때
	//

	if (DebugManager::Get().IsDebugMode() && DebugManager::Get().IsGamePaused())
	{
		return; // 아래의 모든 로직을 건너뜀
	}

	super::Tick(deltaTime);
}

void GameLevel::Render()
{
	super::Render();

	/*
	* 현재 랜더 순서:
	*
	* INT_MAX - 디버그 정보
	*
	* 10 - Player
	*
	* 5 - Enemy, KnigeProjectile
	* 
	* 3 - ExpOrb
	* 
	* 0 - 배경
	*
	*/

	RenderBackground(); // 배경 그리기

	RenderUI(); // UI 그리기

	RenderDebugeData(); // 디버그 데이터 랜더
}

void GameLevel::ReadTileMapFile(const char* filename)
{
	// 최종 에셋 경로 
	char filePath[256] = { };
	sprintf_s(filePath, 256, "../Assets/%s", filename);

	// 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, filePath, "rt"); // 열기 모드 t 접두사를 쓰면 \n\r -> \n

	// 예외 처리
	if (file == nullptr)
	{
		printf("맵 파일 읽기 실패: %s.\n", filePath);
		__debugbreak();

		return;
	}

	// 파싱
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// 확인할 파일 크기 활용해 버퍼 할당
	char* buffer = new char[fileSize + 1];
	memset(buffer, 9, fileSize + 1);
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	int index = 0; // 배열 순회를 위한 인덱스
	int size = (int)readSize; // 문자열 길이 값

	//
	// 타일맵의 동적 할당 초기화를 위해 맵 크기 계산
	//

	int width = 0;
	int height = 0;
	int tempWidth = 0;
	for (size_t i = 0; i < fileSize; ++i)
	{
		if (buffer[i] == '\n') // COL: 개행을 만났을 경우
		{
			if (tempWidth > width)
			{
				width = tempWidth;
			}
			tempWidth = 0;
			++height;
		}
		else // ROW
		{
			++tempWidth;
		}
	}

	if (tempWidth > 0) // 이게 뭐야 마지막 줄 계산인가
	{
		++height;
		if (tempWidth > width)
		{
			width = tempWidth;
		}
	}

	tileWidth = width;
	tileHeight = height;

	// 1차원 배열 할당
	tileMap = new char[tileWidth * tileHeight];
	memset(tileMap, ' ', tileWidth * tileHeight); // 공백으로 초기화

	//
	// 파싱해서 tileMap에 넣기
	//

	int x = 0, y = 0;
	for (size_t i = 0; i < fileSize; ++i)
	{
		char ch = buffer[i];
		if (ch == '\n')
		{
			++y;
			x = 0;
			continue;
		}
		if (x < tileWidth && y < tileHeight)
		{
			tileMap[y * tileWidth + x] = ch;
			++x;
		}
	}

	delete[] buffer;

	// 파일 닫기
	fclose(file);
}

void GameLevel::RenderBackground()
{
	for (int screenY = 0; screenY < Engine::Get().Height(); ++screenY)
	{
		for (int screenX = 0; screenX < Engine::Get().Width(); ++screenX)
		{
			// 화면 좌표에 카메라 월드 좌표를 더하여, 그려야 할 타일의 월드 좌표를 구함
			int worldY = (int)player->GetCameraPosition().y - screenY;
			int worldX = (int)player->GetCameraPosition().x + screenX; // Y축은 아래로 갈수록 값이 줄어들어야 함

			// 타일링
			int tileX = (worldX % tileWidth + tileWidth) % tileWidth;
			int tileY = (worldY % tileHeight + tileHeight) % tileHeight;

			char tileChar[2] = { tileMap[tileY * tileWidth + tileX], '\0' };

			Engine::Get().WriteToBuffer({ screenX, screenY }, tileChar, Color::Intensity, 0);
		}
	}
}

void GameLevel::RenderUI()
{
	int sortingOrder = 10;
	int barWidth = Engine::Get().Width();

	// EXP 
	float expRatio = player->GetExpRatio();
	float filledExp = static_cast<float>(barWidth * expRatio); // 채워진 부분의 너비

	// HP
	float hpRatio = player->GetHpRatio();
	int filledHp = static_cast<int>(barWidth * hpRatio); // 채워진 부분의 너비

	for (int xi = 0; xi < Engine::Get().Width(); ++xi)
	{
		Color hpColor = (xi < filledHp) ? Color::Red : Color::White;
		Color exColor = (xi < filledExp) ? Color::Blue : Color::White;
		char hpbarChar[2] = { '#', '\0' };

		Engine::Get().WriteToBuffer({xi, 0}, hpbarChar, exColor, sortingOrder);
		Engine::Get().WriteToBuffer({xi, Engine::Get().Height() - 1 }, hpbarChar, hpColor, sortingOrder);
	}

}

void GameLevel::ProcessDebuge()
{
	// 틸트'~' 키를 눌렀을 때 디버그 모드를 토글
	if (Input::Get().GetKeyDown(VK_OEM_3))
	{
		DebugManager::Get().ToggleDebugMode();
	}

	if (!DebugManager::Get().IsDebugMode())
	{
		return;
	}

	// !/1번 키를 눌렀을 때 게임 일시 정지 토글
	if (Input::Get().GetKeyDown('1'))
	{
		DebugManager::Get().ToggleGamePause();
	}

	// 2번 키를 눌렀을 업그레이드 레벨로 이동
	if (Input::Get().GetKeyDown('2'))
	{
		Game::Get().TriggerUpgradeLevel();
	};

	// 3번 키를 눌렀을 때 하나 스폰
	if (Input::Get().GetKeyDown('3'))
	{
		AddActor(new Enemy(player->GetCameraPosition())); // 적 스폰 TEST
	};

}

void GameLevel::RenderDebugeData()
{
	// 디버그므로 가장 앞으로
	int renderOrder = INT_MAX;

	// 1. 모드 상태
	if (!DebugManager::Get().IsDebugMode())
	{
		return;
	}
	else
	{
		char buffer1[60] = {};
		sprintf_s(buffer1, 60, "[KEY'~']ToggleDEBUG");
		Engine::Get().WriteToBuffer(Vector2I(0, Engine::Get().Height() - 8), buffer1, Color::Green, renderOrder);

		sprintf_s(buffer1, 60, "-------------------- ");
		Engine::Get().WriteToBuffer(Vector2I(0, Engine::Get().Height() - 7), buffer1, Color::Green, renderOrder);
	}

	// 2. 일시정지 안내
	char buffer2[60] = {};
	sprintf_s(buffer2, 60, "[KEY'1']GamePause");
	Color isPusecolor = DebugManager::Get().IsGamePaused() ? Color::Red : Color::Green;
	Engine::Get().WriteToBuffer(Vector2I(0, Engine::Get().Height() - 6), buffer2, isPusecolor, renderOrder);

	// 3. 업그레이드 레벨로 이동
	char buffer3[60] = {};
	sprintf_s(buffer3, 60, "[KEY'2']UpgradeLevel");
	Engine::Get().WriteToBuffer(Vector2I(0, Engine::Get().Height() - 5), buffer3, Color::Green, renderOrder);

	// 4. 적 스폰 정보
	char buffer4[60] = {};
	sprintf_s(buffer4, 60, "[KEY'3']EnemySpwned:(%d)", Enemy::count);
	Engine::Get().WriteToBuffer(Vector2I(0, Engine::Get().Height() - 4), buffer4, Color::Green, renderOrder);
}