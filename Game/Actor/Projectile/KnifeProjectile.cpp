#include "KnifeProjectile.h"

#include <vector>
#include "Engine.h"
#include "level/GameLevel.h"
#include "Actor/Enemy/Enemy.h"

KnifeProjectile::KnifeProjectile(float damage, float speed, const Vector2I& cameraPosition, Vector2I& direiction, const Vector2I& offset)
	: Actor("-", Color::LightWhite, { Engine::Get().Width() / 2, Engine::Get().Height() / 2 })
	, speed(speed)
	, damage(damage)
	, direction(direiction)
	, cameraPosition(cameraPosition)
	, projectilePosition(cameraPosition + direiction + offset)
{
	SetSortingOrder(6);

	// 방향에 따라 다른 아스키 표시
	SetImafeByDirection();
}

void KnifeProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 다음 위치 계산
	Vector2F movement = direction.Normalize() * speed * deltaTime; // 이동할 크기
	Vector2F nextPosition = projectilePosition + movement; // 이동될 위치
	Vector2I nextScreenPos = Engine::Get().OrthogonalToScreenCoords(nextPosition, cameraPosition); // 다음에 이동할 화면 위치

	// 계산위치 실제로 적용
	projectilePosition = nextPosition;

	// 화면 좌표계로 변환
	Vector2I screenPosition = Engine::Get().OrthogonalToScreenCoords(projectilePosition, cameraPosition);

	// 화면 좌표계 가장자리에 있을 경우 제거
	// 1과 2를 더한 건 문자 크기까지 고려해서
	if (screenPosition.x + 1 < 0 || screenPosition.x + 2 > Engine::Get().Width() ||
		screenPosition.y + 1 < 0 || screenPosition.y + 2 > Engine::Get().Height())
	{
		Destroy();
	}

	// 화면 좌표계에 실제로 적용
	SetPosition(screenPosition);

	//
	// 현재 위치에 적이 있는지 검사 
	// 
	std::vector<Actor*> actors = GetOwner()->GetActors();

	for (Actor* actor : actors)
	{
		if (!actor->As<Enemy>())
		{
			continue;
		}

		if (actor->Position() != screenPosition)
		{
			continue;
		}

		// 데미지 처리
		IDamageable* enemyDamageable = dynamic_cast<IDamageable*> (actor);
		if (enemyDamageable)
		{
			enemyDamageable->TakeDamage(damage);
			Destroy();
		}
	}
}

void KnifeProjectile::Render()
{
	super::Render();
}

void KnifeProjectile::SetImafeByDirection()
{
	// 우측 상단
	if (direction.x > 0 && direction.y < 0)
	{
		SetImage("\\");
	}
	// 좌측 상단
	else if (direction.x < 0 && direction.y < 0)
	{
		SetImage("/");
	}
	// 우측 하단
	else if (direction.x > 0 && direction.y > 0)
	{
		SetImage("/");
	}
	// 좌측 하단
	else if (direction.x < 0 && direction.y > 0)
	{
		SetImage("\\");
	}

	/// 상하좌우 방향을 나중에 판별

	// 우측 방향
	else if (direction.x > 0.1f && direction.y > -0.1f && direction.y < 0.1f)
	{
		SetImage("-");
	}
	// 좌측 방향
	else if (direction.x < -0.1f && direction.y > -0.1f && direction.y < 0.1f)
	{
		SetImage("-");
	}
	// 위쪽 방향 (y축이 위로 갈수록 커지는 직교 좌표계 기준)
	else if (direction.y > 0.1f && direction.x > -0.1f && direction.x < 0.1f)
	{
		SetImage("|");
	}
	// 아래쪽 방향 (y축이 아래로 갈수록 커지는 좌표계 기준)
	else if (direction.y < -0.1f && direction.x > -0.1f && direction.x < 0.1f)
	{
		SetImage("|");
	}
}
