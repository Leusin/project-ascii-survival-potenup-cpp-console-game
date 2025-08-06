#pragma once

#include "Actor/Actor.h"

#include <vector>
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

public:
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

	/// <summary>
	/// 체력 회복
	/// </summary>
	void AddHp(float amount);

private:
	void ProcessDamaged(float deltaTime);

	/// <summary>
	/// 레벨에 비례하여 단순 체력이 증가
	/// </summary>
	/// <returns></returns>
	float CalculateMaxHp() const;

	/// <summary>
	/// 레벨과 성장 증가량을 바탕으로 계산
	/// </summary>
	float CalculateMaxExp() const;

public: // GetterSetter
	const Vector2I& GetCameraPosition() const;
	const Vector2F& GetDirection() const;

public:
	// 플레이어 무기 목록
	std::vector<class Weapon*> weapons;

private:
	/// <summary>
	/// 플레이어의 위치이자 카메라의 위치이기도 하다.
	///   영향받는 것.
	///	  - 적의 위치
	///   - 배경
	///   - 아이템
	/// </summary>
	Vector2F worldPosition; // 이동을 계산할 때 쓰는 것
	Vector2I cameraPosition; // 다른 객체들은 이걸 참조한다.

	//
	// 입력 처리 관련
	//
	Vector2F moveInput = Vector2F::Zero;
	Vector2F direction = Vector2F::Right; // 마지막으로 이동한 방향

	//
	// 데미지 처리 관련
	//
	bool isOnDamaged = false;
	Timer onDamagedTimer;
	float invulnerableTime = 0.1f;

	//
	// 레벨
	//
	unsigned int level = 1;

	//
	// 이동 속도
	//
	float speed = 5.f;

	//
	// 경험치 관련
	//
	const float baseExp = 5.f;
	float growthRate = 1.1f; // 레벨업에 필요한 경험치 증가율
	float currentExp = 0.f;

	//
	// 체력 관련
	//
	const float baseHp = 20.f;
	float currentHp = 0.0f;
	float hpPerLevel = 1.1f;
};
