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
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	// 1. 방향 입력 받기 (입력 로직만)
	Vector2F moveInput = Vector2F::Zero;

	// 직교 좌표계로 입력 받기
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) { moveInput.y += 1.f; }; // 위로 이동하면 Y +
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) { moveInput.y += -1.f; }; // 아래로 이동하면 Y-
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) { moveInput.x += -1.f; };
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) { moveInput.x += 1.f; };

	// 2. 입력이 있을 때만 이동 처리 (물리적 이동)
	if (moveInput.SqrMagnitude() > 0.f)
	{
		// 이동 방향
		Vector2F moveDirection = moveInput.Normalize();

		// 현재 위치 업데이트
		worldPosition = worldPosition + moveDirection * stats.speed * deltaTime;

		// 플레이어는 항상 화면 중앙에 그려지므로, 이 코드는 불필요
		//SetPosition(worldPosition);

		direction = moveInput;
	}

	// 어차피 화면 정 중앙이긴 하지만,
	SetPosition(Engine::Get().OrthogonalToScreenCoords(worldPosition, worldPosition));
}

/// <summary>
/// 현재 플레이어의 랜더러는 화면 좌표계에 움직이기 때문에 디버깅용임
/// </summary>
void Player::Render()
{
	super::Render();
}

const Vector2F& Player::GetWorldPosition() const
{
	return worldPosition;
}

const Vector2F& Player::GetDirection() const
{
	return direction;
}
