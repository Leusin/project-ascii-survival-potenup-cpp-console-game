#pragma once

#include "Actor/Actor.h"

/// <summary>
/// 화면 밖으로 나가거나 적을 부디칠 때 까지 
/// 처음 발사될 때 가장 가까준 엊을 향해 나아간다.
/// </summary>
class WagicWandProjectile :public Actor
{
	RTTI_DECLARATIONS(WagicWandProjectile, Actor)

public:
	WagicWandProjectile(float damage, float speed, const Vector2I& cameraPosition, const Vector2I direiction);

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	float speed;
	float damage;

	/// <summary>
	/// 투척 방향
	/// </summary>
	Vector2F direction = Vector2F::Right;

	const Vector2I& cameraPosition; // 플레이어의 위치 (월드 좌표계에서)

	/// <summary>
	/// 투사체의 월드 위치
	/// </summary>
	Vector2F projectilePosition = Vector2F::Zero;
};