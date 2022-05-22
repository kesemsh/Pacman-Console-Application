#pragma once

#include <iostream>
#include <Windows.h>

class ColorUtilities
{
public:

	enum colors { blue = 9, green = 10, cyan = 11, red = 12, magenta = 13, yellow = 14, white = 15};

	static bool is_color;

	static bool getIs_color(){
		return is_color;
	}
	static void setIs_Color(bool isColor) {
		is_color = isColor;
	}
	static void setColor(int color){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	static void checkAndSetColor(colors color){
		if (is_color)
			setColor(color);
		else
			setColor(white);
	}
};

