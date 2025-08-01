#pragma once

#include <windows.h>
#include "Core.h"

class Engine_API Vector2
{
public:
	Vector2(int x = 0, int y = 0);
	~Vector2();

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	bool operator==(const Vector2& other) const;

	/// <summary>
	/// 벡터 정규화 취소.
	/// 생각해보니 벡터의 원소들이 int라서 여기서 정규화 하긴 좀 그러타
	/// </summary>
	//Vector2 Normalize() const;

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
	int x = 0;
	int y = 0;

private:
	char* value = nullptr;
};
