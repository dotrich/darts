#include <iostream>
#include <array>
#include <Windows.h>

#include "inputManager.h"

HANDLE hStdIn;

uint16_t InputManager::getVkCode() // supports keyboard events only, for now
{
	DWORD inRecRead, i;
	INPUT_RECORD inBuffer[1]; // input buffer
	uint16_t input;

	hStdIn = GetStdHandle(STD_INPUT_HANDLE); // input buffer handle
 
	ReadConsoleInput( 
        hStdIn, // input buffer handle 
        inBuffer, // buffer
        1, // size of buffer
        &inRecRead); // records read
	
	return inBuffer[0].Event.KeyEvent.wVirtualKeyCode;
};

void InputManager::inputMenu(funcPtr ptrFuncArray[6])
{ 
	//system("CLS");
	//std::cout << "Error - ptrFuncArray's size must be 6:" << "\n\n"
	//				"		 Index Corrosponds to Key:" << "\n"
	//				"		 [0] = 0x25" << "\n"
	//				"		 [1] = 0x26" << "\n"
	//				"		 [2] = 0x27" << "\n"
	//				"		 [3] = 0x28" << "\n"
	//				"		 [4] = 0x0D" << "\n"
	//				"		 [5] = 0x1B" << "\n";

	DWORD inRecRead, i;
	KEY_EVENT_RECORD key; // individual key; record
	inBuffer = new INPUT_RECORD[1]; // input buffer

	hStdIn = GetStdHandle(STD_INPUT_HANDLE); // input buffer handle
 
	ReadConsoleInput( 
		hStdIn, // input buffer handle 
		inBuffer, // buffer
		1, // size of buffer
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