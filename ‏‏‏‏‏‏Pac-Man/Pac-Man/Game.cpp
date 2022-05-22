#include "Game.h"

Game::Game()
{
	_speed = 200;
	key = '\n';
	_lives = START_LIVES;
	ghost_num = 0;
	isCurrGameOver = false;
	isLastScreen = false;
}

Game::~Game()
{
	for(auto ptr: ghost_vec)
	{
		delete ptr;
	}
	ghost_vec.clear();
}

void Game::start(int argc, char** argv)
{
	try {
		files.cheackArgumentsAndSetGameMode(argc, argv);
	}
	catch (Files::Error& e)
	{
		e.getmsg();
		m.setUserChoice((char)m.menuOptions::exit_game);
	}

	std::fstream stepsFile;
	std::fstream resultFile;

	while (m.getUserChoice() != (char)m.menuOptions::exit_game)
	{
		if (files.getGameMode() == Files::gameMode::load || files.getGameMode() == Files::gameMode::silent)
		{
			m.setLevel(Menu::levelOptions::Novice);
			m.setUserChoice(Menu::menuOptions::run_game);
		}
		else
			m.menu(files);

		if (m.getUserChoice() == (char)m.menuOptions::run_game && m.getLevel() != 0)
		{
			try {
				runGame(stepsFile, resultFile);
			}
			catch (Files::Error& e) {
				e.getmsg();
				files.setScreenNum(0);
			}

			resetGame(stepsFile, resultFile);

			if (files.getGameMode() == Files::gameMode::normal || files.getGameMode() == Files::gameMode::save)
				m.setUserChoice((char)m.menuOptions::print_menu);
		}
	}

	if(files.getGameMode() == Files::gameMode::silent)
		files.printTestResults();
}

void Game::runGame(std::fstream& stepsFile, std::fstream& resultFile)
{
	char tempKey = '\n';
	bool isGhostMove = true;
	bool isFruitMove = true;
	bool wasPacEaten = false;
	short timeOfDeath = 0;
	short timeOfWin = 0;

	initGame(stepsFile, resultFile);

	while (!isCurrGameOver)
	{
		timeOfDeath++;
		timeOfWin++;

		Sleep(_speed);
		
		if (files.getGameMode() == Files::gameMode::normal || files.getGameMode() == Files::gameMode::save)
		{
			if (_kbhit())
			{
				tempKey = getKey(); //get the last key from the user as temp key until validity check
				wasPacEaten = false;
			}

			if (tempKey == ESC)
				pauseGame(tempKey);

			if (isKeyValid(tempKey))
				key = tempKey;

			if (wasPacEaten)
				key = (char)p.move_keys::stay;;
		}
		
		p.move(b, key, stepsFile, files.getGameMode());
		displayLegend();

		if (isGhostMove)
		{
			if(ghost_num != 0)
				moveGhosts(stepsFile);

			if (isFruitMove)
			{
				f.move(b, stepsFile, files.getGameMode());
			}
			isFruitMove = !isFruitMove;
		}
		isGhostMove = !isGhostMove; //moves the ghost every secont iteration so the ghost _speed will be half of the pacman _speed

		isWon(timeOfWin, stepsFile, resultFile);

		checkCollision(wasPacEaten, timeOfDeath, stepsFile, resultFile);

		if(files.getGameMode() != Files::gameMode::silent)
			gotoxy(b.getWidth() + 1, b.getHeight() + 1); //tab
	}
}

void Game::initGame(std::fstream& stepsFile, std::fstream& resultFile)
{
	try {
		files.openFiles(stepsFile, resultFile);
	}
	catch (Files::Error& e)
	{
		//e.getmsg();
		m.setUserChoice(Menu::menuOptions::exit_game);
		throw e;
	}

	if (files.getGameMode() == Files::gameMode::load)
		_speed = 50;
	else if (files.getGameMode() == Files::gameMode::silent)
		_speed = 0;

	isCurrGameOver = false;

	b.initBoard(files);
	displayLegend();
	initPacman();
	initGhostVec();
	f.resetFruit(b, stepsFile, files.getGameMode());
}

void Game::initGhostVec()
{
	setNumOfGhosts(); 

	if (ghost_num != 0)
	{
		setGhostVec();
		setStartPosGhosts();
		displayGhosts();
	}
}

