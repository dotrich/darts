#include <iostream>
#include <conio.h>
#include <windows.h>
#include <map>
#include "darts.h"

using std::cout;
using std::cin;
using std::string;

const uint16_t Y_POS_DEFAULT = 14; // cursor y position
const uint16_t X_POS_DEFAULT = 29; // cursor x position
static uint16_t yPos = Y_POS_DEFAULT; // menu cursor y position
static uint16_t xPos = X_POS_DEFAULT; // menu cursor x position

Darts dartsGame;

uint16_t screen = 0; // 0=init, 1=menu, 2=settings, 3=info, 4=quit
uint16_t input = 0;

CONSOLE_CURSOR_INFO curInfo;
HANDLE hConsole;
COORD cursorPos;

//std::map<char, uint16_t> POS_DEFAULT;
//POS_DEFAULT[axis] = NULL;

//typedef void (*FuncPointer)(uint16_t, uint16_t);
//
//FuncPointer scrnArray[4] = {NULL};
//scrnArray[0] = &scrnMenu;

void setGraphics()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hConsole, &curInfo);
	curInfo.dwSize = 100;
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(hConsole, &curInfo);
}

void drawString(uint16_t x,uint16_t y, string out)
{
	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(hConsole, cursorPos);

	if (out != "") // allows only cursor changing if no string entered
		cout << out;
}

void clampCurDraw(char axis, uint16_t no, uint16_t max)
{
	// clamps x or y axis
	if (toupper(axis) == 'X')
		xPos = clamp(xPos, X_POS_DEFAULT, X_POS_DEFAULT + max);
	else if (toupper(axis) == 'Y')
		yPos = clamp(yPos, Y_POS_DEFAULT, Y_POS_DEFAULT + max);
		
	if (toupper(axis) == 'X') // moves cursor after clamp
		drawString(xPos, yPos, "");
	else if (toupper(axis) == 'Y') // removes old and adds new cursor
	{
		for (int x = 0; x < max+1; x++) // removes old cursor
		{
			drawString(4, Y_POS_DEFAULT + x, " ");
			drawString(6, Y_POS_DEFAULT + x, " ");
		}

		drawString(4, yPos, "["); // adds new cursor
		drawString(6, yPos, "]");
	}
}

void updateSettings()
{
	drawString(29, 15, std::to_string((long long unsigned)dartsGame.getNoPlayers() ));
	drawString(29, 16, std::to_string((long long unsigned)dartsGame.getScore() ));
	drawString(29, 17, std::to_string((long long unsigned)dartsGame.getNoGames() ));
}

void scrnInit(uint16_t scrn) // changes screen and sets up UI
{
	system("CLS");
	drawString(0, 0, "");
	screen = scrn;

	if (scrn >= 1 && scrn <= 3)
		cout << "\n\n"
				"    8888888b.        d8888 8888888b. 88888888888 .d8888b.  " << "\n"
				"    888  'Y88b      d88888 888   Y88b    888    d88P  Y88b " << "\n"
				"    888    888     d88P888 888    888    888    Y88b.      " << "\n"
				"    888    888    d88P 888 888   d88P    888     'Y888b.   " << "\n"
				"    888    888   d88P  888 8888888P'     888        'Y88b. " << "\n"
				"    888    888  d88P   888 888 T88b      888          '888 " << "\n"
				"    888  .d88P d8888888888 888  T88b     888    Y88b  d88P " << "\n"
				"    8888888P' d88P     888 888   T88b    888     'Y8888P'  " << "\n"
				"\n\n";

	drawString(0, 12, "");

	switch (scrn)
	{
		case 1 :
			cout << "    Main Menu - \n"
					"\n"
					"    [1] - Play Darts" << "\n"
					"     2  - Information" << "\n"
					"     3  - Quit" << "\n";
			break;
		case 2 :
			cout << "    Game Settings - \n"
					"\n"
					"    [1] - Start Game" << "\n"
					"     2  - No of Players    [           ]" << "\n"
					"     3  - Starting Score   [           ]" << "\n"
					"     4  - No of Games      [           ]" << "\n";

			updateSettings();
			drawString(X_POS_DEFAULT, Y_POS_DEFAULT, "");

			break;
		case 3 :
			cout << "    Game Information - " << "\n"
					"\n"
					"    This version of darts is based on darts301 and allows" << "\n"
					"    1 to 1000 opponents to compete. Score can be set" << "\n"
					"    from 101 to 30001 and 1 to 100 million games can be" << "\n"
					"    played in any one session." << "\n"
					"\n"
					"    [1] - Back";
			break;
	}
	
}

