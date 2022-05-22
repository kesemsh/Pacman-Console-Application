#pragma once

#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "Global.h"
#include "ColorUtilities.h"

class Files
{
private:
	short game_mode;
	short screen_num;

	std::vector<bool> testResults_vec;
	std::vector<std::string> screenFiles_vec;
	std::vector<std::string> stepsFiles_vec;
	std::vector<std::string> resultFiles_vec;

public:
	Files();
	enum gameMode { normal = 0, save = 1, load = 2, silent = 3 };

	short getGameMode() const;
	void cheackArgumentsAndSetGameMode(int, char**);
	void printTestResults() const;
	void checkAndUpdateTests(std::fstream& resultFile, short&);

	void openFiles(std::fstream&, std::fstream&);
	void closeFiles(std::fstream&, std::fstream&);

	void setScreenFilesVec(std::vector<std::string>&);
	void setStepsAndResultFiles();
	void setScreenNum(short);

	const std::string& getScreenFileName(short) const;
	const std::string& getStepsFileName(short) const;
	const std::string& getResultFileName(short) const;
	short getScreenNum() const;
	short getNumOfScreens() const;

	class Error{
	private:
		std::string msg;
		friend class Files;
	public:
		Error(std::string files): msg(files){}
		void getmsg() {
			system("cls");
			std::cout << msg << "\nPress any key to continue.\n"; _getch();
		}
	};
	class InvalidcharsFoundInFile: public Error{
	public:
		InvalidcharsFoundInFile() :Error("Invalid chars found in file.") {}
	};
	class InvalidNumOfPacman : public Error {
	public:
		InvalidNumOfPacman() :Error("Invalid number of Pacman in file.") {}
	};
	class InvalidNumOfGhosts : public Error {
	public:
		InvalidNumOfGhosts() :Error("Invalid number of ghosts in file.") {}
	};
	class InvalidLegend : public Error {
	public:
		InvalidLegend() :Error("Invalid legend in file.") {}
		friend class Files;
	};
	class InvalidScreenSize : public Error {
	public:
		InvalidScreenSize() :Error("Invalid screen size in file.") {}
	};
	class NoScreenFilesFound : public Error {
	public:
		NoScreenFilesFound() :Error("No screen files found.") {}
	};
	class NoStepsOrResultFilesFound : public Error {
	public:
		NoStepsOrResultFilesFound() :Error("No stepts/result files found.") {}
	};
	class InvalidFile : public Error {
	public:
		InvalidFile() :Error("Invalid file.") {}
	};
	class InvalidArgName : public Error {
	public:
		InvalidArgName() :Error("Invalid name for arguments.\nPlease enter one of the following options in the command line:\n-save\n-load\n-load -silent\n") {}
	};
	class InvalidNumOfArg : public Error {
	public:
		InvalidNumOfArg() :Error("Invalid number of arguments.\nPlease enter one of the following options in the command line:\n-save\n-load\n-load -silent\n") {}
	};
};

