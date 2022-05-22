#pragma once

#include <Windows.h>
#include <iostream>

static void  gotoxy(short x, short y)
{
	static HANDLE hConsoleOutput = NULL;
	if (NULL == hConsoleOutput)
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwCursorPosition = { x, y };
	std::cout.flush();
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}
