#pragma once

/// <summary>
/// 무기 객체들이 공통적으로 가지는 속성
/// </summary>
struct WeaponStats
{
	unsigned int currentLevel = 1;
	const unsigned int maxLevel = 4;

	/// <summary>
	/// 적중당 투사체 한 발이 주는 피해량입니다.
	/// </summary>
	float baseDamaged = 5.f;

	/// <summary>
	/// 무기의 기본 투사체 속도입니다.
	/// </summary>
	float speed;

	/// <summary>
	/// 무기 효과의 지속 시간입니다.
	/// </summary>
	float duration;

	/// <summary>
	/// 무기를 다시 사용하는 데 필요한 시간입니다
	/// </summary>
	float cooldown;

	/// <summary>
	/// 같은 적이 같은 발사체에 다시 맞을 수 있는 시간입니다.
	/// </summary>
	float hitboxDelay;

	/// <summary>
	/// 화면에 표시할 수 있는 최대 발사체 수입니다.
	/// </summary>
	float poolLimit;

	/// <summary>
	/// 치명타에 피해 증가 배율 입니다.
	/// </summary>
	float critMulti = 0.1f;

	/// <summary>
	/// 무기의 기본 범위 입니다.
	/// </summary>
	unsigned int area = 1;

	/// <summary>
	/// 사용당 발사되는 발사체의 양입니다.
	/// </summary>
	int amount = 1;

	/// <summary>
	/// 발사체를 추가로 발사하는 데 필요한 시간입니다.
	/// </summary>
	float projectileInterval = 0.1f;
};