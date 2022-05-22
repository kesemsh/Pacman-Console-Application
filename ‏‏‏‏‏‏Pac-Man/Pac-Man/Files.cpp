#include "Files.h"

Files::Files()//constractor that sets the board information
{
	game_mode = -1;
	screen_num = 0;

	try {
		setScreenFilesVec(screenFiles_vec);
	}
	catch (NoScreenFilesFound& e){
		e.getmsg();
	}
}

const std::string& Files::getScreenFileName(short i) const 
{
	return screenFiles_vec[i];
}

const std::string& Files::getStepsFileName(short i) const
{
	return stepsFiles_vec[i];
}

const std::string& Files::getResultFileName(short i) const
{
	return resultFiles_vec[i];
}

short Files::getScreenNum() const 
{
	return screen_num;
}

short Files::getNumOfScreens() const 
{
	return (short)screenFiles_vec.size();
}

void Files::setScreenFilesVec(std::vector<std::string>& fileNames) 
{
	std::string extention = ".screen";
	std::string path = std::filesystem::current_path().string();

	for (auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == extention)
		{
			fileNames.push_back(entry.path().stem().string() + extention);
		}
	}

	if (fileNames.size() == 0)
	{
		throw NoScreenFilesFound();
	}
	else
	{
		std::sort(fileNames.begin(), fileNames.end());
		
		setStepsAndResultFiles();
	}
}

void Files::setScreenNum(short n)
{
	screen_num = n;
}

void Files::setStepsAndResultFiles()
{
	short len;
	short size = (short)screenFiles_vec.size();

	stepsFiles_vec.resize(size);
	resultFiles_vec.resize(size);

	for (short i = 0; i < size; i++)
	{
		len = (short)screenFiles_vec[i].length();
		len -= 6;

		for (short j = 0; j < len; j++)
		{
			stepsFiles_vec[i].push_back(screenFiles_vec[i][j]);
			resultFiles_vec[i].push_back(screenFiles_vec[i][j]);
		}

		std::stringstream str1;
		str1 << stepsFiles_vec[i] << "steps";
		str1 >> stepsFiles_vec[i];

		std::stringstream str2;
		str2 << resultFiles_vec[i] << "result";
		str2 >> resultFiles_vec[i];
	}
}

short Files::getGameMode() const
{
	return game_mode;
}

void Files::cheackArgumentsAndSetGameMode(int argc, char** argv)
{
	switch (argc)
	{
	case 1:
		game_mode = Files::gameMode::normal;
		break;
	case 2:
		if (std::strcmp("-save", argv[1]) == 0)
			game_mode = Files::gameMode::save;

		else if (std::strcmp("-load", argv[1]) == 0)
			game_mode = Files::gameMode::load;
		else
			throw InvalidArgName();
		break;
	case 3:
		if (std::strcmp("-load", argv[1]) == 0 && std::strcmp("-silent", argv[2]) == 0)
			game_mode = Files::gameMode::silent;
		else if (std::strcmp("-save", argv[1]) == 0 && std::strcmp("-silent", argv[2]) == 0)
			game_mode = Files::gameMode::save;
		else
			throw InvalidArgName();
		break;
	default:
		throw InvalidNumOfArg();
	}
}

void Files::printTestResults() const
{
	gotoxy(0, 0);

	for (short i = 0; i < testResults_vec.size(); i++)
	{
		if (testResults_vec[i])
			std::cout << "Screen " << i + 1 << " Test passed." << std::endl;
		else
			std::cout << "Screen " << i + 1 << " Test failed." << std::endl;
	}
}

void Files::checkAndUpdateTests(std::fstream& resultFile, short& PointOfTime)
{
	bool isTestPassed = true;
	short count = 0;
	short num;
	short found1, found2;
	std::string line, numFromFile;

	getline(resultFile, line);
	found1 = (short)line.find("dies: ");
	found2 = (short)line.find("wins: ");

	if (found1 == -1) //wins
	{
		numFromFile = line.substr(found2 + 6, line.size() - (found2 + 6));
	}
	else //dies
	{
		numFromFile = line.substr(found1 + 6, line.size() - (found1 + 6));
	}

	std::stringstream numStr(numFromFile);
	numStr >> num;

	if (num != PointOfTime)
		isTestPassed = false;

	testResults_vec.push_back(isTestPassed);
}

void Files::openFiles(std::fstream& stepsFile, std::fstream& resultFile)
{
	if (game_mode != gameMode::normal)
		if (stepsFiles_vec.size() == 0 || resultFiles_vec.size() == 0)
			throw NoStepsOrResultFilesFound();

	if (game_mode == gameMode::save)
	{
		stepsFile.open(stepsFiles_vec[screen_num], std::ios::out);
		resultFile.open(resultFiles_vec[screen_num], std::ios::out);
	}
	else if (game_mode == gameMode::load || game_mode == gameMode::silent)
	{
		stepsFile.open(stepsFiles_vec[screen_num], std::ios::in);
		resultFile.open(resultFiles_vec[screen_num], std::ios::in);

		if(!stepsFile || !resultFile)
			throw NoStepsOrResultFilesFound();
	}
}

void Files::closeFiles(std::fstream& stepsFile, std::fstream& resultFile)
{
	if (game_mode != gameMode::normal)
	{
		stepsFile.close();
		resultFile.close();
	}
}