#pragma once

#include "Actor/Actor.h"

#include "Stats/CharacterStats.h"
#include "Math/Vector2F.h"

class Enemy :public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(Vector2F& cameraPostion);
	//virtual ~Enemy();

	//virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	virtual void OnDestroy() override;

private:
	void SetSpawnPosition();

	void MoveToPlayer(float deltaTime);
	
	/// <summary>
	/// 화면 이탈 확인 및 월드 좌표 조정
	/// </summary>
	void HandleScreenWrap();

private:
	CharacterStats stat;

	Vector2F worldPosition;
	Vector2F& playerPosition;
};
