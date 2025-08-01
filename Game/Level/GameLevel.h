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

private:
	// Note: Level에서 알아서 지워 줄 것이다. 메모리 관리하면 안됨
	class Player* player; 

	//TEST
	char map[8][10];
};