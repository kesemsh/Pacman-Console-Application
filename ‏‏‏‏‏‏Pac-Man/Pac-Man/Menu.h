#pragma once

#include "Files.h"

class Menu
{
private:
	char user_choice;
	short _level;
	short screen_choice;
	bool wasScreenChosen;
	ColorUtilities _cu;

public:
	Menu();
	enum menuOptions{print_menu = '0', run_game = '1', add_colors = '2', cancle_colors = '3', choose_screen = '4', print_instroctions = '8', exit_game = '9' };
	enum levelOptions{Novice = '1', Good = '2', Best = '3' };

	void setUserChoice(char);
	void setWasScreenChosen(bool);
	void setLevel(char);
	void setScreenChoice(char);
	void setColorUtility(ColorUtilities);

	char getUserChoice() const;
	bool getWasScreenChosen() const;
	short getLevel() const;
	ColorUtilities getColorUtility() const;

	void menu(Files&);
	void printMenu() const;
	void levelMenu();
	void screenMenu(Files&);
	bool isLevelValid(char) const;
	bool isScreenChoiceValid(Files&) const;
	void printScreenOptions(Files&) const;
	void printLevelOptions() const;
	void printInstroctions() const;
	void printInvalidChoice() const;
	void printGoodBye() const;
};