void Game::setStartPosGhosts()
{
	short i = 0;

	for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end(); ptr++)
	{
		(**ptr).setStartPos(b.getGhostStartPosX(i), b.getGhostStartPosY(i));
		i++;
	}
}

void Game::initPacman()
{
	p.setStartPos(b.getPacStartPosX(), b.getPacStartPosY());
	p.display(b, files.getGameMode());
}

void Game::setNumOfGhosts()
{
	ghost_num = b.getNumOfGhosts();
}

void Game::setGhostVec()
{
	switch (m.getLevel())
	{
	case 1:
		for (short i = 0; i < ghost_num; i++)
		{
			ghost_vec.push_back(new NoviceGhost);
		}
		break;
	case 2:
		for (short i = 0; i < ghost_num; i++)
		{
			ghost_vec.push_back(new GoodGhost);
		}		
		break;
	case 3:
		for (short i = 0; i < ghost_num; i++)
		{
			ghost_vec.push_back(new BestGhost);
		}
		break;
	}
}

void Game::displayGhosts()
{
	for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end(); ptr++)
	{
		(**ptr).display(b, files.getGameMode());
	}
}

void Game::moveGhosts(std::fstream& stepsFile)
{
	short ghostNum = 0;

	for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end(); ptr++)
	{
		ghostNum++;
		(**ptr).moveStrategy(p, b, stepsFile, files.getGameMode(), ghostNum);
	}
}

void Game::checkCollision(bool& pacWasEaten, short& timeOfDeath, std::fstream& stepsFile, std::fstream& resultFile)
{
	if (p.isCollide(f) && f.getIsVisible()) //if pacman meets fruit and fruit is visible
	{	
		p.setPoints(p.getPoints() + f.getPoints());
		displayLegend();
		eatFruit(stepsFile);
	}
	if (ghost_num != 0)
	{
		for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end() && isCurrGameOver == false; ptr++)
		{
			if ((**ptr).isCollide(p)) //if pacman meets ghost
			{
				ghostEatPacman(pacWasEaten, timeOfDeath, resultFile);
			}
			if ((**ptr).isCollide(f) && f.getIsVisible()) //if fruit meets ghost
			{
				eatFruit(stepsFile);
			}
		}
	}
}

bool Game::isKeyValid(char& tempKey) const
{
	if (tempKey == (char)p.move_keys::down || tempKey == (char)p.move_keys::low_down || tempKey == (char)p.move_keys::up || tempKey == (char)p.move_keys::low_up || tempKey == (char)p.move_keys::right || tempKey == (char)p.move_keys::low_right || tempKey == (char)p.move_keys::left || tempKey == (char)p.move_keys::low_left|| tempKey == (char)p.move_keys::stay || tempKey == (char)p.move_keys::low_stay)
		return true;
	else
		return false;
}

int Game::getKey() const
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

void Game:: pauseGame(char& tempKey)
{
	do
	{
		gotoxy(0, b.getHeight() + 1);
		m.getColorUtility().checkAndSetColor(m.getColorUtility().red);
		std::cout << "*******************************************" << std::endl;
		std::cout << "*Game paused, press ESC again to continue.*" << std::endl;
		std::cout << "*******************************************" << std::endl;
		std::cout << "To return to main menu press 0." << std::endl;
		
		tempKey = getKey();
	} while (tempKey != ESC && tempKey != m.menuOptions::print_menu); //only esc resume the game
	
	if (tempKey == m.menuOptions::print_menu)
	{
		isCurrGameOver = true;
		isLastScreen = true;
	}
	else
	{
		tempKey = '\0';

		gotoxy(0, b.getHeight() + 1);
		std::cout << "                                           " << std::endl;
		std::cout << "                                           " << std::endl;
		std::cout << "                                           " << std::endl;
		std::cout << "                                           " << std::endl;
	}
}

void Game::displayLegend() const
{
	m.getColorUtility().checkAndSetColor(m.getColorUtility().yellow);
	
	gotoxy(b.getLegendStartPosX(), b.getLegendStartPosY());

	if (files.getGameMode() != Files::gameMode::silent)
	{
		printf("Lives: %1hd Points: %2hd", _lives, p.getPoints());

		gotoxy(b.getWidth() + 1, b.getHeight() + 1); //tab
	}
}

