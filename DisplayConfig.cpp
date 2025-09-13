#include "DisplayConfig.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool inGame = false;
int  activeGame = -1;
int  currentSelection = 0;

void drawScoreLabel(const char* label, int score, int x, int y) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.print(label);
  display.print(score);
}

bool btnPressed(uint8_t pin) { return !digitalRead(pin); }
