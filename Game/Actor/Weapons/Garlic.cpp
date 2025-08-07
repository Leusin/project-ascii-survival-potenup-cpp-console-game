#include "Garlic.h"

#include "Engine.h"
#include "level/GameLevel.h"
#include "Actor/Enemy/Enemy.h"

Garlic::Garlic()
	: Weapon("0", "Garlic", Vector2I::Zero)
{
	stats.currentLevel = 0;
	stats.baseDamaged = 6.5f;
	stats.area = 10;
	stats.cooldown = 1.3f;
	stats.projectileInterval = 0.1f;

	cooldownTimer.SetTargetTime(stats.cooldown);

	SetSortingOrder(2);

	upgradeDescription =
	{
		"Attacks horizontally",
		"Increases damage and adds one more projectile",
		"Increases damage and range",
		"Increases damage, range, and speed"
	};
}

void Garlic::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 0 레벨이라면 활성하지 않음
	if (stats.currentLevel < 1)
	{
		return;
	}

	cooldownTimer.Tick(deltaTime);

	if (!cooldownTimer.IsTimeout())
	{
		return;
	}

	cooldownTimer.Reset();
	cooldownTimer.SetTargetTime(stats.cooldown);

	//
	// 여기서 적 범위 검사하고 데미지처리
	//

	// 현재 위치에 적이 있는지 검사 
	std::vector<Actor*> actors = GetOwner()->GetActors();

	for (Actor* actor : actors)
	{
		if (!actor->As<Enemy>())
		{
			continue;
		}

		Vector2I enemyPos = actor->Position();
		Vector2I center = Engine::Get().ScreenCenter(); // 타원 중심
		Vector2I relative = enemyPos - center;

		int r = GetRadious();
		int a = r; // 가로 반지름
		int b = static_cast<int>(r / ratio); // 보정된 세로 반지름

		int aa = a * a;
		int bb = b * b;
		int xx = relative.x * relative.x;
		int yy = relative.y * relative.y;

		// 결정값
		int p = bb * xx + aa * yy - aa * bb;

		// p(x, y) > 0 : 점이 타원 외부에 있다
		if (p > 0)
		{
			continue;
		}
		// p(x, y) < 0 : 점이 타원 내부에 있다
		// p(x, y) = 0 : 점이 타원 위에 있다


		IDamageable* enemyDamageable = dynamic_cast<IDamageable*> (actor);
		if (enemyDamageable)
		{
			enemyDamageable->TakeDamage(stats.baseDamaged);
		}
	}
}

void Garlic::Render()
{
	/*
		타원을 그리는 방법이 여러가지 있는데
		그 중 Midpoint Ellipse 알고리즘을 사용할 것이다.
		판별 방식과 소수연산을 쓰지 않고 타원을 그릴 수 있는 것이
		마치 브레젠험 직선 알고리즘의 그것과 비슷하다.

		알고리즘 기본 원리:
		타운의 방정식은 다음과 같다.

		x^2 / a^2 + t^2 / b^2 = 1
		a = 타원의 장반경(x축)
		b = 타원의 단반경(y축)

		다음 함수로 점을 결정한다.
		p(x, y) = b^2 * x^2 + a^2 * y^2 - a^2 - b^2

		p(x, y) < 0 : 점이 타원 내부에 있다 <- 이걸로 피격 판정하면 된다
		p(x, y) > 0 : 점이 타원 외부에 있다
		p(x, y) = 0 : 점이 타원 위에 있다


		구현 방법
		1. 타원의 1/4분면에서 시작 (0, b)
		2. 판별 변수 p 를 설정한다.
		3. 반복문: p의 부호를 보고 다음 점을 결정한다.
			i. p < 0 이면 오른 쪽으로 이동: x++
			ii.p >=0 이면 오른 쪽 아래로 이동: x++, y--;
		4. 점 찍기: 계산된 모든 점을 대칭적으로 복사하여 8방향(x, y, -x, y, x, -y, -x, -y 등)에 점을 찍기
	*/
	// 콘솔 폰트 비율 보정 (가로 : 세로 = radious : radious * ratio로 가정)
	int r = GetRadious();
	int a = r; // 가로 반지름
	int b = static_cast<int>(r / ratio); // 보정된 세로 반지름
	
	// 대칭점 그리기 람다
	// 4분면을 한번에 그린다.
	auto drawEllipsePoints = [&](int currentX, int currentY)
		{
			const char* image = GetImage();
			int sortingOrder = GetSortingOrder();
			Vector2I center = Engine::Get().ScreenCenter();

			Engine::Get().WriteToBuffer(center + Vector2I{ currentX, currentY }, image, color, sortingOrder);
			Engine::Get().WriteToBuffer(center + Vector2I{ -currentX, currentY }, image, color, sortingOrder);
			Engine::Get().WriteToBuffer(center + Vector2I{ currentX, -currentY }, image, color, sortingOrder);
			Engine::Get().WriteToBuffer(center + Vector2I{ -currentX, -currentY }, image, color, sortingOrder);
		};

	int x = 0;
	int y = b;
	
	//
	// 타원 알고리즘 Region 1
	// 기울기가 -1보다 크거나 같은 구간
	// x를 증가시키면서 y 결정
	// 
	
	int bb = b * b;
	int aa = a * a;

	// p(x, y) = b^2 * x^2 + a^2 * y^2 - a^2 * b-b^2
	int p1 = bb - aa * b + aa / 4;
	while (2 * bb * x <= 2 * aa * y)
	{
		drawEllipsePoints(x, y);
		if (p1 < 0)
		{
			p1 += 2 * bb * x + bb;
		}
		else
		{
			y--;
			p1 += 2 * bb * x - 2 * aa * y + bb;
		}
		x++;
	}

	//
	// 타원 알고리즘 Region 2
	// 기울기가 -1보다 작은 구간
	// y를 감소시키면서 x 결정
	//
	
	int p2 = bb * x * x + aa * (y - 1) * (y - 1) - aa * bb;
	while (y >= 0)
	{
		drawEllipsePoints(x, y);
		if (p2 > 0)
		{
			p2 += -2 * aa * y + aa;
		}
		else
		{
			x++;
			p2 += 2 * bb * x - 2 * aa * y + aa;
		}
		y--;
	}

}

void Garlic::LevelUp()
{
	super::LevelUp();

	if (stats.currentLevel == 1)
	{
		stats.baseDamaged = 1.f;
		stats.area = 60;
	}
	else if (stats.currentLevel == 2)
	{
		stats.area = 60;
	}
	else if (stats.currentLevel == 3)
	{
		// 공격력 1 증가, 범위 10% 증가
		stats.area = 60;
	}
	else if (stats.currentLevel == 4)
	{
		// 공격력 1 증가, 범위 10% 증가
		stats.area = 100;
	}
}

int Garlic::GetRadious()
{
	return (int)(totalRadious * (float)stats.area / 100);
}
