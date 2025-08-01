#pragma once

#include "Level/Level.h"


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
	// Note: Level에서 알아서 지워 줄 것이다.
	class Actor* player; 
};