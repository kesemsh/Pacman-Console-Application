#pragma once

#include "GoodGhost.h"

class BestGhost : public GoodGhost
{
public:
	virtual void move(GameObject&, Board&, std::fstream&, short, short) override;
};






























