#include "Player.h"

#include "Input.h"

Player::Player()
	: Actor("@")
{
	// TEST
	SetPosition({ 10 ,10 });
	stats.speed = 10.f;
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

	// 해당 방향으로 이동 시키기 
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W')) { moveInput.y = -1; };
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')) { moveInput.y = 1; };
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')) { moveInput.x = -1; };
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')) { moveInput.x = 1; };

	if (!(Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W') || Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S')))
	{
		moveInput.y = 0;
	}
	if (!(Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A')|| Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D')))
	{
		moveInput.x = 0;
	}

	// 2. 입력이 있을 때만 이동 처리 (물리적 이동)
	if (!(moveInput == Vector2::Zero))
	{
		// int 값을 float 값으로 변경
		float dx = static_cast<float>(moveInput.x);
		float dy = static_cast<float>(moveInput.y);

		float magnitude = moveInput.Magnitude();
		dx /= magnitude;
		dy /= magnitude;

		float moveX = dx * stats.speed * deltaTime;
		float moveY = dy * stats.speed * deltaTime;

		// TODO : 지금 이 스태틱 변수 어떻게든 처리할 것
		static float currentPosX = (float)Position().x;
		static float currentPosY = (float)Position().y;
		currentPosX += moveX;
		currentPosY += moveY;

		SetPosition({ (int)currentPosX, (int)currentPosY });

		direction = moveInput;
	}
}

void Player::Render()
{

}

