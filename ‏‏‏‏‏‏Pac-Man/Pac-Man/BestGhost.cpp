#include "BestGhost.h"

void BestGhost::move(GameObject& p, Board& b, std::fstream& stepsFile, short game_mode, short ghostNum)
{
	bestMove(p, b, stepsFile, game_mode, ghostNum);
}