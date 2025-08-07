#pragma once

#include <map>
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
    class Enemy* CreateEnemy(const Vector2I& cameraPosition, EnemyType type = EnemyType::Bat)
    {
        EnemyStats stats = enemyStatsMap.at(type); // 맵에서 해당 타입의 stats를 찾아 Enemy를 생성
        return new Enemy(cameraPosition, stats);
    }

    class Enemy* CreateRandomEnemy(const Vector2I& cameraPosition, int min = 0, int max = static_cast<int>(EnemyType::SIZE) - 1)
    {
        if (min > static_cast<int>(EnemyType::SIZE) - 1)
        {
            min = static_cast<int>(EnemyType::SIZE) - 1;
        }
        if (min > max)
        {
            std::swap(min, max); // 안전하게 스왑
        }

        int randomInt = Utils::Random(min, max);
        EnemyType randomType = static_cast<EnemyType>(randomInt);

        return CreateEnemy(cameraPosition, randomType);
    }

    EnemyFactory()
    {
        // 모든 적 타입의 stats를 미리 정의합니다.
        enemyStatsMap[EnemyType::Basic] = EnemyStats{};
        enemyStatsMap[EnemyType::Bat] = { "b", Color::LightBlue, 6.0f, 1.5f, 0.02f, 1.0f };
        enemyStatsMap[EnemyType::Slime] = { "s", Color::Green, 8.0f, 0.8f, 1.0f, 3.0f };
        enemyStatsMap[EnemyType::Goblin] = { "g", Color::Yellow, 10.0f, 1.8f, 0.05f, 6.0f };
        enemyStatsMap[EnemyType::Golem] = { "G", Color::Blue, 20.0f, 1.0f, 0.25f, 8.0f };
        enemyStatsMap[EnemyType::Mumak] = { "M", Color::Yellow, 35.0f, 2.8f, 2.0f, 13.f};
        enemyStatsMap[EnemyType::EpicKnight] = { "K", Color::LightMagenta, 100.0f, 4.0f, 1.0f, 15.0f };
    }

private:
	std::map<EnemyType, EnemyStats > enemyStatsMap;
};