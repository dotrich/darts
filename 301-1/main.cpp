#include "functions.h"
using std::cout;

typedef void (*funcPtr)();

uint16_t screen = 0; // 0=init, 1=menu, 2=settings, 3=info, 4=quit
uint16_t input = 0; // key pressed

HANDLE hStdIn;
funcPtr scrnMainCluster[6];
funcPtr scrnInfoCluster[6];
funcPtr scrnSettingsCluster[6];

// INPUT MANAGER
void inputMenu(funcPtr ptrFuncArray[6])
{ 
	DWORD inRecRead, i;
	INPUT_RECORD inBuffer[128]; // input buffer
	KEY_EVENT_RECORD key; // individual key; record
	uint16_t keyVk; // individual key; vk code

	hStdIn = GetStdHandle(STD_INPUT_HANDLE); // input buffer handle
 
	ReadConsoleInput( 
        hStdIn, // input buffer handle 
        inBuffer, // buffer
        128, // size of buffer
        &inRecRead); // records read

	static uint16_t keys[6] = {
		0x25, // left
		0x26, // up
		0x27, // right
		0x28, // down
		0x0D, // return
		0x1B} ; // escape
 	
	for (i = 0; i < inRecRead; i++) // searches records
	{						
		switch (inBuffer[i].EventType) // keyboard, mouse etc
		{
			case KEY_EVENT : // keyboard event
				key = inBuffer[i].Event.KeyEvent; // current key record
				keyVk = inBuffer[i].Event.KeyEvent.wVirtualKeyCode; // current virtual key code

				input = keyVk; // local key press

				if (!key.bKeyDown) // release event
				{
					for (uint16_t x = 0; x < 6; x++)
					{
						if (keyVk == keys[x])
						{
							if (ptrFuncArray[x] != nullptr)
								ptrFuncArray[x]();
						}
					}
				}
		}
	}
};

// SCREENS
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
	
};

void scrnMain() // main screen
{
	inputMenu(scrnMainCluster);

	if (screen == 1)
	{
		// clamps x and y cursors
		clampCurDraw('y', yPos, 2);
		clampCurDraw('x', xPos, 0);
	}
};

void scrnSettings() // game settings prior to playing
{
	const uint16_t xMax = 8;
	const uint16_t yMax = 4;

	uint32_t curInt = 0;
	uint32_t temp = 0;
	
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

	inputMenu(scrnSettingsCluster); // input manager

	// setting modifiers
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

	// hides cursor when on start game option
	GetConsoleCursorInfo(hConsole, &curInfo);

	if (yPos > Y_POS_DEFAULT && yPos < Y_POS_DEFAULT + yMax)
		curInfo.bVisible = 1;
	else
		curInfo.bVisible = 0;

	SetConsoleCursorInfo(hConsole, &curInfo);

	// clamps x and y cursors
	clampCurDraw('y', yPos, yMax);
	clampCurDraw('x', xPos, xMax);
};

void scrnInfo() // game info
{
	inputMenu(scrnInfoCluster);
};

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
};

// MENU CLUSTERS - used by input manager
// general
void menuLeft() { xPos--; };
void menuUp()
{
	yPos--;
	xPos = X_POS_DEFAULT;
};

void menuRight() { xPos++; };
void menuDown()
{
	yPos++;
	xPos = X_POS_DEFAULT;
};

// scrnMain
void menuMainReturn()
{
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
};

void menuMainEscape()
{
	switch (yPos)
	{
		case Y_POS_DEFAULT + 2 :
			screen = 5; // quit
			break;
	}
};

// scrnInfo
void menuInfoReturn()
{
	scrnInit(1);
};

// scrnSettings
void menuSettingsEscape()
{
	// reset cursor and move screen
	GetConsoleCursorInfo(hConsole, &curInfo);
	curInfo.bVisible = 0; // hide cursor
	SetConsoleCursorInfo(hConsole, &curInfo);

	scrnInit(1); // main menu
	xPos = X_POS_DEFAULT;
	yPos = Y_POS_DEFAULT;
};

void menuSettingsReturn()
{
	switch (yPos)
	{
		case Y_POS_DEFAULT :
			break;
		case Y_POS_DEFAULT + 4 :
			menuSettingsEscape();
			break;
	}
};

// MAIN
void main()
{
	// CLUSTER SETUP
	// scrnMain
	scrnMainCluster[0] = &menuLeft;
	scrnMainCluster[1] = &menuUp;
	scrnMainCluster[2] = &menuRight;
	scrnMainCluster[3] = &menuDown;
	scrnMainCluster[4] = &menuMainReturn;
	scrnMainCluster[5] = &menuMainEscape;

	// scrnInfo
	for (uint16_t x = 0; x < 4; x++)
		scrnInfoCluster[x] = nullptr;

	scrnInfoCluster[4] = &menuInfoReturn;
	scrnInfoCluster[5] = &menuInfoReturn;

	// scrnSettings
	scrnSettingsCluster[0] = &menuLeft;
	scrnSettingsCluster[1] = &menuUp;
	scrnSettingsCluster[2] = &menuRight;
	scrnSettingsCluster[3] = &menuDown;
	scrnSettingsCluster[4] = &menuSettingsReturn;
	scrnSettingsCluster[5] = &menuSettingsEscape;
	
	// ENTRY POINT
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
};