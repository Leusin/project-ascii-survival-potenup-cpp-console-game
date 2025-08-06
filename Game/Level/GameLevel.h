#pragma once

#include "Level/Level.h"
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

public:

private:
	void ReadTileMapFile(const char* filename);

	//
	// Render 관련
	//
	void RenderBackground(); 
	void RenderUI();

	// 디버그 관련 처리
	void ProcessDebuge();
	void RenderDebugeData();

private:
	class Player* player; 
	EnemyFactory enemyFactory;

	// 타일 맵 데이터
	char* tileMap = nullptr;
	int tileHeight = 0; // 너비
	int tileWidth = 0; // 높이
};