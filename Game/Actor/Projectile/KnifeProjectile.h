#pragma once

#include "Actor/Actor.h"

/// <summary>
/// 화면 밖으로 나가거나 적을 부디칠 때 까지 
/// 지정된 방향으로 나아간다.
/// </summary>
class KnifeProjectile :public Actor
{
	RTTI_DECLARATIONS(KnifeProjectile, Actor)

public:
	KnifeProjectile(float damage, float speed, const Vector2I& cameraPosition, Vector2F& direiction);

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void SetImafeByDirection();

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