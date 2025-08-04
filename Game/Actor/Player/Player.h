#pragma once

#include "Actor/Actor.h"

#include "Math/Vector2F.h"
#include "Stats/CharacterStats.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

public:
	const Vector2F& GetWorldPosition() const;
	const Vector2F& GetDirection() const;

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
};
