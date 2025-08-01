#pragma once

#include <windows.h>
#include "Core.h"

class Engine_API Vector2
{
public:
	Vector2(float x = 0.f, float y = 0.f);
	Vector2(int x, int y);
	~Vector2();

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const float& scalar) const;

	/// <summary>
	/// 이 연산은 부동 소수점(float) 오차를 고려하여
	/// 두 벡터가 허용 오차(tolerance) 범위 내에 있는지 확인합니다.
	/// 허용 오차는 0.0001f 로 설정되어 있습니다.
	/// </summary>
	bool operator==(const Vector2& other) const;

	/// <summary>
	/// 벡터 정규화 취소.
	/// </summary>
	Vector2 Normalize() const;

	/// <summary>
	/// 벡터 크기 구하기
	/// </summary>
	float Magnitude() const;

	/// <summary>
	///만약 벡터의 크기 비교만 필요하고, 실제 크기 값을 얻을 필요 없을 때 사용
	/// </summary>
	float SqrMagnitude() const;

	// 형변환 연산자 오버로딩
	operator COORD();
	operator COORD() const;

	const char* ToString();

	static Vector2 Zero;
	static Vector2 One;
	static Vector2 Up;
	static Vector2 Right;

public:
	float x = 0.f;
	float y = 0.f;

private:
	char* value = nullptr;
};
