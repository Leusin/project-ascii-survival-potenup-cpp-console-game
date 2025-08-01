#pragma once

#include <Windows.h>
#include "Math/Vector2.h"

/// <summary>
/// 
/// </summary>
class ScreenBuffer
{
	friend class Engine;

public:
	// 새 버퍼 생성 시 사용
	ScreenBuffer(const Vector2& screenSize);

	// 콘솔에 전달된 핸들을 통해 콘솔 버퍼 생성
	ScreenBuffer(HANDLE console, const Vector2& screenSize);

	~ScreenBuffer();

	void Clear(); // 버퍼 지우기
 
	void Render(CHAR_INFO* charInfo); // 콘솔 버퍼에 문자열 버퍼 사용시

private:
	HANDLE buffer = nullptr; // 콘솔 핸들
	
	Vector2 screenSize; // 화면 크기
};