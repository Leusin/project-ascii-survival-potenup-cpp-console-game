#pragma once

#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Utils/DebugManager.h"
#include "Math/Vector2F.h"
#include "Actor/Enemy/EnemyFactory.h"

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void ReadTileMapFile(const char* filename);

	// 게임 레벨
	void SpawnEnemyMainWave(); // 시간 기반
	void SpawnFillerEnemys(); // 수량 기반

	//
	// Render 관련
	//
	void RenderBackground(); 
	void RenderTimer();
	void RenderUI();

	// 디버그 관련 처리
	void ProcessDebuge();
	void RenderDebugeData();

private:
	class Player* player; 
	EnemyFactory enemyFactory;
	
	// 게임 타이머
	//Timer gameTimer;
	int targetTime = 300;

	// 타일 맵 데이터
	char* tileMap = nullptr;
	int tileHeight = 0; // 너비
	int tileWidth = 0; // 높이

	// 게임 레벨 관련
	unsigned int levelWave = 0;

	DebugManager debugManager;
};