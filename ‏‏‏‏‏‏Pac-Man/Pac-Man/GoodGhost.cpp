#include "GoodGhost.h"

GoodGhost::GoodGhost()
{
	setSymbol('$');
	setColor(ColorUtilities::colors::red);
	_lastmove = 0;
	_direction = 0;
	_noviceCount = 0;
}

void GoodGhost::moveStrategy(GameObject& p, Board& b, std::fstream& stepsFile, short game_mode, short ghostNum) //Good ghost mode
{
	move(p, b, stepsFile, game_mode, ghostNum);
}

void GoodGhost::move(GameObject& p, Board& b, std::fstream& stepsFile, short game_mode, short ghostNum)
{
	if (_noviceCount == 0)
	{
		if (rand() % 20 == 0) //switch to stupid mode
		{
			_noviceCount = 5;
			noviceMove(b, stepsFile, game_mode, ghostNum); //novice ghost move
		}
		else
		{
			bestMove(p, b, stepsFile, game_mode, ghostNum); //best ghost mode
		}
	}
	else
	{
		_noviceCount--;
		noviceMove(b, stepsFile, game_mode, ghostNum); //novice ghost move
	}
}

void GoodGhost::noviceMove(Board& b, std::fstream& stepsFile, short game_mode, short ghostNum)
{
	displayChar(game_mode, b.getBoardAt(getY(), getX())); //display the last character instend of the ghost
	
	_direction = _lastmove;

	while (true)
	{
		if (game_mode == Files::gameMode::load || game_mode == Files::gameMode::silent)
		{
			try {
				short move = getMoveFromFile(stepsFile) - '0';
				_direction = move;
			}
			catch (Files::Error& e) {
				throw e;
			}
		}

		switch (_direction)
		{
		case 0: //up
			setNextY(getY() - 1);
			setNextX(getX());
			break;
		case 1: //down
			setNextY(getY() + 1);
			setNextX(getX());
			break;
		case 2: //left
			setNextY(getY());
			setNextX(getX() - 1);
			break;
		case 3: //right
			setNextY(getY());
			setNextX(getX() + 1);
			break;
		}
		if (!isOutOfBounds(b) && b.getBoardAt(getNextY(), getNextX()) != (char)b.boardItems::wall) //if the next move is not a wall
		{
			//set the current move to be the next one
			setX(getNextX());
			setY(getNextY());
			_lastmove = _direction;
			break;
		}
		else
		{
			_direction = rand() % 4;
		}
	}
		
	display(b, game_mode); //print ghost charecter

	if (game_mode == Files::gameMode::save)
		stepsFile << "Ghost" << ghostNum << " direction: " << _direction << std::endl;
}

void GoodGhost::bestMove(GameObject& p, Board& b, std::fstream& stepsFile, short game_mode, short ghostNum)
{
	displayChar(game_mode, b.getBoardAt(getY(), getX())); //display the last character instend of the ghost

	setNextX(getX());
	setNextY(getY());

	findPacman(p);

	while (isOutOfBounds(b) || b.getBoardAt(getNextY(), getNextX()) == (char)b.boardItems::wall) //the next move is a wall
	{
		int ind;
		int dArr[4] = { 0,1,2,3 };

		ind = rand() % 4;

		while (_direction == dArr[ind])
		{
			ind = rand() % 4;
		}
		setNextX(getX());
		setNextY(getY());

		_direction = dArr[ind];

		if (_direction == 0) //up
		{
			setNextY(getNextY() - 1);
		}
		else if (_direction == 1) //down
		{
			setNextY(getNextY() + 1);
		}
		else if (_direction == 2) // left
		{
			setNextX(getNextX() - 1);
		}
		else if (_direction == 3) //right
		{
			setNextX(getNextX() + 1);
		}
	}

	setX(getNextX());
	setY(getNextY());

	display(b, game_mode); //print ghost charecter

	if (game_mode == Files::gameMode::save)
		stepsFile << "Ghost" << ghostNum << " direction: " << _direction << std::endl;
}

void GoodGhost::findPacman(GameObject& p)
{
	if (getX() > p.getX()) //pacman is on the left side of the ghost
	{
		_direction = 2;
		setNextX(getNextX() - 1);
	}
	else if (getX() < p.getX()) //pacman is on the right side of the ghost
	{
		_direction = 3;
		setNextX(getNextX() + 1);
	}
	else if (getX() == p.getX())
	{
		if (getY() > p.getY()) //pacman is above the ghost
		{
			_direction = 0;
			setNextY(getNextY() - 1);
		}
		else if (getY() < p.getY()) //pacman is below the ghost
		{
			_direction = 1;
			setNextY(getNextY() + 1);
		}
	}
}
