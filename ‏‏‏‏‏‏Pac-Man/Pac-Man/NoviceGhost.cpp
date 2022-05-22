#include "NoviceGhost.h"

void NoviceGhost::move(GameObject& p, Board& b, std::fstream& stepsFile ,short game_mode, short ghostNum)
{
	noviceMove(b, stepsFile, game_mode, ghostNum);
}
