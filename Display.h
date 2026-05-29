#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#define ST7735_ROTATION 1 // 0, 1, 2, 3
#define ST7735_TEXT_SIZE 2

void ST7735_Setup();
void ST7735_Text_Center(const char* fmt, ...);

#endif