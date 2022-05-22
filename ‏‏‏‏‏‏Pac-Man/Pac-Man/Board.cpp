#include "Board.h"

Board::Board()//constractor that sets the board information
{
	width = 0;
	height = 0;
	bread_crumbs = 0;
	pacStartPos = {};
	legendStartPos = {};
}

char Board::getBoardAt(short y, short x) const 
{
	return board_vec[y][x];
}

short Board::getWidth() const
{
	return width;
}

short Board::getHeight() const
{
	return height;
}

short Board::getGhostStartPosX(short i) const
{
	return ghostStartPos_vec[i]._x;
}

short Board::getGhostStartPosY(short i) const
{
	return ghostStartPos_vec[i]._y;
}

short Board::getTunnelsPosFirstX(short i) const
{
	return tunnelsPos_vec[i].first._x;
}

short Board::getTunnelsPosFirstY(short i) const
{
	return tunnelsPos_vec[i].first._y;
}

short Board::getTunnelsPosSecondX(short i) const
{
	return tunnelsPos_vec[i].second._x;
}

short Board::getTunnelsPosSecondY(short i) const
{
	return tunnelsPos_vec[i].second._y;
}

short Board::getPacStartPosX() const
{
	return pacStartPos._x;
}

short Board::getPacStartPosY() const
{
	return pacStartPos._y;
}

short Board::getLegendStartPosX() const
{
	return legendStartPos._x;
}

short Board::getLegendStartPosY() const
{
	return legendStartPos._y;
}

short Board::getBreadCrumbs() const
{
	return bread_crumbs;
}

short Board::getNumOfGhosts() const 
{
	return (short)ghostStartPos_vec.size();
}

short Board::getNumOfTunnels() const 
{
	return (short)tunnelsPos_vec.size();
}

void Board::setBoardAt(short y, short x, char c)
{
	board_vec[y][x] = c;
}

void Board::setTunnelsVec()
{
	Position p1, p2;

	p1._x = 0;
	p2._x = width - 1;

	for (short y = 0; y < height; y++)
	{
		p1._y = p2._y = y;
		if (board_vec[y][0] == ' ' && board_vec[y][width - 1] == ' ' ||
			board_vec[y][0] == '.' && board_vec[y][width - 1] == ' ' ||
			board_vec[y][0] == ' ' && board_vec[y][width - 1] == '.' ||
			board_vec[y][0] == '.' && board_vec[y][width - 1] == '.')
		{
			tunnelsPos_vec.push_back(std::make_pair(p1, p2));
		}
	}

	p1._y = 0;
	p2._y = height - 1;

	for (short x = 0; x < width; x++)
	{
		p1._x = p2._x = x;
		if (board_vec[0][x] == ' ' && board_vec[height - 1][x] == ' ' ||
			board_vec[0][x] == '.' && board_vec[height - 1][x] == ' ' ||
			board_vec[0][x] == ' ' && board_vec[height - 1][x] == '.' ||
			board_vec[0][x] == '.' && board_vec[height - 1][x] == '.')
		{
			tunnelsPos_vec.push_back(std::make_pair(p1, p2));
		}
	}
}

void Board::resetBoard()
{
	width = 0;
	height = 0;
	bread_crumbs = 0;
	pacStartPos = {};
	legendStartPos = {};
	board_vec.clear();
	ghostStartPos_vec.clear();
	tunnelsPos_vec.clear();
}

void Board::updateBreadCrumbs()
{
	bread_crumbs--;
}

void Board::fillBoardVec(Files& f) //gets board from files
{
	bool isFirstLine = true;

	if (f.getNumOfScreens() <= f.getScreenNum())
	{
		throw Files::NoScreenFilesFound();
	}

	std::ifstream files(f.getScreenFileName(f.getScreenNum()));
	std::string line = "";

	if (!files)
	{
		throw Files::InvalidFile();
	}

	while (getline(files, line))
	{
		board_vec.push_back(line);
		
		if (isFirstLine)
		{
			width = (short)line.length();
			isFirstLine = false;
		}
	}
	height += (short)board_vec.size();

	files.close();

	if (height == 0 || width == 0 || height > 25 || width > 80 || width < 20)
	{
		throw Files::InvalidScreenSize();
	}
}

