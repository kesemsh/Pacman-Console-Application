#pragma once

#include "GameObject.h"

class GoodGhost :public GameObject
{
private:
	short _lastmove;
	short _direction;
	short _noviceCount;

public:
	GoodGhost();

	void moveStrategy(GameObject&, Board&, std::fstream&, short, short);
	virtual void move(GameObject&, Board&, std::fstream&, short, short);
	void noviceMove(Board&, std::fstream&, short, short);
	void bestMove(GameObject&, Board&, std::fstream&, short, short);
	void findPacman(GameObject&);
};
