#include <Wire.h>
#include "DisplayConfig.h"

// Declarations
void startSnakeGame();   void snakeLoopTick();
void startTetrisGame();  void tetrisLoopTick();
void startPongGame();    void pongLoopTick();
void startBreakoutGame();void breakoutLoopTick();
void startFlappyGame();  void flappyLoopTick();
void startInvasionGame();void invasionLoopTick();
void startFroggerGame(); void froggerLoopTick();
void startPacmanGame();  void pacmanLoopTick();

const char* menuItems[] = {
  "Snake","Tetris","Pong","Breakout","Flappy","Invasion","Frogger","Pacman"
};
const int menuLength = sizeof(menuItems)/sizeof(menuItems[0]);

void drawMenu(){
  display.clearDisplay();
  display.drawRect(FIELD_X-1,FIELD_Y-1,FIELD_W+2,FIELD_H+2,SSD1306_WHITE);
  display.fillRect(FIELD_X+3,FIELD_Y+4,54,20,SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK); display.setTextSize(1);
  display.setCursor(FIELD_X+17,FIELD_Y+6); display.print("Mini");
  display.setCursor(FIELD_X+13,FIELD_Y+14);display.print("Arcade");
  display.setTextColor(SSD1306_WHITE); display.setCursor(FIELD_X+8,FIELD_Y+26); display.print("by r03n");
  for(int i=0;i<menuLength;i++){ int y=FIELD_Y+48+i*9; display.setCursor((i==currentSelection)?FIELD_X+3:FIELD_X+10,y); if(i==currentSelection) display.print(">"); display.print(menuItems[i]);}
  display.display();
}

void handleMenuInput(){
  if(btnPressed(BTN_UP))  { currentSelection=(currentSelection-1+menuLength)%menuLength; delay(140); }
  if(btnPressed(BTN_DWN)) { currentSelection=(currentSelection+1)%menuLength; delay(140); }
  if(btnPressed(BTN_MID)) {
    inGame=true; activeGame=currentSelection;
    switch(activeGame){
      case 0: startSnakeGame();    break;
      case 1: startTetrisGame();   break;
      case 2: startPongGame();     break;
      case 3: startBreakoutGame(); break;
      case 4: startFlappyGame();   break;
      case 5: startInvasionGame(); break;
      case 6: startFroggerGame();  break;
      case 7: startPacmanGame();   break;
    }
  }
  if(btnPressed(BTN_RST)) currentSelection=0;
}

void setup(){
  pinMode(BTN_UP,INPUT_PULLUP); pinMode(BTN_DWN,INPUT_PULLUP);
  pinMode(BTN_LFT,INPUT_PULLUP); pinMode(BTN_RHT,INPUT_PULLUP);
  pinMode(BTN_MID,INPUT_PULLUP); pinMode(BTN_SET,INPUT_PULLUP);
  pinMode(BTN_RST,INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.setRotation(3);
  display.clearDisplay(); display.display();
  drawMenu();
}

void loop(){
  if(inGame){
    switch(activeGame){
      case 0: snakeLoopTick();    break;
      case 1: tetrisLoopTick();   break;
      case 2: pongLoopTick();     break;
      case 3: breakoutLoopTick(); break;
      case 4: flappyLoopTick();   break;
      case 5: invasionLoopTick(); break;
      case 6: froggerLoopTick();  break;
      case 7: pacmanLoopTick();   break;
    }
  } else {
    handleMenuInput();
    drawMenu();
  }
}
