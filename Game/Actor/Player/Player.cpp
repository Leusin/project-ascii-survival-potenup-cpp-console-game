#include "Player.h"

#include "Engine.h"
#include "Input.h"
#include "Level/Level.h"
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
		// 이동 방향
		Vector2F moveDirection = moveInput.Normalize();

		// 현재 위치 업데이트
		worldPosition = worldPosition + moveDirection * stats.speed * deltaTime;
		direction = moveInput;
	}

	// 어차피 화면 정 중앙이긴 하지만,
	SetPosition(Engine::Get().OrthogonalToScreenCoords(worldPosition, worldPosition));
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