void Board::initBoard(Files& f)
{
	try {
		fillBoardVec(f);//get the files that contains the board
	}
	catch (Files::Error& e) {
		throw e;
	}

	try {
		ShowAndUpdateBoard(f);//prints the board
	}
	catch (Files::Error& e) {
		throw e;
	}

	setTunnelsVec();
}

void Board::ShowAndUpdateBoard(Files& f)
{
	short i;
	short countP = 0, countG = 0, countL = 0;
	bool isLegend = false;
	Position p, p1 = { -1,-1 }, p2 = { -1,-1 };

	for (short y = 0; y < height; y++)
	{
		for (short x = 0; x < width; x++)
		{
			p = { x, y };

			if (p._x == p1._x && p._y == p1._y)
			{
				ColorUtilities::checkAndSetColor(ColorUtilities::colors::yellow);
				if(f.getGameMode() != Files::gameMode::silent)
					std::cout << "Lives: 3 Points: 0  ";

				legendStartPos._x = x;
				legendStartPos._y = y;

				i = x;
				for (x; x <= 20 + i; x++)
				{
					board_vec[y][x] = (char)boardItems::wall;
				}
				x--;
			}
			else if (p._x == p2._x && p._y == p2._y)
			{
				ColorUtilities::checkAndSetColor(ColorUtilities::colors::yellow);
				if (f.getGameMode() != Files::gameMode::silent)
					std::cout << "                    ";

				i = x;
				for (x; x <= 20 + i; x++)
				{
					board_vec[y][x] = (char)boardItems::wall;
				}
				x--;
			}
			else
			{
				switch (board_vec[y][x])
				{
				case (char)boardItems::pacman:
					countP++;

					pacStartPos._x = x;
					pacStartPos._y = y;

					ColorUtilities::checkAndSetColor(ColorUtilities::colors::yellow);
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << board_vec[y][x];
					
					board_vec[y][x] = ' ';
					break;
				case (char)boardItems::ghost:
					countG++;
					
					ghostStartPos_vec.push_back(p);

					ColorUtilities::checkAndSetColor(ColorUtilities::colors::red);
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << board_vec[y][x];

					board_vec[y][x] = ' ';
					break;
				case (char)boardItems::empty:
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << ' ';
					board_vec[y][x] = ' ';
					break;
				case (char)boardItems::breadCrumbs:
					ColorUtilities::checkAndSetColor(ColorUtilities::colors::white);
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << '.';

					bread_crumbs++;
					board_vec[y][x] = '.';
					break;
				case (char)boardItems::wall:
					ColorUtilities::checkAndSetColor(ColorUtilities::colors::blue);
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << board_vec[y][x];
					break;
				case (char)boardItems::legend:
					isLegend = true;
					countL++;

					p1._x = x;
					p2._x = x;
					p1._y = y + 1;
					p2._y = y + 2;

					ColorUtilities::checkAndSetColor(ColorUtilities::colors::yellow);
					if (f.getGameMode() != Files::gameMode::silent)
						std::cout << "                    ";

					i = x;
					for (x; x <= 20 + i; x++)
					{
						if(x > width)
							throw Files::InvalidLegend();

						board_vec[y][x] = (char)boardItems::wall;
					}
					x--;
					break;
				default:
					throw Files::InvalidcharsFoundInFile();
				}
			}
		}
		if (f.getGameMode() != Files::gameMode::silent)
			std::cout << '\n';
	}

	if (countP == 0 || countP > 1)
		throw Files::InvalidNumOfPacman();
	if (countG > 4)
		throw Files::InvalidNumOfGhosts();
	if (countL > 1 || !isLegend)
		throw Files::InvalidLegend();
}
