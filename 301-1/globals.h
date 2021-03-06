#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <windows.h>

#include "darts.h"
#include "inputManager.h"

extern Darts dartsGame;
extern InputManager inputManager;

extern CONSOLE_CURSOR_INFO curInfo;
extern HANDLE hConsole;
extern COORD cursorPos;

extern uint16_t xPos;
extern uint16_t yPos;

#endif