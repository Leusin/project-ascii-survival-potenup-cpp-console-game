#include "GameLevel.h"

#include "Engine.h"
#include "Actor/Actor.h"
#include "Actor/Player/Player.h"

//TEST
#include "Actor/Enemy/Enemy.h"

GameLevel::GameLevel()
{
	player = new Player();
	AddActor(player); // 플레이어 추가

	// 타일맵 로드 후 배경에 출력
	ReadTileMapFile("TileMap.txt");

	AddActor(new Enemy(cameraPostion)); // 적 스폰 TEST
	//AddActor(new Enemy(cameraPostion)); // 적 스폰 TEST
	//AddActor(new Enemy(cameraPostion)); // 적 스폰 TEST
	//AddActor(new Enemy(cameraPostion)); // 적 스폰 TEST
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
	super::Tick(deltaTime);

	// 카메라 포지션 업데이트
	if (player != nullptr)
	{
		cameraPostion = player->GetWorldPosition();
	}
}

void GameLevel::Render()
{
	super::Render();

	RenderBackground(); // 배경 그리기

	//
	// 디버그 정보 
	//

	// 디버깅을 위해 캐릭터의 위치 출력
	char buffer1[40] = { };
	sprintf_s(buffer1, 40, "Player Pos: (%.2f, %.2f)", player->GetWorldPosition().x, player->GetWorldPosition().y);
	Engine::Get().WriteToBuffer(Vector2(0, 0), buffer1); // 출력.
}

Vector2 GameLevel::GetCameraPosition() const
{
	return player->GetWorldPosition();
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

	tileSizeX = width;
	tileSizeY = height;

	// 1차원 배열 할당
	tileMap = new char[tileSizeX * tileSizeY];
	memset(tileMap, ' ', tileSizeX * tileSizeY); // 공백으로 초기화
	
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
		if (x < tileSizeX && y < tileSizeY) 
		{
			tileMap[y * tileSizeX + x] = ch;
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
			int worldY = (int)player->GetWorldPosition().y - screenY;
			int worldX = (int)player->GetWorldPosition().x + screenX; // Y축은 아래로 갈수록 값이 줄어들어야 함

			// 타일링
			int tileX = (worldX % tileSizeX + tileSizeX) % tileSizeX;
			int tileY = (worldY % tileSizeY + tileSizeY) % tileSizeY;

			char tileChar[2] = { tileMap[tileY * tileSizeX + tileX], '\0' };

			Engine::Get().WriteToBuffer({ screenX, screenY }, tileChar, Color::Intensity, 0);
		}
	}
}