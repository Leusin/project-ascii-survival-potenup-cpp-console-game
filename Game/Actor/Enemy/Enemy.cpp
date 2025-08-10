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

	// 데미지 타이머 처리
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

	// 이동 
	MoveToPlayer(deltaTime);
	// 화면 래핑
	HandleScreenWrap();
}

void Enemy::Render()
{
	super::Render();
}

void Enemy::OnDestroy()
{
	TryToDropOrb();
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
	// 플레이어를 향해 이동(월드 좌표계)
	Vector2F playerPos = { (float)round(cameraPosition.x), (float)round(cameraPosition.y) };
	Vector2F toPlayer = playerPos - worldPosition;

	Vector2F movement = toPlayer.Normalize() * stats.speed * deltaTime;
	Vector2F nextWorldPosition = worldPosition + movement; // 다음 이동할 월드 위치

	// 다음 이동 위치 확인
	Vector2I nextScreenPos = Engine::Get().OrthogonalToScreenCoords(nextWorldPosition, cameraPosition); // 다음에 이동할 화면 위치

	for (Actor* actor : GetOwner()->GetActors())
	{
		// 플레이어가 있는 경우, 플레이어에게 데미지를 가함
		if (actor->As<Player>())
		{
			if (actor->Position() != nextScreenPos)
			{
				continue;
			}

			if (IDamageable* damageable = dynamic_cast<IDamageable*>(actor))
			{
				damageable->TakeDamage(stats.damage);
			}

			break;
		}

		// 적이 있는 경우, 이동하지 않음
		if (actor->As<Enemy>())
		{
			if (actor != this && actor->Position() == nextScreenPos)
			{
				return;

			}
		}
	}

	if (nextScreenPos != Engine::Get().ScreenCenter())
	{
		SetPosition(nextScreenPos);
		worldPosition = nextWorldPosition; // 새 월드 위치 적용
	}
}

void Enemy::HandleScreenWrap()
{
	Vector2I screenPosition = Position();

	int w = Engine::Get().Width();
	int h = Engine::Get().Height();

	Vector2I newScreenPos = Position();

	//
	// 좌우 화면 처리
	//
	if (screenPosition.x < 0) // 화면 왼쪽을 벗어남
	{
		newScreenPos.x = w;
	}
	else if (screenPosition.x >= w) // 화면 오른쪽을 벗어남
	{
		newScreenPos.x = 0;
	}

	//
	// 상하 화면 처리
	//
	else if (screenPosition.y < 0) // 화면 위쪽을 벗어났을 때 (y가 0보다 낮음)
	{
		newScreenPos.y = h;
	}
	else if (screenPosition.y > h) // 화면 아래쪽을 벗어났을 때 (y가 화면 높이 이상)
	{
		newScreenPos.y = 0;
	}

	//
	// 그려지는 화면 안에 있기 때문에 예외처리가 필요 없는 경우
	//
	else
	{
		return;
	}


	// 겹치는 적이 있는지 검사
	for (Actor* actor : GetOwner()->GetActors())
	{
		if (actor == this)
		{
			continue;
		}

		if (!actor->As<Enemy>())
		{
			continue;
		}

		if (actor->Position() == newScreenPos)
		{
			return;
		}
	}

	// 겹치는 적이 없을 때만 위치를 갱신
	worldPosition = Engine::Get().ScreenToOrthogonalCoords(newScreenPos, cameraPosition);
	SetPosition(newScreenPos);
}

void Enemy::TryToDropOrb()
{
	// 3분의 1 확률로 경험치 아이템 스폰하기
	int dropExpChance = Utils::Random(0, 2);
	if (dropExpChance == 0)
	{
		Vector2I spawnPos = { (int)round(worldPosition.x), (int)round(worldPosition.y) };
		GetOwner()->AddActor(new ExpOrb(spawnPos, cameraPosition, stats.exp));
		return;
	}

	// 10분의 1 확률로 경험치 아이템 스폰하기
	int dropHealChance = Utils::Random(0, 10);
	if (dropHealChance == 0)
	{
		Vector2I spawnPos = { (int)round(worldPosition.x), (int)round(worldPosition.y) };
		GetOwner()->AddActor(new HealOrb(spawnPos, cameraPosition, stats.exp));
		return;
	}


}