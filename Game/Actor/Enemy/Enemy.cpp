#include "Enemy.h"

#include "Engine.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Actor/Player/Player.h"
#include "Actor/Item/ExpOrb.h"
#include "Actor/Item/HealOrb.h"
#include "Interface/IDamageable.h"

unsigned int Enemy::aliveCount = 0;
unsigned int Enemy::kiledCount = 0;

Enemy::Enemy(const Vector2I& spawnPosition, const Vector2I& cameraPostion, const EnemyStats& stats)
	: Actor(stats.icon, stats.color, spawnPosition)
	, stats(stats)
	, cameraPosition(cameraPostion)
{
	worldPosition = Engine::Get().ScreenToOrthogonalCoords(spawnPosition, cameraPosition);

	SetSortingOrder(8);
	onDamagedTimer.SetTargetTime(invulnerableTime);

	aliveCount++;
}

Enemy::~Enemy()
{
	kiledCount++;
	aliveCount--;
}

void Enemy::BeginPlay()
{
	super::BeginPlay();

}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 데미지를 입는 중인지 검사
	if (isOnDamaged)
	{
		onDamagedTimer.Tick(deltaTime);

		if (onDamagedTimer.IsTimeout())
		{
			isOnDamaged = false;
			onDamagedTimer.Reset();
		}
	}

	color = isOnDamaged ? onDamagedColor : stats.color;


	HandleScreenWrap();
	MoveToPlayer(deltaTime);
}

void Enemy::Render()
{
	super::Render();
}

void Enemy::OnDestroy()
{
	TryToDropExpOrb();
	TryToDropHealOrb();
}

void Enemy::TakeDamage(float damage)
{
	isOnDamaged = true;

	stats.hp -= damage;
	if (stats.hp <= 0.f)
	{
		Destroy();
	}
}

void Enemy::MoveToPlayer(float deltaTime)
{
	// 월드 좌표계를 플레이어를 향해 이동하도록 조작한다.

	Vector2F playerPos = { (float)round(cameraPosition.x), (float)round(cameraPosition.y) };

	Vector2F toPlayer = playerPos - worldPosition;

	Vector2F movement = toPlayer.Normalize() * stats.speed * deltaTime;

	Vector2F nextPosition = worldPosition + movement; // 다음 이동할 월드 위치

	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);

	//
	// 다음 이동 위치 확인
	//

	Vector2I nextScreenPos = Engine::Get().OrthogonalToScreenCoords(nextPosition, cameraPosition); // 다음에 이동할 화면 위치

	std::vector<Actor*> actors = GetOwner()->GetActors();

	for (Actor* actor : actors)
	{
		// 플레이어가 있는 경우, 플레이어에게 데미지를 가함
		if (actor->As<Player>())
		{
			if (actor->Position() == nextScreenPos)
			{
				IDamageable* playerDamageable = dynamic_cast<IDamageable*>(actor);

				if (playerDamageable)
				{
					playerDamageable->TakeDamage(stats.damage);
				}

				break;
			}
		}

		//
		// 적이 있는 경우, 이동하지 않음
		//

		if (!actor->As<Enemy>())
		{
			continue;
		}

		if (actor == this)
		{
			continue;
		}

		if (actor->Position() == nextScreenPos)
		{
			return;
		}
	}

	if (nextScreenPos != Engine::Get().ScreenCenter())
	{
		worldPosition = nextPosition; // 새 월드 위치 적용
		SetPosition(nextScreenPos); // 새 화면 위치 적용
	}
}

void Enemy::HandleScreenWrap()
{
	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);

	/// 좌우 화면 처리

	// 화면 왼쪽을 벗어남 -> 월드 좌표를 화면 너비만큼 오른쪽으로 이동
	if (screenPosition.x < 0)
	{
		worldPosition.x += Engine::Get().Width();
	}
	// 화면 오른쪽을 벗어남 -> 월드 좌표를 화면 너비만큼 왼쪽으로 이동
	else if (screenPosition.x >= Engine::Get().Width() + 1)
	{
		worldPosition.x -= Engine::Get().Width();
	}

	/// 상하 화면 처리

	if (screenPosition.y < 0)
	{
		worldPosition.y -= Engine::Get().Height(); // 월드 좌표를 화면 높이만큼 아래로 이동
	}
	// 화면 아래쪽을 벗어났을 때 (y가 화면 높이 이상)
	else if (screenPosition.y >= Engine::Get().Height() + 1)
	{
		worldPosition.y += Engine::Get().Height(); // 월드 좌표를 화면 높이만큼 위로 이동
	}

	screenPosition = Engine::Get().OrthogonalToScreenCoords(worldPosition, cameraPosition);
	SetPosition(screenPosition);
}

void Enemy::TryToDropExpOrb()
{
	// 3분의 1 확률로 경험치 아이템 스폰하기
	int dropChance = Utils::Random(0, 2);
	if (dropChance != 0)
	{
		return;
	}

	Vector2I spawnPos = { (int)round(worldPosition.x), (int)round(worldPosition.y) };
	GetOwner()->AddActor(new ExpOrb(spawnPos, cameraPosition, stats.exp));
}

void Enemy::TryToDropHealOrb()
{
	// 30분의 1 확률로 경험치 아이템 스폰하기
	int dropChance = Utils::Random(0, 29);
	if (dropChance != 0)
	{
		return;
	}

	Vector2I spawnPos = { (int)round(worldPosition.x), (int)round(worldPosition.y) };
	GetOwner()->AddActor(new HealOrb(spawnPos, cameraPosition, stats.exp));
}