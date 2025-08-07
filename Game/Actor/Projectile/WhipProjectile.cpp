#include "WhipProjectile.h"

#include <vector>
#include "Engine.h"
#include "level/GameLevel.h"
#include "Actor/Enemy/Enemy.h"

WhipProjectile::WhipProjectile(float damage, unsigned int length, const Vector2I Position)
	: Actor("~", Color::LightWhite, Position)
	, damage(damage)
	, length(length)
{
	SetSortingOrder(5);

	char* str = new char[length + 1];
	for (unsigned int i = 0; i <length; ++i)
	{
		str[i] = GetImage()[0];
	}
	str[length] = '\0';

	SetImage(str);
	SafeDeleteArray(str);
}

void WhipProjectile::BeginPlay()
{
	super::BeginPlay();

	SetLifetime(1.f); // 무조건 1초동안
}

void WhipProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);


	// 현재 위치에 적이 있는지 검사 
	std::vector<Actor*> actors = GetOwner()->GetActors();

	for (Actor* actor : actors)
	{
		if (!actor->As<Enemy>())
		{
			continue;
		}

		//
		// AABB 검사
		//

		auto enemyPos = actor->Position();
		auto currentPos = Position();

		if (enemyPos.y != currentPos.y)
		{
			continue;
		}
		// 적이 같은 y 열에 있고

		// 내 위치 x 보다 크거나 같고 x + length 보다 작거나 같은면
		if (currentPos.x <= enemyPos.x && (currentPos.x + (int)length - 1) >= enemyPos.x)
		{
			// 데미지 처리
			IDamageable* enemyDamageable = dynamic_cast<IDamageable*> (actor);
			if (enemyDamageable)
			{
				enemyDamageable->TakeDamage(damage);
			}
		}
	}

}

void WhipProjectile::Render()
{
	super::Render();
}
