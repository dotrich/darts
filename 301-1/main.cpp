#include "functions.h"

using std::cout;

uint16_t screen = 0; // 0=init, 1=menu, 2=settings, 3=info, 4=quit
uint16_t input = 0; // key pressed
bool lock13 = 0; // enter key

WNDPROC pOriginalWindowProc = nullptr;
HWND hWindow = GetConsoleWindow();

LRESULT APIENTRY CommandProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 13)
	{
		switch (uMsg)
		{
			case WM_KEYDOWN : // locks enter key
				lock13 = true;
				break;
			case WM_KEYUP :
				lock13 = false; // unlocks enter key
				break;
		}
	}

	return CallWindowProc(pOriginalWindowProc, hwnd, uMsg, wParam, lParam); 
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
					"     4  - No of Games      [           ]" << "\n"
					"     5  - Main Menu" << "\n";

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

void scrnMain() // main screen
{
	if (_kbhit())
	{
		input = toupper(_getch());

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
	const uint16_t xMax = 8;
	const uint16_t yMax = 4;

	uint32_t curInt = 0;
	uint32_t temp = 0;

	if (_kbhit() && !lock13)
	{
		input = toupper(_getch());

		switch (yPos) // gets current y pos setting var
		{
			case Y_POS_DEFAULT + 1 :
				curInt = dartsGame.getNoPlayers();
				break;
			case Y_POS_DEFAULT + 2 :
				curInt = dartsGame.getScore();
				break;
			case Y_POS_DEFAULT + 3 :
				curInt = dartsGame.getNoGames();
				break;
		}

		if (input == 72 || input == 80) // resets x pos after moving up/down
			xPos = X_POS_DEFAULT;

		if (input >= 48 && input <= 57) // main keys 0-9
		{
			if ((xPos - 29) + 1 > intLength(curInt)) 
			{
				// WIP: to add/delete digits
			} 
			else if (input == 48 && xPos - 29 == 0) {} // stops zero being added to front
			else
			{
				temp = intModifier(input, xPos - 29, curInt);

				// commits updated setting
				if (curInt == dartsGame.getNoPlayers())
					dartsGame.setNoPlayers(temp);
				else if (curInt == dartsGame.getScore())
					dartsGame.setScore(temp);
				else if (curInt == dartsGame.getNoGames())
					dartsGame.setNoGames(temp);

				updateSettings();
			}
		}

		switch (input)
		{
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
			case 27 : // escape
				// reset cursor and move screen
				GetConsoleCursorInfo(hConsole, &curInfo);
				curInfo.bVisible = 0; // hide cursor
				SetConsoleCursorInfo(hConsole, &curInfo);

				scrnInit(1); // main menu
				xPos = X_POS_DEFAULT;
				yPos = Y_POS_DEFAULT;
				return;
		}

		// hides cursor when on start game option
		GetConsoleCursorInfo(hConsole, &curInfo);

		if (yPos > Y_POS_DEFAULT && yPos < Y_POS_DEFAULT + yMax)
			curInfo.bVisible = 1;
		else
			curInfo.bVisible = 0;			

		SetConsoleCursorInfo(hConsole, &curInfo);

		// clamps x and y positions
		clampCurDraw('y', yPos, yMax);
		clampCurDraw('x', xPos, xMax);	
	}
}

void scrnInfo() // game info
{
	if (_kbhit())
	{
		input = toupper(_getch());

		if (input == 13 || input == 27) // return or escape
			scrnInit(1);
	}
		
}

void scrnGame() // game is played using settings
{
	if (_kbhit())
	{
		input = toupper(_getch());

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
	pOriginalWindowProc = (WNDPROC)SetWindowLong(hWindow, GWL_WNDPROC, (LONG)CommandProc);
	cout << "Console Val: " << (int)hWindow << "\n";

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