#pragma once

#include "GameObject.h"

class Fruit : public GameObject
{
private:
	short _points;
	short _lastmove;
	short count_appear;
	bool isVisible;

public:
	Fruit();
	enum points{five = 5, six = 6,  seven = 7, eight = 8, nine = 9};
	const short APPEAR_MOVES = 20;

	void setPoints();
	void setAppearance(short);

	short getPoints() const;
	bool getIsVisible() const;
	void setRandLocation(Board&);
	
	void resetFruit(Board&, std::fstream&, short);
	void move(Board&, std::fstream&, short);
	void setFruitLocationFromFile(std::fstream&);
};

