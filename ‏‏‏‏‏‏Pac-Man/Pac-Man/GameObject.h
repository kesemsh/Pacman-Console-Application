#pragma once

#include <time.h> 
#include <cstdlib>

#include "Board.h"
#include "Files.h"

class GameObject {
private:
	short _x, _y;
	short 	_startX, _startY;
	short _nextX, _nextY;
	char _symbol;
	ColorUtilities::colors _color;

public:
	GameObject();

	void setX(short);
	void setY(short);
	void setStartPos(short, short);
	void setNextX(short);
	void setNextY(short);
	void setSymbol(char);
	void setColor(ColorUtilities::colors);

	short getX() const;
	short getY() const;
	short getNextX() const;
	short getNextY() const;
	char getSymbol() const;

	void resetPosition();
	void display(Board&, short) const;
	void displayChar(short, char) const;
	bool isCollide(GameObject&) const;
	bool isOutOfBounds(Board&) const;
	char getMoveFromFile(std::fstream&) const;
};