#pragma once

#include "Actor/Actor.h"

#include "Utils/Timer.h"
#include "Math/Vector2F.h"
#include "Stats/CharacterStats.h"
#include "Stats/EnemyStats.h"
#include "Interface/IDamageable.h"
class Enemy :public Actor, public IDamageable
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2I& cameraPostion, const EnemyStats& stats = EnemyStats{});
	virtual ~Enemy();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual void OnDestroy() override;


public: 
	virtual void TakeDamage(float damage) override; // IDamageable

public: 
	static inline unsigned int GetAliveCount() { return aliveCount; };

private:
	void SetRendomSpawnPosition();

	/// <summary>
	/// 플레이어를 향해 이동 및 이동 가능 검사, 플레이어 공격 
	/// </summary>
	/// <param name="deltaTime"></param>
	void MoveToPlayer(float deltaTime);
	
	/// <summary>
	/// 화면 이탈 확인 및 월드 좌표 조정
	/// </summary>
	void HandleScreenWrap();

	/// <summary>
	/// 아이템 드롭
	/// </summary>
	void TryToDropExpOrb();
	void TryToDropHealOrb();
private:
	EnemyStats stats;

	Vector2F worldPosition;
	const Vector2I& cameraPosition;

	bool isOnDamaged = false;
	Timer onDamagedTimer;
	const float invulnerableTime = 0.125f;
	Color onDamagedColor = Color::Intensity;

	static unsigned int aliveCount;
};
