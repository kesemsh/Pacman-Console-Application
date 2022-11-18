# Pacman Console Application
This project was created during C++ course in The Academic College of Tel Aviv, Yaffo, 2022.

In this game you can use a custom screen using a file format and run tests with command arguments and more file formats which are all mentioned below.

# Game Keys:
A: Left

W: Up

S: Stay

X: Down

D: Right

# Command Arguments
The Steps and Result files are for runing tests on the game.
in the command line add desired mode: -load|-save [-silent]

Game modes:
1. load mode - No menu, will run the loaded game as is from the Steps file and compare the current results to the Result file.

2. save mode - Steps and Result files are created for each Screen file you play. You can then load them later for testing.

3. silent mode - Avoids printing to the screen. Note: this mode can only be added to load mode otherwise it will be ignored.

# Screen File Format:
'#' - Walls.

'%' - Empty space.

' ' - Breadcrumbs.

'&' - Legend location.

'$' - Ghost location (can be 0 - 4 ghosts).

'@' - Pacman location.

Empty spaces along the walls will be tunnels.
Note: if a tunnel is added another tunnel must be added to the wall across from it to pass through it.

# Steps File Format:
The first line is the fruit's starting position:
": x,y"

For pacman use the game keys for directions:
": W" for up.
": X" for down.
": A" for left.
": D" for right.
": S" for stay.

For ghosts and the fruit use the directions:
": 0" for up.
": 1" for down.
": 2" for left.
": 3" for right.

Note: Each ghost will appear with it's number (1 - 4).

# Result File Format:
In this file are only two possible lines:

-"dies: <Time>" - indicating the point of time the pacman died on this screen.

-"wins: <Time>" - indicating the point of time the pacman finished the screen successfully.


**Note For All Formats: It's important to keep the end of each line as obove**
