#pragma once

#include <map>
#include "Enemy.h"
#include "Utils\Utils.h"
#include "Math/Vector2I.h"
#include "Stats/EnemyStats.h"

enum class EnemyType 
{
    Basic = 0,
    Fast = 1,
    Mumak = 2,
    // ... 새로운 적 종류 추가

    SIZE // 겟수 세기
};

class EnemyFactory
{
public:
    class Enemy* CreateEnemy(const Vector2I& cameraPosition, EnemyType type = EnemyType::Basic)
    {
        EnemyStats stats = enemyStatsMap.at(type); // 맵에서 해당 타입의 stats를 찾아 Enemy를 생성
        return new Enemy(cameraPosition, stats);
    }

    class Enemy* CreateRandomEnemy(const Vector2I& cameraPosition)
    {
        int randomInt = Utils::Random(0, static_cast<int>(EnemyType::SIZE) - 1);
        EnemyType randomType = static_cast<EnemyType>(randomInt);

        return CreateEnemy(cameraPosition, randomType);
    }

    EnemyFactory()
    {
        // 모든 적 타입의 stats를 미리 정의합니다.
        enemyStatsMap[EnemyType::Basic] = EnemyStats{ };
        enemyStatsMap[EnemyType::Fast] = { "F", Color::Cyan, 3.0f, 3.0f, 0.5f, 5.0f };
        enemyStatsMap[EnemyType::Mumak] = { "M", Color::Yellow, 15.0f, 0.8f, 2.0f, 10.f};
    }
private:
    std::map<EnemyType, EnemyStats> enemyStatsMap;
};