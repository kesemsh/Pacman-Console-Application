#include "Pacman.h"

Pacman::Pacman()//constractor that sets the pacman information
{
	setSymbol('@');
	setColor(ColorUtilities::colors::yellow);
	_points = 0;
}

void Pacman::setPoints(short p)
{
	_points = p;
}

short Pacman::getPoints() const
{
	return _points;
}

void Pacman::resetPoints()
{
	_points = 0;
}

void Pacman::move_tunnel(Board& b, short game_mode)
{
	for (short i = 0; i < b.getNumOfTunnels(); i++)
	{
		if (getNextX() == b.getTunnelsPosFirstX(i) && getNextY() == b.getTunnelsPosFirstY(i))
		{
			setX(b.getTunnelsPosSecondX(i));
			setY(b.getTunnelsPosSecondY(i));
		}
		else if (getNextX() == b.getTunnelsPosSecondX(i) && getNextY() == b.getTunnelsPosSecondY(i))
		{
			setX(b.getTunnelsPosFirstX(i));
			setY(b.getTunnelsPosFirstY(i));
		}
	}
}

void Pacman::move(Board& b, char& key, std::fstream& stepsFile, short game_mode)
{
	displayChar(game_mode, ' ');

	setNextX(getX());
	setNextY(getY());

	if (game_mode == Files::gameMode::load || game_mode == Files::gameMode::silent)
	{
		try {
			char move = getMoveFromFile(stepsFile);
			key = move;
		}
		catch (Files::Error& e) {
			throw e;
		}
	}
	else if(key == '\n')
		key = (char)move_keys::stay;

	switch (key)
	{
	case (char)move_keys::left:
	case (char)move_keys::low_left:
		setNextX(getX() - 1);

		if(game_mode == Files::gameMode::save)
			stepsFile << "Pacman key: " << (char)move_keys::left << std::endl;
		break;
	case (char)move_keys::right:
	case (char)move_keys::low_right:
		setNextX(getX() + 1);

		if (game_mode == Files::gameMode::save)
			stepsFile << "Pacman key: " << (char)move_keys::right << std::endl;
		break;
	case (char)move_keys::up:
	case (char)move_keys::low_up:
		setNextY(getY() - 1);

		if (game_mode == Files::gameMode::save)
			stepsFile << "Pacman key: " << (char)move_keys::up << std::endl;
		break;
	case (char)move_keys::down:
	case (char)move_keys::low_down:
		setNextY(getY() + 1);

		if (game_mode == Files::gameMode::save)
			stepsFile << "Pacman key: " << (char)move_keys::down << std::endl;
		break;
	case (char)move_keys::stay:
	case (char)move_keys::low_stay:

		if (game_mode == Files::gameMode::save)
			stepsFile << "Pacman key: " << (char)move_keys::stay << std::endl;
		break;
	}

	if (!isOutOfBounds(b))
	{
		checkIfPacmanEatBreadCrumb(b);
		
		if (b.getBoardAt(getNextY(), getNextX()) == '.' || b.getBoardAt(getNextY(), getNextX()) == ' ')
		{
			setY(getNextY());
			setX(getNextX());
		}
	}

	move_tunnel(b, game_mode);

	display(b, game_mode); //prints the pacman in the new location
}

void Pacman::checkIfPacmanEatBreadCrumb(Board& b)
{
	if (b.getBoardAt(getNextY(), getNextX()) == '.') //pacman eats breadcrumb
	{
		b.setBoardAt(getNextY(), getNextX(), ' ');
		b.updateBreadCrumbs();
		_points++;
	}
}