#include "Fruit.h"

Fruit::Fruit()//constractor that sets the pacman information
{
	setColor(ColorUtilities::colors::green);
	_points = 0;
	_lastmove = 0;
	count_appear = 0;
	isVisible = true;
}

void Fruit::setPoints()
{
	short i;
	
	srand((unsigned int)time(NULL));

	i = rand() % 5;

	switch (i) {
	case 0:
		setSymbol(five + '0');
		_points = five;
		break;
	case 1:
		setSymbol(six + '0');
		_points = six;
		break;
	case 2:
		setSymbol(seven + '0');
		_points = seven;
		break;
	case 3:
		setSymbol(eight + '0');
		_points = eight;
		break;
	case 4:
		setSymbol(nine + '0');
		_points = nine;
		break;
	}
}

void Fruit::setAppearance(short a)
{
	count_appear = a;
}

void Fruit::resetFruit(Board& b, std::fstream& stepsFile, short game_mode)
{
	setAppearance(0);
	setPoints();

	if (game_mode == Files::gameMode::load || game_mode == Files::gameMode::silent)
	{
		setFruitLocationFromFile(stepsFile);
	}
	else
	{
		setRandLocation(b);

		if (game_mode == Files::gameMode::save)
		{
			stepsFile << "Fruit start position: " << getX() << "," << getY() << std::endl;
		}
	}

	display(b, game_mode);
}

short Fruit::getPoints() const
{
	return _points;
}

bool Fruit::getIsVisible() const
{
	return isVisible;
}

void Fruit::setRandLocation(Board& b)
{
	short x, y;
	bool isValid = false;

	x = (rand() % (b.getWidth() - 1)) + 1;
	y = (rand() % (b.getHeight() - 1)) + 1;

	while (!isValid)
	{
		if (b.getBoardAt(y, x) != (char)b.boardItems::wall && b.getBoardAt(y, x) != (char)b.boardItems::pacman && b.getBoardAt(y, x) != (char)b.boardItems::ghost)
		{
			isValid = true;
		}
		else
		{
			x = (rand() % (b.getWidth() - 1)) + 1;
			y = (rand() % (b.getHeight() - 1)) + 1;
		}
	}
	setX(x);
	setY(y);
}

void Fruit::move(Board& b, std::fstream& stepsFile, short game_mode)
{	
	displayChar(game_mode, b.getBoardAt(getY(), getX()));

	short direction = _lastmove;

	while (true)
	{
		if (game_mode == Files::gameMode::load || game_mode == Files::gameMode::silent)
		{
			try {
				short move = getMoveFromFile(stepsFile) - '0';
				direction = move;
			}
			catch (Files::Error& e)
			{
				throw e;
			}
		}

		switch (direction)
		{
		case 0: //up
			setNextY(getY() - 1);
			setNextX(getX());
			count_appear++;
			break;
		case 1: //down
			setNextY(getY() + 1);
			setNextX(getX());
			count_appear++;
			break;
		case 2: //left
			setNextY(getY());
			setNextX(getX() - 1);
			count_appear++;
			break;
		case 3: //right
			setNextY(getY());
			setNextX(getX() + 1);
			count_appear++;
			break;
		}
		if (!isOutOfBounds(b) && b.getBoardAt(getNextY(), getNextX()) != (char)b.boardItems::wall) //if the next move is not a wall
		{
			//set the current move to be the next one
			setY(getNextY());
			setX(getNextX());
			_lastmove = direction;
			break;
		}
		else
		{
			direction = rand() % 4;
		}
	}
	
	if (count_appear % (APPEAR_MOVES*2) < APPEAR_MOVES)
	{
		isVisible = true;

		if (game_mode == Files::gameMode::save)
			stepsFile << "Fruit visible direction: " << direction << std::endl;
	}
	else
	{
		isVisible = false;
		
		if (game_mode == Files::gameMode::save)
			stepsFile << "Fruit invisible direction: " << direction << std::endl;
	}

	if (isVisible)
	{
		display(b, game_mode);
	}
}

void Fruit::setFruitLocationFromFile(std::fstream& stepsFile)
{
	short count = 0;
	short pointX, pointY;
	std::string line, strX, strY;
	std::size_t i, found1, found2;

	getline(stepsFile, line);
	found1 = line.find(": ");
	found2 = line.find(",");

	for (i = found1 + 2; i < found2; i++)
	{
		count++;
	}
	strX = line.substr(found1 + 2, count);

	std::stringstream num1(strX);
	num1 >> pointX;

	count = 0;
	for (i = found2 + 1; i < line.size(); i++)
	{
		count++;
	}
	strY = line.substr(found2 + 1, count);

	std::stringstream num2(strY);
	num2 >> pointY;

	setX(pointX);
	setY(pointY);
}