#pragma once

#include "Actor/Actor.h"

#include "Utils/Timer.h"
#include "Math/Vector2F.h"
#include "Stats/CharacterStats.h"
#include "Interface/IDamageable.h"

class Enemy :public Actor, public IDamageable
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2I& cameraPostion);
	virtual ~Enemy();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual void OnDestroy() override;

public: 
	virtual void TakeDamage(float damage) override; // IDamageable

	static unsigned int count;
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

private:
	CharacterStats stats;

	Vector2F worldPosition;
	const Vector2I& cameraPosition;

	Color renderColor;

	bool isOnDamaged = false;
	Timer onDamagedTimer;
	float onDamagedTargetTime = 0.25f;

};
