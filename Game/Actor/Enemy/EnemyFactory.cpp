#include "EnemyFactory.h"

#include "Engine.h"
#include "Actor/Actor.h"

EnemyFactory::EnemyFactory(std::vector<class Actor*>& actors)
	: actors(actors)
	, nullPos(Engine::Get().ScreenCenter())
{
	// 모든 적 타입의 stats를 미리 정의합니다.
	enemyStatsMap[EnemyType::Basic] = EnemyStats{};
	enemyStatsMap[EnemyType::Bat] = { "b", Color::LightBlue, 6.0f, 3.5f, 0.12f, 1.0f };
	enemyStatsMap[EnemyType::Slime] = { "s", Color::Green, 8.0f, 2.8f, 1.0f, 3.0f };
	enemyStatsMap[EnemyType::Goblin] = { "g", Color::Yellow, 10.0f, 4.0f, 2.0f, 6.0f };
	enemyStatsMap[EnemyType::Golem] = { "G", Color::Blue, 20.0f, 4.0f, 4.0f, 8.0f };
	enemyStatsMap[EnemyType::Mumak] = { "M", Color::Yellow, 35.0f, 1.5f, 6.0f, 13.f };
	enemyStatsMap[EnemyType::EpicKnight] = { "K", Color::LightMagenta, 100.0f, 4.5f, 8.0f, 15.0f };
};

Enemy* EnemyFactory::CreateEnemy(const Vector2I& cameraPosition, EnemyType type)
{
	Vector2I spawnPostion = CheckRendomSpawnPosition();

	if (spawnPostion == nullPos)
	{
		return nullptr;
	}

	EnemyStats stats = enemyStatsMap.at(type); // 맵에서 해당 타입의 stats를 찾아 Enemy를 생성
	Enemy* newEnemy = new Enemy(spawnPostion, cameraPosition, stats);
	return newEnemy;
}

Enemy* EnemyFactory::CreateRandomEnemy(const Vector2I& cameraPosition, int min, int max)
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

Vector2I EnemyFactory::CheckRendomSpawnPosition()
{
	Vector2I spawnPostion;

	int PosType = Utils::Random(0, 3);
	if (PosType == 0)
	{
		// 위 쪽이라면 (x, 1)
		spawnPostion = { Utils::Random(0, Engine::Get().Width() - 1) , 0 };
	}
	else if (PosType == 1)
	{
		// 왼 쪽이라면 (1, y)
		spawnPostion = { 0, Utils::Random(1, Engine::Get().Height() - 1) };
	}
	else if (PosType == 2)
	{
		// 아래 쪽이라면 (x, hight)
		spawnPostion = { Utils::Random(0, Engine::Get().Width() - 1) , Engine::Get().Height() - 1 };
	}
	else if (PosType == 3)
	{
		// 오른 쪽이라면 (width, y)
		spawnPostion = { Engine::Get().Width() - 1, Utils::Random(0, Engine::Get().Height() - 1) };
	}

	for (Actor* actor : actors)
	{
		if (actor->As<Enemy>())
		{
			if (actor->Position() == spawnPostion)
			{
				return nullPos;
			}
		}
	}

	return spawnPostion;
};