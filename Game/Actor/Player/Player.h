#pragma once

#include "Actor/Actor.h"

#include "Utils/Timer.h"
#include "Math/Vector2F.h"
#include "Stats/CharacterStats.h"
#include "Interface/IDamageable.h"

class Player : public Actor, public IDamageable
{
	RTTI_DECLARATIONS(Player, Actor)

public:// 생성자, 오버라이드
	Player();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

public: // Interface 구현
	virtual void TakeDamage(float damage) override;

public: // GetterSetter
	const Vector2F& GetWorldPosition() const;
	const Vector2F& GetDirection() const;

	/// <summary>
	/// 현재 hp 와 최대 hp 의 비율. UI 에 사용
	/// </summary>
	/// <returns></returns>
	float GetHpRatio() const;

	/// <summary>
	/// 현재 경험치와 다음 레벨로 가는 경험치 비율 반환. UI 에 사용
	/// </summary>
	float GetExpRatio() const;

	/// <summary>
	/// 경험치 추가 밎 레벨업 검사
	/// </summary>
	void AddExp(float exp);

private:
	void ProcessDamaged(float deltaTime);

	/// <summary>
	/// 경험치 관련 데이터를 바탕으로 다음레벨 경험치를 계산
	/// </summary>
	float CalculateMaxExp() const;

private:
	/// <summary>
	/// 플레이어의 위치이자 카메라의 위치이기도 하다.
	///   영향받는 것.
	///	  - 적의 위치
	///   - 배경
	///   - 아이템
	/// </summary>
	Vector2F worldPosition;

	CharacterStats stats;
	Vector2F moveInput = { 0, 0 };

	// 마지막으로 이동한 방향
	// 투사체 의 방향이 될 것
	Vector2F direction = Vector2F::Right;

	//
	// 데미지 처리 관련
	//
	float baseMaxHp;
	bool isOnDamaged = false;
	Timer onDamagedTimer;
	float invulnerableTime = 0.1f;

	//
	// 경험치 관련
	//
	float currentExp = 0.f;
	const float baseExp = 10.f;
	const float growthRate = 1.1f; // 레벨업에 필요한 경험치 증가율
};
