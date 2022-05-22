#include "Menu.h"

bool ColorUtilities::is_color;

Menu::Menu()
{
	_level = 0;
	screen_choice = -1;
	wasScreenChosen = false;
	user_choice = '0';
}

void Menu::setUserChoice(char u)
{
	user_choice = u;
}

void Menu::setWasScreenChosen(bool files)
{
	wasScreenChosen = files;
}

void Menu::setLevel(char l)
{
	_level = l - '0';
}

void Menu::setScreenChoice(char files)
{
	screen_choice = (files - '0') - 1;
}

void Menu::setColorUtility(ColorUtilities c)
{
	_cu = c;
}

short Menu::getLevel() const
{
	return _level;
}

char Menu::getUserChoice() const
{
	return user_choice;
}

bool Menu::getWasScreenChosen() const
{
	return wasScreenChosen;
}

ColorUtilities Menu::getColorUtility() const
{
	return _cu;
}

void Menu::menu(Files& f)
{
	switch (user_choice)
	{
	case menuOptions::print_menu:
		system("cls"); 
		printMenu();
		user_choice = _getch();
		menu(f);
		break;
	case menuOptions::run_game:
		system("cls");
		levelMenu();
		break;
	case menuOptions::add_colors:
		_cu.setIs_Color(true);
		user_choice = (char)menuOptions::print_menu;
		break;
	case menuOptions::cancle_colors:
		_cu.setIs_Color(false);
		user_choice = (char)menuOptions::print_menu;
		break;
	case menuOptions::choose_screen:
		system("cls");
		try {
			screenMenu(f);
		}
		catch (Files::NoScreenFilesFound& e) {
			e.getmsg();
			user_choice = (char)menuOptions::print_menu;
			menu(f);
		}
		user_choice = (char)menuOptions::run_game;
		break;
	case menuOptions::print_instroctions:
		printInstroctions();
		user_choice = (char)menuOptions::print_menu;
		break;
	case menuOptions::exit_game:
		printGoodBye();
		break;
	default:
		printInvalidChoice();
		user_choice = (char)menuOptions::print_menu;
	}
}
	 
void Menu::levelMenu()
{
	bool isValid = true;
	char level;

	printLevelOptions();
	level = _getch();
	isValid = isLevelValid(level);

	if (isValid == false)
	{
		_cu.checkAndSetColor(_cu.red);
		std::cout << "\n\t\t\t  ********************************" << std::endl;
		std::cout << "\t\t\t  * Please enter a valid choice! *" << std::endl;
		std::cout << "\t\t\t  ********************************" << std::endl;
	}

	while (isValid == false)
	{
		level = _getch();
		isValid = isLevelValid(level);
	}

	system("cls");
	setLevel(level);
}

void Menu::screenMenu(Files& f)
{
	bool isValid = true;

	if (f.getNumOfScreens() == 0)
	{
		throw Files::NoScreenFilesFound();
	}

	printScreenOptions(f);
	setScreenChoice(_getch());
	isValid = isScreenChoiceValid(f);

	if (isValid == false)
	{
		_cu.checkAndSetColor(_cu.red);
		std::cout << "\n\t\t\t  ********************************" << std::endl;
		std::cout << "\t\t\t  * Please enter a valid choice! *" << std::endl;
		std::cout << "\t\t\t  ********************************" << std::endl;
	}

	while (isValid == false)
	{
		setScreenChoice(_getch());
		isValid = isScreenChoiceValid(f);
	}

	system("cls");
	f.setScreenNum(screen_choice);
	wasScreenChosen = true;
	levelMenu();
}
	 
bool Menu::isLevelValid(char level) const
{
	if (level == (char)levelOptions::Novice || level == (char)levelOptions::Good || level == (char)levelOptions::Best)
		return true;
	else
		return false;
}

bool Menu::isScreenChoiceValid(Files& f) const
{
	for (short i = 0; i < f.getNumOfScreens(); i++)
	{
		if (screen_choice == i)
			return true;
	}
	return false;
}
	 
void Menu::printMenu() const
{
	system("cls");

	_cu.checkAndSetColor(_cu.yellow);
	std::cout << "\t\t\t    **********************" << std::endl;
	std::cout << "\t\t\t    * WELCOME TO PACMAN! *" << std::endl;
	std::cout << "\t\t\t    **********************\n" << std::endl;
	_cu.checkAndSetColor(_cu.green);
	std::cout << "\t\tTo continue please choose one of the following:\n" << std::endl;
	_cu.checkAndSetColor(_cu.cyan);
	std::cout << "\t\t\t  To start a new game press (1).\n" << std::endl;
	_cu.checkAndSetColor(_cu.magenta);
	std::cout << "\t\t    For color press (2) to cancel press (3).\n" << std::endl;
	_cu.checkAndSetColor(_cu.red);
	std::cout <<  "\t\t  To choose a screen for the board press (4).\n" << std::endl;
	_cu.checkAndSetColor(_cu.blue);
	std::cout << "\t\t   To present instructions and keys press (8).\n" << std::endl;
	_cu.checkAndSetColor(_cu.yellow);
	std::cout << "\t\t\t\tTo exit press (9)." << std::endl;
}
	 
