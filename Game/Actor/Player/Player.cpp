#include "Player.h"

#include<cmath>
#include "Engine.h"
#include "Input.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Actor/Item/Item.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Weapons/Knife.h"
#include "Actor/Weapons/MagicWand.h"
#include "Actor/Weapons/Whip.h"
#include "Actor/Weapons/Garlic.h"

Player::Player()
	: Actor("@", Color::White)
	, worldPosition(Position())
	, level(1)
	, speed(5.0f)
	// 경험치 관련
	, baseExp(5.0f)
	, growthRate(1.1f)
	, currentExp(0.0f)
	// 체력 관련
	, baseHp(20.0f)
	, currentHp(0.0f)
	, hpPerLevel(1.1f)
{
	currentHp = CalculateMaxHp();

	SetSortingOrder(10);

	SetPosition(Engine::Get().ScreenCenter());

	onDamagedTimer.SetTargetTime(invulnerableTime);
}

void Player::BeginPlay()
{
	super::BeginPlay();

	//
	// 무기 등록
	//

	// 칼
	weapons.emplace_back(new Knife(cameraPosition, direction));
	GetOwner()->AddActor(weapons.back());
	weapons.back()->LevelUp(); // 기본 무기

	// 마법 지팡이
	weapons.emplace_back(new MagicWand(cameraPosition));
	GetOwner()->AddActor(weapons.back());

	// 채찍
	weapons.emplace_back(new Whip(cameraPosition, direction));
	GetOwner()->AddActor(weapons.back());

	// 마늘
	weapons.emplace_back(new Garlic());
	GetOwner()->AddActor(weapons.back());
	weapons.back()->LevelUp(); // TEST
}

void Player::Tick(float deltaTime)
{
	ProcessDamaged(deltaTime); // 플레이어가 데이미지를 입었을 경우 처리(무적 시간)

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	Vector2I moveInput = Vector2I::Zero;

	// 직교 좌표계로 입력 받음
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) { moveInput.y += 1; }; // 위로 이동하면 Y +
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) { moveInput.y += -1; }; // 아래로 이동하면 Y-
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) { moveInput.x += -1; };
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) { moveInput.x += 1; };

	// 입력이 있을 때만 이동 처리
	if (moveInput != Vector2I::Zero)
	{
		// 마지막으로 보고 있었던 방향 업데이트
		direction = moveInput;

		Vector2F moveDirection = moveInput; // 이동 방향

		Vector2F movement = moveDirection.Normalize() * speed * deltaTime; // 이동 거리

		Vector2F nextPosition = worldPosition + movement; // 다음 이동할 월드 위치

		//
		// 이동 위치 검사. 적이 있다면 이동해서는 안됨
		//
		
		// 충돌 검사
		bool canMove = true;
		
		Vector2I ScreenMove = moveInput;
		ScreenMove.y *= -1;
		Vector2I nextTilePos = Engine::Get().ScreenCenter() + ScreenMove;

		std::vector<Actor*> actors = GetOwner()->GetActors();
		for (Actor* actor : actors)
		{
			if (actor->As<Item>())
			{
				if (actor->Position() != nextTilePos)
				{
					continue;
				}

				Item* itme = dynamic_cast<Item*>(actor);

				if (itme)
				{
					itme->OnCollected(this);
				}

				continue;
			}

			// 적이 있는 경우, 이동하지 않음
			if (!actor->As<Enemy>())
			{
				continue;
			}

			if (actor == this)
			{
				continue;
			}

			// 이동하려는 경로에 타일에 적이 있는지
			if (actor->Position() == nextTilePos)
			{
				canMove = false; // 충돌 발생
				break; // 루프 종료
			}
		}

		// 이동 위치 업데이트
		if (canMove)
		{
			worldPosition = nextPosition;
			cameraPosition.x = (int)round(worldPosition.x);
			cameraPosition.y = (int)round(worldPosition.y);
		}
	}
}

void Player::Render()
{
	super::Render();

	color = isOnDamaged ? Color::Red : Color::White;
	color = isInvincible ? Color::LightYellow : Color::White;
}

void Player::TakeDamage(float damage)
{
	if (isOnDamaged || isInvincible)
	{
		return;
	}

	isOnDamaged = true;

	float damaged = currentHp - damage;

	if (damaged < 0.0f)
	{
		hasDead = true;
		currentHp = 0;
	}
	else
	{
		currentHp = damaged;
	}
}

const Vector2I& Player::GetCameraPosition() const
{
	return cameraPosition;
}

const Vector2F& Player::GetDirection() const
{
	return direction;
}

float Player::GetHpRatio() const
{
	return currentHp / CalculateMaxHp();
}

float Player::GetExpRatio() const
{
	return currentExp / CalculateMaxExp();
}

void Player::AddExp(float exp)
{
	currentExp += exp;

	// 레벨업이 가능한지 검사
	float toNextLevel = CalculateMaxExp();
	if (currentExp < toNextLevel) // 레벨업 불가 예외 처리
	{
		return;
	}

	// 레벨업 처리
	currentExp -= toNextLevel;
	++level;


	// 50%  확률로 레벨업 이벤트 발생
	int random = Utils::Random(0, 1);
	if (random == 0)
	{
		// 레벨업 이벤트
		Game::Get().GoToUpgradeLevel(weapons);
	}
}

void Player::AddHp(float amount)
{
	float currentMaxhp = CalculateMaxHp();
	currentHp = (currentHp + amount) > currentMaxhp ? currentMaxhp : (currentHp + amount);
}

void Player::ProcessDamaged(float deltaTime)
{
	// 데미지를 입는 중인지 검사
	if (isOnDamaged)
	{
		onDamagedTimer.Tick(deltaTime);

		if (onDamagedTimer.IsTimeout())
		{
			isOnDamaged = false;
			onDamagedTimer.Reset();
		}
	}
}

float Player::CalculateMaxHp() const
{
	return baseHp + (level * hpPerLevel);
}

float Player::CalculateMaxExp() const
{
	return baseExp * powf(growthRate, (float)level); // 레벨에 따라 지수적으로 증가
}

