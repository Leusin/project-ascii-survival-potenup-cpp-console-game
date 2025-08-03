#include "Vector2.h"

#include <iostream>
#include <cmath>
#include "Engine.h"

// 전역 변수 초기화.
Vector2 Vector2::Zero = Vector2(0.f, 0.f);
Vector2 Vector2::One = Vector2(1.f, 1.f);
Vector2 Vector2::Up = Vector2(0.f, 1.f);
Vector2 Vector2::Right = Vector2(1.f, 0.f);
Vector2 Vector2::Down = Vector2(0.f, -1.f);
Vector2 Vector2::Left = Vector2(-1.f, 0.f);

// 대각선 벡터는 길이가 1이 되도록 정규화된 값을 사용합니다.
const float invSqrt2 = 0.707106781f;
Vector2 Vector2::UpRight = Vector2(invSqrt2, invSqrt2);
Vector2 Vector2::UpLeft = Vector2(-invSqrt2, invSqrt2);
Vector2 Vector2::DownRight = Vector2(invSqrt2, -invSqrt2);
Vector2 Vector2::DownLeft = Vector2(-invSqrt2, -invSqrt2);

Vector2::Vector2(int x, int y)
	:x(static_cast<float>(x)), y(static_cast<float>(y))
	
{
}

Vector2::Vector2(float x, float y)
	:x(x), y(y)
{
}

Vector2::~Vector2()
{
	if (value == nullptr)
	{
		return;
	}

	delete[] value;
}


Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

bool Vector2::operator==(const Vector2& other) const
{
	const float epsilon = 0.0001f; // 허용 가능한 오차
	return (std::abs(x - other.x) < epsilon) && (std::abs(y - other.y) < epsilon);
}

Vector2 Vector2::Normalize() const
{
	float magnitude = Magnitude();

	if (magnitude > 0) // 0보다 클 때만 나누기 연산을 수행
	{
		return { x / magnitude, y / magnitude };
	}

	__debugbreak();
	return { 0.f, 0.f };
}

float Vector2::Magnitude() const
{
	// 각 원소를 제곱 값을 모두 더한 뒤 제곱근 해줌

	return sqrt(SqrMagnitude());
}


float Vector2::SqrMagnitude() const
{
	return (x * x + y * y);
}

Vector2::operator COORD()
{
	COORD coord;
	coord.X = (short)x;
	coord.Y = (short)y;

	return coord;
}

Vector2::operator COORD() const
{
	COORD coord;
	coord.X = (short)x;
	coord.Y = (short)y;

	return coord;
}

const char* Vector2::ToString()
{
	if (value == nullptr)
	{
		delete[] value;
	}

	value = new char[1024];
	sprintf_s(value, 1024, "(%d, %d)", static_cast<int>(x), static_cast<int>(y));

	return value;
}