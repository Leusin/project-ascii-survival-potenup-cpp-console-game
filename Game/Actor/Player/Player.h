#pragma once

#include "Actor/Actor.h"

#include "Math/Vector2.h"
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
	const Vector2& GetWorldPosition() const;
	const Vector2& GetDirection() const;

private:
	/// <summary>
	/// 플레이어의 위치이자 카메라의 위치이기도 하다.
	///   영향받는 것.
	///   - 배경
	///   - 아이템
	/// </summary>
	Vector2 worldPosition;

	CharacterStats stats;
	Vector2 moveInput = { 0, 0 };

	// 마지막으로 이동한 방향
	// 투사체 의 방향이 될 것
	Vector2 direction = Vector2::Right;
};
