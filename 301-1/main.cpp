#include <iostream>
#include <conio.h>
#include <windows.h>
#include "darts.h"

#include <stdio.h>
#include <stdlib.h>

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

// unused code
//#include <map>
//std::map<char, uint16_t> POS_DEFAULT;
//POS_DEFAULT[axis] = NULL;

// function pointer array
//typedef void (*FuncPointer)(uint16_t, uint16_t);
//
//FuncPointer scrnArray[4] = {NULL};
//scrnArray[0] = &scrnMenu;





DWORD g_main_tid = 0;
HHOOK g_kb_hook = 0;

BOOL CALLBACK con_handler(DWORD)
{
	PostThreadMessage(g_main_tid, WM_QUIT, 0, 0);
	return TRUE;
};

LRESULT CALLBACK kb_proc(int code, WPARAM w, LPARAM l)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)l;
	const char *info = NULL;

	if (w == WM_KEYUP)
	{
		if (l == 13)
			cout << WH_KEYBOARD_LL << "\n";
		//printf ("%s - vkCode [%04x], scanCode [%04x]\n", info, p->vkCode, p->scanCode);
	}

	// always call next hook
	return CallNextHookEx(g_kb_hook, code, w, l);
};
 
int main (void)
{
	g_main_tid = GetCurrentThreadId ();
	SetConsoleCtrlHandler(&con_handler, TRUE);
	g_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, 
					&kb_proc,
					GetModuleHandle (NULL), // cannot be NULL, otherwise it will fail
					0);

	if (g_kb_hook == NULL)
	{
		fprintf (stderr, "SetWindowsHookEx failed with error %d\n", ::GetLastError ());
		return 0;
	};
       
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};
	UnhookWindowsHookEx(g_kb_hook);
	return 0;
};















void setGraphics()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // used to reference console

	// set cursor info, full size, hidden by default
	GetConsoleCursorInfo(hConsole, &curInfo); // need to retrieve cursor info first
	curInfo.dwSize = 100;
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(hConsole, &curInfo); // sets once finished
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
		
	if (toupper(axis) == 'X') // moves x cursor after clamp
		drawString(xPos, yPos, "");
	else if (toupper(axis) == 'Y') // removes old and adds new y cursor
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

void updateSettings() // refreshes settings on game settings screen
{
	drawString(29, 15, std::to_string((long long unsigned)dartsGame.getNoPlayers() ));
	drawString(29, 16, std::to_string((long long unsigned)dartsGame.getScore() ));
	drawString(29, 17, std::to_string((long long unsigned)dartsGame.getNoGames() ));
}

int64_t intLength(int64_t var)
{
	uint16_t len = 0;

	while (var != 0) // finds length of var
	{
		var /= 10;
		len++;
	}

	return len;
}

uint32_t intModifier(uint16_t input, uint16_t xPos, uint32_t var)
{
	uint32_t incNo = 0;
	uint32_t decNo = 0;
	uint32_t modNo = 0;
	uint16_t len = intLength(var);

	for (uint16_t x = 48; x <= 57; x++) // converts ascii 48-57 input into equivelent decimal 0-9
	{
		if (x == input)
		{
			incNo = x - 48; // gets increment digit
			break;
		}		
	}

	modNo = pow((float)10, len - xPos); // division number for modulo
	decNo = (var % modNo) / pow((float)10, len - (xPos + 1)); // gets decrement digit

	incNo = incNo * (modNo / 10); // new value for swapping in
	decNo = decNo * (modNo / 10); // old value for swapping out
	
	return (var - decNo) + incNo; // swaps
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
	const uint16_t xMax = 8;
	const uint16_t yMax = 4;

	uint32_t curInt = 0;
	uint32_t temp = 0;

	if (_kbhit())
	{
		input = toupper(getch());

		switch (yPos) // gets current y pos setting
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
			//case 13 : // return
			//	break;
			case WM_KEYUP :
				//if (wParam == 13)
				//	swprintf_s(msg, L"WM_KEYUP: 0x%x\n", wParam);
				//	OutputDebugString(msg);
				//else
				//	break;

				//if (WM_KEYUP)
				//{
				//	switch (yPos)
				//	{	
				//		case Y_POS_DEFAULT :
				//			// WIP: to start game
				//			return;
				//		case Y_POS_DEFAULT + yMax : ;
				//			// main menu, falls down
				//	}
				//}
				//else
				//	break;
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