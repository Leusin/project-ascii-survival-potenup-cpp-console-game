#include "MenuItem.h"

#include <iostream>
#include "Core/Core.h"

MenuItem::MenuItem(const char* text, OnSelected onSelected)
	: onSelected(onSelected)
{
	size_t len = (int)strlen(text) + 1;
	menuText = new char[len];
	strcpy_s(menuText, len, text);
}


MenuItem::~MenuItem()
{
	SafeDeleteArray(menuText);
}
