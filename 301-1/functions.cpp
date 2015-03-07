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

int16_t intLength(int64_t var)
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

/*// unused code

//#include <map>
//std::map<char, uint16_t> POS_DEFAULT;
//POS_DEFAULT[axis] = nullptr;

// function pointer array
//typedef void (*FuncPointer)(uint16_t, uint16_t);
//
//FuncPointer scrnArray[4] = {nullptr};
//scrnArray[0] = &scrnMenu;*/