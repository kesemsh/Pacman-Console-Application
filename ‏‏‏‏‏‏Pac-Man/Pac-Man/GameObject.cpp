#include "GameObject.h"

GameObject::GameObject() //empty constractor to resetPosition the charecter location, _lives and _symbol
{
	_x = 0;
	_y = 0;
	_startX = 0;
	_startY = 0;
	_nextX = 0;
	_nextY = 0;
	_symbol = '*';
	_color = ColorUtilities::colors::white;
}

void GameObject::setX(short x)
{
	_x = x;
}

void GameObject::setY(short y)
{
	_y = y;
}

void GameObject::setStartPos(short x, short y)
{
	_startX = x;
	_startY = y;
	_x = x;
	_y = y;
}

void GameObject::setNextX(short nx)
{
	_nextX = nx;
}

void GameObject::setNextY(short ny)
{
	_nextY = ny;
}

void GameObject::setSymbol(char files)
{
	_symbol = files;
}

void GameObject::setColor(ColorUtilities::colors color)
{
	_color = color;
}

short GameObject::getX() const
{
	return _x;
}

short GameObject::getY() const
{
	return _y;
}

short GameObject::getNextX() const
{
	return _nextX;
}

short GameObject::getNextY() const
{
	return _nextY;
}

char GameObject::getSymbol() const
{
	return _symbol;
}

void GameObject::resetPosition()
{
	setX(_startX);
	setY(_startY);
}

void GameObject::display(Board& b, short game_mode) const
{
	gotoxy(_x, _y);
	ColorUtilities:: checkAndSetColor(_color);
	if (game_mode != Files::gameMode::silent)
	{
		std::cout << _symbol;
		gotoxy(b.getWidth() + 1, b.getHeight() + 1); //tab
	}
}

void GameObject::displayChar(short game_mode, char ch) const
{
	gotoxy(_x, _y);
	ColorUtilities::checkAndSetColor(ColorUtilities::colors::white);
	if (game_mode != Files::gameMode::silent)
		std::cout << ch;
}

bool GameObject::isCollide(GameObject& o) const //check if objects collide
{
	return ((_y == o.getY() && _x == o.getX()) || (_nextX == o.getNextX() && _nextY == o.getNextY()));
}

bool GameObject::isOutOfBounds(Board& b) const
{
	if (_nextX < b.getWidth() && _nextY < b.getHeight() && _nextY >= 0 && _nextX >= 0)
		return false;
	else
		return true;
}

char GameObject::getMoveFromFile(std::fstream& stepsFile) const
{
	char move = '\n';
	std::string line;
	
	getline(stepsFile, line);

	if (line.size() == 0)
		throw Files::InvalidFile();

	return move = line[line.size() - 1];
}
