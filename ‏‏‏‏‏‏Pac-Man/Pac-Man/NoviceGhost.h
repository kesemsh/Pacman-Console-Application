#pragma once

#include "GoodGhost.h"

class NoviceGhost : public GoodGhost
{
public:
	void move(GameObject&, Board&, std::fstream& ,short, short) override;
};