#pragma once

#include<iterator>

#include "Menu.h"
#include "Pacman.h"
#include "Fruit.h"
#include "BestGhost.h"
#include "NoviceGhost.h"

class Game
{
private:
	int _speed = 200;
	char key;
	short _lives;
	short ghost_num;
	bool isCurrGameOver;
	bool isLastScreen;

	Files files;
	Menu m;
	Board b;
	Pacman p;
	Fruit f;
	std::vector<GoodGhost*> ghost_vec;
	
public:
	Game();
	~Game();
	static const int ESC = 27;
	static const int START_LIVES = 3;

	void start(int,char**);
	void runGame(std::fstream&, std::fstream&);

	void resetGame(std::fstream&, std::fstream&);
	void initGame(std::fstream&, std::fstream&);
	void initGhostVec();
	void initPacman();
	void setNumOfGhosts();
	void setGhostVec();
	void setStartPosGhosts();

	int getKey() const;
	bool isKeyValid(char&) const;
	void displayGhosts();
	void displayLegend() const;

	void pauseGame(char&);
	void moveGhosts(std::fstream&);
	void checkIsLastScreen(bool&);
	void checkCollision(bool&, short&, std::fstream&, std::fstream&);
	void ghostEatPacman(bool&, short&, std::fstream&);
	void eatFruit(std::fstream&);
	void isWon(short&, std::fstream&, std::fstream&);
};