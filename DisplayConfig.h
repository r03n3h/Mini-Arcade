#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

extern Adafruit_SSD1306 display;

#define SEG_SIZE   6
#define FIELD_X    2
#define FIELD_Y    2
#define FIELD_W    60
#define FIELD_H    120
#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define BTN_UP   32
#define BTN_DWN  33
#define BTN_LFT  25
#define BTN_RHT  26
#define BTN_MID  27
#define BTN_SET  14
#define BTN_RST  12

extern bool inGame;
extern int  activeGame;
extern int  currentSelection;

void drawScoreLabel(const char* label, int score, int x, int y);
bool btnPressed(uint8_t pin);

#endif
