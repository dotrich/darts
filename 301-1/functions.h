#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <iostream>
#include <string>
#include <conio.h>
#include <cstdint>

#include "globals.h"

const uint16_t X_POS_DEFAULT = 29;
const uint16_t Y_POS_DEFAULT = 14;

void setGraphics();
void drawString(uint16_t x,uint16_t y, std::string out);
void clampCurDraw(char axis, uint16_t no, uint16_t max);
void updateSettings();

int16_t intLength(int64_t var);
uint32_t intModifier(uint16_t input, uint16_t xPos, uint32_t var);

#endif