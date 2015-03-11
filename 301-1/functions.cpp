#include "functions.h"

using std::string;
using std::cout;
using std::cin;

void setGraphics()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // used to reference console

	// set cursor info, full size, hidden by default
	GetConsoleCursorInfo(hConsole, &curInfo); // need to retrieve cursor info first
	curInfo.dwSize = 100;
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(hConsole, &curInfo); // sets once finished
};

void drawString(uint16_t x,uint16_t y, string out)
{
	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(hConsole, cursorPos);

	if (out != "") // allows only cursor changing if no string entered
		cout << out;
};

void clampCurDraw(char axis, uint16_t no, uint16_t max)
{
	if (toupper(axis) == 'X')
	{
		xPos = clamp(xPos, X_POS_DEFAULT, X_POS_DEFAULT + max); // clamp x
		drawString(xPos, yPos, ""); // moves x cursor
	}
	else if (toupper(axis) == 'Y') // removes old and adds new y cursor
	{
		yPos = clamp(yPos, Y_POS_DEFAULT, Y_POS_DEFAULT + max); // clamp y

		for (int x = 0; x < max+1; x++) // removes old cursor
		{
			drawString(4, Y_POS_DEFAULT + x, " ");
			drawString(6, Y_POS_DEFAULT + x, " ");
		}

		drawString(4, yPos, "["); // adds new cursor
		drawString(6, yPos, "]");
	}
};

void updateSettings() // refreshes settings on game settings screen
{
	for (uint16_t x = 15; x < 18; x++)
		drawString(29, x, "         ");

	drawString(29, 15, std::to_string((long long unsigned)dartsGame.getNoPlayers() ));
	drawString(29, 16, std::to_string((long long unsigned)dartsGame.getScore() ));
	drawString(29, 17, std::to_string((long long unsigned)dartsGame.getNoGames() ));
};

int16_t intLength(int64_t var)
{
	uint16_t len = 0;

	while (var != 0) // finds length of var
	{
		var /= 10;
		len++;
	}

	return len;
};

uint32_t intModifier(uint16_t input, uint16_t xPos, uint32_t var) // uses decimal input to modify digits of var
{
	uint32_t incNo, decNo;
	uint32_t modNo, modResult;
	uint16_t len = intLength(var);

	for (uint16_t x = 48; x <= 57; x++) // converts vkcodes 48-57 input into equivelent decimal 0-9
	{
		if (x == input)
		{
			incNo = x - 48; // gets increment digit
			break;
		}		
	}

	if (input == 46) // delete
	{
		if (xPos == len) // last digit
		{
			return (uint32_t)(var / 10);
		}
		else if (xPos <= len) // all digits inbetween
		{
			/* example:
			9876 starting no
			digit 8 selected for deletion */
			
			modNo = pow((float)10, len - xPos); // gets modulo for 876
			modResult = var % modNo; // uses modNo, equals 876
			decNo = (var - modResult) / 10; // takes 876 from starting no, divides by 10, equals 900

			modNo = pow((float)10, len - (xPos + 1)); // gets modulo for 76
			modResult = var % modNo; // uses modNo, equals 76
			incNo = decNo + modResult; // adds decNo and modResult, equals 976, 8 has been removed

			return incNo;
		}
	}
	else if (input == 48 && xPos >= len)
	{
		return (uint32_t)(var * 10); // adding zero to end
	}
	else if (input >= 49 && input <= 57 && xPos >= len)
	{
		return (uint32_t)((var * 10) + incNo);
	}
	else // modifying digit
	{
		modNo = pow((float)10, len - xPos); // division number for modulo
		decNo = (var % modNo) / pow((float)10, len - (xPos + 1)); // gets decrement digit

		incNo = incNo * (modNo / 10); // new value for swapping in
		decNo = decNo * (modNo / 10); // old value for swapping out
	
		return (var - decNo) + incNo; // swaps
	}
};

/*// unused code

//#include <map>
//std::map<char, uint16_t> POS_DEFAULT;
//POS_DEFAULT[axis] = nullptr;

// function pointer array
//typedef void (*FuncPointer)(uint16_t, uint16_t);
//
//FuncPointer scrnArray[4] = {nullptr};
//scrnArray[0] = &scrnMenu;*/

// non-functional input hook
//WNDPROC pOriginalWindowProc = nullptr;
//HWND hWindow = GetConsoleWindow();

//LRESULT APIENTRY CommandProc(
//	HWND hwnd, 
//	UINT uMsg, 
//	WPARAM wParam, 
//	LPARAM lParam)
//{
//	if (wParam == 13)
//	{
//		switch (uMsg)
//		{
//			case WM_KEYDOWN :
//				cout << "TURKEY" << "\n";
//				break;
//			case WM_KEYUP :
//				cout << "BUTTS" << "\n";
//				break;
//		}
//	}
//
//	return CallWindowProc(pOriginalWindowProc, hwnd, uMsg, wParam, lParam); 
//};

//pOriginalWindowProc = (WNDPROC)SetWindowLong(hWindow, GWL_WNDPROC, (LONG)CommandProc);
//MSG msg;
//cout << "Console Val: " << (int)hWindow << "\n";
//
//while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
//{
//	TranslateMessage(&msg);
//	DispatchMessage(&msg);
//}