void scrnMain() // main menu (which screen, max options)
{
	if (_kbhit())
	{
		input = toupper(getch());

		switch (input)
		{
			case 72 : // up
				yPos--;
				break;
			case 80 : // down
				yPos++;
				break;
			case 13 : // return
				switch (yPos)
				{
					case Y_POS_DEFAULT :
						scrnInit(2); // game settings
						break;
					case Y_POS_DEFAULT + 1 :
						scrnInit(3); // information
						break;
					case Y_POS_DEFAULT + 2 :
						screen = 5; // quit
						break;
				}

				yPos = Y_POS_DEFAULT;
				return;
		}
				
		clampCurDraw('y', yPos, 2);
	}
}

void scrnSettings() // game settings prior to playing
{
	if (_kbhit())
	{
		input = toupper(getch());

		switch (input)
		{
			case 72 || 80 : // up or down
				xPos = X_POS_DEFAULT;
				cout << "HELP";
				break;
			case 72 : // up
				yPos--;
				break;
			case 80 : // down
				yPos++;
				break;
			case 75 : // left
				xPos--;				
				break;
			case 77 : // right
				xPos++;
				break;
			case 13 : // return				
				if (GetAsyncKeyState(13) && (yPos == Y_POS_DEFAULT))
				{	
					//WIP									
					yPos = Y_POS_DEFAULT;
					return;
				}

				break;
			case 27 : // escape
				GetConsoleCursorInfo(hConsole, &curInfo);
				curInfo.bVisible = 0;
				SetConsoleCursorInfo(hConsole, &curInfo);

				scrnInit(1);
				xPos = X_POS_DEFAULT;
				yPos = Y_POS_DEFAULT;
				return;
		}

		GetConsoleCursorInfo(hConsole, &curInfo);

		if (yPos > Y_POS_DEFAULT)			
			curInfo.bVisible = 1;
		else
			curInfo.bVisible = 0;			

		SetConsoleCursorInfo(hConsole, &curInfo);

		drawString(X_POS_DEFAULT, yPos, "");
		clampCurDraw('y', yPos, 3);
		clampCurDraw('x', xPos, 8);
	}
}

void scrnInfo() // game info
{
	if (_kbhit())
	{
		input = toupper(getch());

		if (input == 13 || input == 27) // return or escape
			scrnInit(1);
	}
		
}

void scrnGame() // game is played using settings
{
	if (_kbhit())
	{
		input = toupper(getch());

		if (input == 13 || input == 27) // return or escape
			scrnInit(1);
	}

	dartsGame.setNoPlayers(dartsGame.getNoPlayers());
	dartsGame.setScore(dartsGame.getScore());
	dartsGame.setNoGames(dartsGame.getNoGames());

	Sleep(1000);
	system("CLS");
	dartsGame.playGame();
}

void main()
{
	do
	{
		switch(screen)
		{
			case 0 :
				setGraphics();
				scrnInit(1);
				break;
			case 1 :
				scrnMain(); // main menu
				break;
			case 2 :
				scrnSettings(); // game settings
				break;
			case 3 :
				scrnInfo(); // game info
				break;
			case 4 :
				scrnGame(); // game
				break;
		}
	}
	while (screen != 5);
}