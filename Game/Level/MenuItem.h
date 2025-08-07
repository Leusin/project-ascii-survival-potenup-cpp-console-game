#pragma once

struct MenuItem
{
	typedef void (*OnSelected)(); // 함수 포인터 선언

	MenuItem(const char* text, OnSelected onSelected);
	~MenuItem();

	char* menuText = nullptr; // 메뉴 텍스트
	OnSelected onSelected = nullptr; // 메뉴 선택 시 실행할 동작
};