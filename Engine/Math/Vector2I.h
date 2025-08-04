#pragma once

#include <windows.h>
#include "Core.h"
#include "Vector2F.h"

struct Engine_API Vector2I
{
    Vector2I(int x = 0, int y = 0);

    // 형변환 연산자 오버로딩
    operator COORD();
    operator COORD() const;
    operator Vector2F();
    operator Vector2F() const;

    // 비교 연산자 (필수)
    bool operator==(const Vector2I& other) const
    {
        return x == other.x && y == other.y;
    }

    // 직교 좌표계에서의 방향 벡터
    static Vector2I Zero;
    static Vector2I One;

public:
    int x = 0;
    int y = 0;
};
