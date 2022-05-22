#pragma once

#include "Files.h"

class Board
{
public:
	struct Position {
		short _x;
		short _y;
	};

private:
	short width, height;
	short bread_crumbs;

	std::vector<std::string> board_vec;
	
	Position pacStartPos;
	Position legendStartPos;
	std::vector<Position> ghostStartPos_vec;
	std::vector<std::pair<Position, Position>> tunnelsPos_vec;

public:
	Board();
	enum boardItems { pacman = '@', ghost = '$', legend = '&', breadCrumbs = ' ', empty = '%', wall = '#' };
	
	void setBoardAt(short, short, char);
	char getBoardAt(short, short) const;
	short getWidth() const;
	short getHeight() const;
	void updateBreadCrumbs();
	short getBreadCrumbs() const;

	void setTunnelsVec();
	short getTunnelsPosFirstX(short) const;
	short getTunnelsPosFirstY(short) const;
	short getTunnelsPosSecondX(short) const;
	short getTunnelsPosSecondY(short) const;
	short getNumOfTunnels() const;

	short getGhostStartPosX(short) const;
	short getGhostStartPosY(short) const;
	short getNumOfGhosts() const;

	short getPacStartPosX() const;
	short getPacStartPosY() const;
	short getLegendStartPosX() const;
	short getLegendStartPosY() const;

	void initBoard(Files&);
	void resetBoard();
	void fillBoardVec(Files&);
	void ShowAndUpdateBoard(Files& f);

};