void Game::ghostEatPacman(bool& pacWasEaten, short& timeOfDeath, std::fstream& resultFile)
{
	pacWasEaten = true;

	if (_lives != 0) //ghost eat pacman but user doesnt lose
	{
		_lives--; //update life
		displayLegend();

		//delete characters and set their location to the starting _points
		p.displayChar(files.getGameMode(), ' ');
		p.resetPosition();

		for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end(); ptr++)
		{
			(**ptr).displayChar(files.getGameMode(), ' ');
			(**ptr).resetPosition();
		}
		
	}
	if (_lives == 0) //ghost eat pacman for the third time
	{
		if (files.getGameMode() == Files::gameMode::save)
			resultFile << "Point of time where pacman dies: " << timeOfDeath << std::endl;

		if (files.getGameMode() == Files::gameMode::silent)
			files.checkAndUpdateTests(resultFile, timeOfDeath);

		isCurrGameOver = true;
		isLastScreen = true;
		
		system("cls");

		if (files.getGameMode() == Files::gameMode::load || files.getGameMode() == Files::gameMode::silent)
		{
			m.setUserChoice((char)m.menuOptions::exit_game);
		}
		else
		{
			m.getColorUtility().checkAndSetColor(m.getColorUtility().magenta);
			std::cout << "\t\t\t    *************************" << std::endl;
			std::cout << "\t\t\t    * GAME OVER! (LOSER XD) *" << std::endl;
			std::cout << "\t\t\t    *************************\n" << std::endl;
			std::cout << "\t\t\tTo return to main menu press any key.\n";
			_getch();
		}
	}
}

void Game::eatFruit(std::fstream& stepsFile)
{
	f.setAppearance(f.APPEAR_MOVES + 1);
	f.setPoints();
}

void Game::isWon(short& timeOfWin, std::fstream& stepsFile, std::fstream& resultFile)
{
	if (b.getBreadCrumbs() == 0)
	{
		if(files.getGameMode() == Files::gameMode::save)
			resultFile << "Point of time where pacman wins: " << timeOfWin << std::endl;
		
		if (files.getGameMode() == Files::gameMode::silent)
			files.checkAndUpdateTests(resultFile, timeOfWin);

		system("cls");

		if (m.getWasScreenChosen())
		{
			m.setWasScreenChosen(false);
			isLastScreen = true;
		}
		else
		{
			files.setScreenNum(files.getScreenNum() + 1);
			checkIsLastScreen(isLastScreen);
		}

		if (isLastScreen)
		{
			isCurrGameOver = true;

			if (files.getGameMode() == Files::gameMode::load || files.getGameMode() == Files::gameMode::silent)
			{
				m.setUserChoice((char)m.menuOptions::exit_game);
			}
			else
			{
				m.getColorUtility().checkAndSetColor(m.getColorUtility().magenta);
				std::cout << "\t\t\t      ***************" << std::endl;
				std::cout << "\t\t\t      * YOU WIN! :) *" << std::endl;
				std::cout << "\t\t\t      ***************\n" << std::endl;
				std::cout << "\t\t\tTo return to main menu press any key.\n";
				_getch();
			}
		}
		else
		{
			resetGame(stepsFile, resultFile);
			try {
				runGame(stepsFile, resultFile);
			}
			catch (Files::Error& e) {
				throw e;
			}
		}
	}
}

void Game::resetGame(std::fstream& stepsFile, std::fstream& resultFile)
{
	key = (char)p.move_keys::stay;

	b.resetBoard();
	p.resetPoints();

	if (ghost_num != 0)
	{
		for (auto ptr = ghost_vec.begin(); ptr != ghost_vec.end(); ptr++)
		{
			delete (*ptr);
		}
		ghost_vec.clear();
		ghost_num = 0;
	}
	
	if (isLastScreen) //finished last screen
	{
		isLastScreen = false;
		files.setScreenNum(0);
		_lives = START_LIVES;
	}

	files.closeFiles(stepsFile, resultFile);
}

void Game::checkIsLastScreen(bool& isLast)
{
	if (files.getScreenNum() > files.getNumOfScreens() - 1)
		isLast = true;
	else
		isLast = false;
}
