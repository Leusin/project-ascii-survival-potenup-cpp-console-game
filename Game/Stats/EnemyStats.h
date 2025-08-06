// EnemyStats.h
#pragma once

#include "Math/Color.h"

/// <summary>
/// 각 적들이 다르게 가질 수 있는 데이터들
/// </summary>
struct EnemyStats
{
	const char* icon;
	Color color = Color::Blue;
	float hp = 5.0f;
	float speed = 1.f;
	float damage = 0.05f;
	float exp = 1.0f;

	EnemyStats(const char* icon = "E", Color color = Color::Blue, float hp = 5.0f, float speed = 1.f, float damage = 0.05f, float exp = 1.0f)
		: icon(icon)
		, color(color)
		, hp(hp)
		, speed(speed)
		, damage(damage)
		, exp(exp)
	{

	}
};