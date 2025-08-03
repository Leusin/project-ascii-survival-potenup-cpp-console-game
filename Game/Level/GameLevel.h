#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"

class GameLevel : public Level

{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	Vector2 GetPlayerPosition() const;

private:
	void ReadTileMapFile(const char* filename);
	void RenderBackground();

private:
	// Note: Level에서 알아서 지워 줄 것이다. 메모리 관리하면 안됨
	class Player* player; 

	//TEST
	char* tileMap = nullptr;
	int tileSizeY = 0;
	int tileSizeX = 0;
};