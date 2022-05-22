#pragma once

#include "GameObject.h"

class Pacman : public GameObject
{
private:
	short _points;

public:
	Pacman();
	enum move_keys { up = 'W', low_up = 'w', down = 'X', low_down = 'x', right = 'D', low_right = 'd', left = 'A', low_left = 'a', stay = 'S', low_stay = 's' };
	
	void setPoints(short);
	short getPoints() const;
	void resetPoints();

	void move(Board&, char&, std::fstream&, short);
	void move_tunnel(Board&, short);
	void checkIfPacmanEatBreadCrumb(Board& b);
};