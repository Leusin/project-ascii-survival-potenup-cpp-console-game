#pragma once

#include <map>
#include <vector>
#include "Enemy.h"
#include "Utils\Utils.h"
#include "Math/Vector2I.h"
#include "Stats/EnemyStats.h"

enum class EnemyType 
{
    Basic = 0,
    Bat = 1,
    Slime = 2,
    Goblin = 3,
    Golem,
    Mumak,
    EpicKnight,

    SIZE // 겟수 세기
};

class EnemyFactory
{
public:
    EnemyFactory(std::vector<class Actor*>& actors);

    Enemy* CreateEnemy(const Vector2I& cameraPosition, EnemyType type = EnemyType::Bat);

    Enemy* CreateRandomEnemy(const Vector2I& cameraPosition, int min = 0, int max = static_cast<int>(EnemyType::SIZE) - 1);

private:
    Vector2I CheckRendomSpawnPosition();

    Vector2I nullPos;

private:
	std::map<EnemyType, EnemyStats > enemyStatsMap;
    std::vector<class Actor*>& actors;
};