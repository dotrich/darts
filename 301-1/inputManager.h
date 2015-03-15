#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <cstdint>

typedef void (*funcPtr)(); // function pointer type for variable array size

class InputManager
{
	private:		
		INPUT_RECORD* inBuffer; // input buffer
		uint16_t keyVk; // vk code of last key event
	public:
		INPUT_RECORD getVkCode(); // gets key record of last key pressed
		~InputManager();

		void inputMenu(funcPtr ptrFuncArray[6]); // simulates menu with passed function array

};

#endif