#include "Player.h"

#include "Engine.h"
#include "Input.h"
#include "Level/Level.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Weapons/Knife.h"

Player::Player()
	: Actor("@", Color::White)
	, worldPosition(Position())
{
	// SET
	stats.speed = 5.f;

	SetSortingOrder(10);

	onDamagedTimer.SetTargetTime(invulnerableTime);
}

void Player::BeginPlay()
{
	super::BeginPlay();

	// 기본 무기 
	Weapon* weapon = new Knife(worldPosition, direction);
	GetOwner()->AddActor(weapon);
}

void Player::Tick(float deltaTime)
{
	ProcessDamaged(deltaTime); // 플레이어가 데이미지를 입었을 경우 처리(무적 시간)

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	Vector2F moveInput = Vector2F::Zero;

	// 직교 좌표계로 입력 받음
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) { moveInput.y += 1.f; }; // 위로 이동하면 Y +
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) { moveInput.y += -1.f; }; // 아래로 이동하면 Y-
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) { moveInput.x += -1.f; };
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) { moveInput.x += 1.f; };

	// 입력이 있을 때만 이동 처리
	if (moveInput.SqrMagnitude() > 0.f)
	{
		Vector2F moveDirection = moveInput.Normalize(); // 이동 방향

		Vector2F movement = moveDirection * stats.speed * deltaTime; // 이동 거리

		Vector2F nextPosition = worldPosition + movement; // 다음 이동할 월드 위치

		//
		// 이동 위치 검사. 적이 있다면 이동해서는 안됨
		//
		
		// 
		Vector2I nextTilePos = Engine::Get().OrthogonalToScreenCoords(nextPosition, worldPosition); // 다음에 이동할 화면 위치

		std::vector<Actor*> actors = GetOwner()->GetActors();
		for (Actor* actor : actors)
		{
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
				return;
			}
		}

		//
		// 이동 위치 검사 완료 -----
		//

		worldPosition = nextPosition; // 이동 위치 업데이트
		direction = moveInput; // 마지막으로 보고 있었던 방향
	}

	// 플레이어는 정 중앙 화면으로 위치 고정
	SetPosition(Engine::Get().ScreenCenter());
}

void Player::Render()
{
	super::Render();

	color = isOnDamaged ? Color::Red : Color::White;
}

void Player::TakeDamage(float damage)
{
	if (isOnDamaged)
	{
		return;
	}

	isOnDamaged = true;

	// TODO: 데미지 처리
}

const Vector2F& Player::GetWorldPosition() const
{
	return worldPosition;
}

const Vector2F& Player::GetDirection() const
{
	return direction;
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