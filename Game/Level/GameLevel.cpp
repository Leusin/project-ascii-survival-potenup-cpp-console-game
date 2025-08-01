#include "GameLevel.h"

#include "Engine.h"
#include "Actor/Actor.h"
#include "Actor/Player/Player.h"

GameLevel::GameLevel()
{
	player = new Player();
	AddActor(player); // 플레이어 추가


	// TEST 맵 데이터를 채워넣는 로직
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 10; x++) {
			if (x == 0 || x == 10 - 1 || y == 0 || y == 8 - 1) {
				map[y][x] = '#';
			}
			else {
				map[y][x] = '.';
			}
		}
	}
}

GameLevel::~GameLevel()
{
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void GameLevel::Render()
{
	super::Render();

	//
	// 배경 그리기(TEMP)
	// 

	/// 타일 맵의 크기 [10][10]
	for (int screenY = 0; screenY < Engine::Get().Height(); ++screenY)
	{
		for (int screenX = 0; screenX < Engine::Get().Width(); ++screenX)
		{
			int worldY = screenY + (int)player->worldPosition.y;
			int worldX = screenX + (int)player->worldPosition.x;

			// 타일링
			int tileX = (worldX % 10 + 10) % 10;
			int tileY = (worldY % 8 + 8) % 8;

			char tileChar[2] = { map[tileY][tileX], '\0' };
			Engine::Get().WriteToBuffer({ screenX, screenY }, tileChar, Color::Intensity);
		}
	}

	// 카메라 위치 계산
	int cameraPosX = (int)player->worldPosition.x - Engine::Get().Width() / 2;
	int cameraPosY = (int)player->worldPosition.y - Engine::Get().Height() / 2;

	//
	// 디버그 정보 
	//

	// 디버깅을 위해 캐릭터의 위치 출력
	char buffer1[40] = { };
	sprintf_s(buffer1, 40, "Player Pos: (%.2f, %.2f)", player->worldPosition.x, player->worldPosition.y);
	Engine::Get().WriteToBuffer(Vector2(0, 0), buffer1); // 출력.

	char buffer2[40] = { };
	sprintf_s(buffer2, 40, "Camera Pos: (%d, %d)", cameraPosX, cameraPosY);
	Engine::Get().WriteToBuffer(Vector2(0, 1), buffer2); // 출력.

	//
	// 플레이어 랜더
	//

	// 플레이어는 무조건 정중앙에서 랜더
	char playerImg[2] = { '@' };
	Engine::Get().WriteToBuffer(
		{ Engine::Get().Width() / 2, Engine::Get().Height() / 2 },
		playerImg
	);
}
