#include "Player.h"

#include "Engine.h"
#include "Input.h"
#include "Math/Color.h"
#include "Level/Level.h"

#include "Actor/Weapons/Knife.h"

Player::Player()
	: Actor("@", Color::Red)
	, worldPosition(Position())
{
	stats.speed = 5.f;
	SetSortingOrder(10);
}

void Player::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	/// 방향키 입력
	// 화면 좌표계와 상관없이 직교 좌표계로 움직이게 처리하고 나중에 처리해야할듯

	/// 대각선 입력 처리
	/*
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) // 위, W
	{
		Vector2 position = Position();
		position.y -= 1;
		SetPosition(position);
	}
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) // 좌, A
	{
		Vector2 position = Position();
		position.x -= 1;
		SetPosition(position);
	}
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) // 아래, S
	{
		Vector2 position = Position();
		position.y += 1;
		SetPosition(position);
	}
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) // 우, D
	{
		Vector2 position = Position();
		position.x += 1;
		SetPosition(position);
	}
	*/

	// 1. 방향 입력 받기 (입력 로직만)
	Vector2 moveInput = Vector2::Zero;

	// 직교 좌표계로 입력 받기
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) { moveInput.y += 1.f; }; // 위로 이동하면 Y +
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) { moveInput.y += -1.f; }; // 아래로 이동하면 Y-
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) { moveInput.x += -1.f; };
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) { moveInput.x += 1.f; };

	// 2. 입력이 있을 때만 이동 처리 (물리적 이동)
	if (!(moveInput == Vector2::Zero))
	{
		// 이동 방향
		Vector2 moveDirection = moveInput.Normalize();

		// 현재 위치 업데이트
		worldPosition = worldPosition + moveDirection * stats.speed * deltaTime;

		// 플레이어는 항상 화면 중앙에 그려지므로, 이 코드는 불필요
		//SetPosition(worldPosition);

		direction = moveInput;
	}
	 
	if(Input::Get().GetKeyDown(VK_SPACE))
	{
		Weapon* weapon = new Knife(worldPosition, direction);
		GetOwner()->AddActor(weapon);
	}
}

/// <summary>
/// 현재 플레이어의 랜더러는 화면 좌표계에 움직이기 때문에 디버깅용임
/// </summary>
void Player::Render()
{
	// TEST용
	// 플레이어의 위치는 무조건 화면 가운데에서 그려져야하기 때문에 안 쓴다.
	// 부모 클래스의 Render는 순전히 디버깅용
	//super::Render();
	Engine::Get().WriteToBuffer({ Engine::Get().Width() / 2, Engine::Get().Height() / 2 }, GetImage(), Color::White, GetSortingOrder());
}

Vector2 Player::GetWorldPosition() const
{
	return worldPosition;
}