void Menu::printLevelOptions() const
{
	_cu.checkAndSetColor(_cu.yellow);
	std::cout << "\t\t\t    *****************" << std::endl;
	std::cout << "\t\t\t    * PICK A LEVEL: *" << std::endl;
	std::cout << "\t\t\t    *****************\n" << std::endl;
	_cu.checkAndSetColor(_cu.green);
	std::cout << "\t\t(1) Novice.\n" << std::endl;
	_cu.checkAndSetColor(_cu.cyan);
	std::cout << "\t\t(2) Good.\n" << std::endl;
	_cu.checkAndSetColor(_cu.magenta);
	std::cout << "\t\t(3) Best.\n" << std::endl;
}

void Menu::printScreenOptions(Files& f) const
{
	_cu.checkAndSetColor(_cu.yellow);
	std::cout << "\t\t\t    ********************" << std::endl;
	std::cout << "\t\t\t    * PICK SCREEN FILE: *" << std::endl;
	std::cout << "\t\t\t    *********************\n" << std::endl;
	_cu.checkAndSetColor(_cu.green);
	
	for (short i = 0; i < f.getNumOfScreens(); i++)
	{
		std::cout << "\t\t(" << i + 1 << ")." << f.getScreenFileName(i) << "\n\n";
	}
}

void Menu::printInstroctions() const
{
	system("cls");

	_cu.checkAndSetColor(_cu.yellow);
	std::cout << "\t\t\t    *****************" << std::endl;
	std::cout << "\t\t\t    * INSTRUCTIONS: *" << std::endl;
	std::cout << "\t\t\t    *****************\n" << std::endl;
	_cu.checkAndSetColor(_cu.green);
	std::cout << "\tAs the player you control Pacman with the keys mentiond below!" << std::endl;
	std::cout << "To win the game Pacman must eat all the food inside the enclosed maze while" << std::endl;
	std::cout << "\t\t    avoiding being eaten by the ghosts." << std::endl;
	std::cout << "\t\t  Each new game Pacman starts with 3 lives." << std::endl;
	std::cout << "\t\tif at any point he's eaten by a ghost a life" << std::endl;
	std::cout << "    will be lost and all characters will start at their starting position." << std::endl;
	std::cout << "While playing make sure to use the tunnels to move around the maze more easily." << std::endl;
	std::cout << "\t\t  If all 3 lives are lost it's a GAME OVER!\n" << std::endl;

	_cu.checkAndSetColor(_cu.cyan);
	std::cout << "\t\tCharacters:" << "\t\t\tKeys:" << std::endl;
	std::cout << "\t\t************" << "\t\t\t*****" << std::endl;

	_cu.checkAndSetColor(_cu.magenta);
	std::cout << "\t\tPacman - @" << "\t\t\tLEFT - a or A." << std::endl;
	std::cout << "\t\tGhost - $" << "\t\t\tRIGHT - d or D." << std::endl;
	std::cout << "\t\tFood- ." << "\t\t\t\tUP - w or W." << std::endl;
	std::cout << "\t\t\t\t\t\tDOWN - x or X." << std::endl;
	std::cout << "\t\t\t\t\t\tSTAY - s or S." << std::endl;
	std::cout << "\t\t\t\t\t\tESC- pause game.\n" << std::endl;

	_cu.checkAndSetColor(_cu.red);
	std::cout << "\t\t\t\tGOOD LUCK!" << std::endl;
	std::cout << "\t\t\tTo return to main menu press any key.\n";
	_getch();
}
	 
void Menu::printGoodBye() const
{
	system("cls");

	_cu.checkAndSetColor(_cu.magenta);
	std::cout << "\t\t\t    ***************" << std::endl;
	std::cout << "\t\t\t    * GOOD BYE!:) *" << std::endl;
	std::cout << "\t\t\t    ***************\n" << std::endl;
}
	 
void Menu::printInvalidChoice() const
{
	system("cls");

	_cu.checkAndSetColor(_cu.red);
	std::cout << "\n\t\t\t  ********************************" << std::endl;
	std::cout << "\t\t\t  * Please enter a valid choice! *" << std::endl;
	std::cout << "\t\t\t  ********************************" << std::endl;
	std::cout << "\n\t\t\t  To return to main menu press any key." << std::endl;
	_getch();
}