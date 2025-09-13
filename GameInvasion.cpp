#include "DisplayConfig.h"

static const int SW = 10, SH = 8;
static int sx, sy, ss = 2;

static const int BW = 2, BH = 4;
static int bx, by, bs = 3;
static bool fire = false;

static const int EW = 12, EH = 8;
static int ex, ey, es = 1, miss = 0;

static int sc = 0;

static bool hit(int ax,int ay,int aw,int ah,int bx,int by,int bw,int bh){
  return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

static int rex() {
  int mn = FIELD_X, mx = FIELD_X + FIELD_W - EW;
  return random(mn, mx + 1);
}

void startInvasionGame() {
  sc = 0; miss = 0; es = 1;
  sx = FIELD_X + (FIELD_W - SW) / 2;
  sy = FIELD_Y + FIELD_H - SH - 2;
  fire = false;
  ex = rex(); ey = FIELD_Y - EH;
}

void invasionLoopTick() {
  if (btnPressed(BTN_LFT)) sx -= ss;
  if (btnPressed(BTN_RHT)) sx += ss;
  if (btnPressed(BTN_UP))  sy -= ss;
  if (btnPressed(BTN_DWN)) sy += ss;

  if (sx < FIELD_X) sx = FIELD_X;
  if (sx + SW > FIELD_X + FIELD_W) sx = FIELD_X + FIELD_W - SW;
  if (sy < FIELD_Y) sy = FIELD_Y;
  if (sy + SH > FIELD_Y + FIELD_H) sy = FIELD_Y + FIELD_H - SH;

  // Fire with SET or MID
  bool fp = btnPressed(BTN_SET) || btnPressed(BTN_MID);
  if (fp && !fire) {
    fire = true;   // <-- lowercase
    bx = sx + SW / 2 - BW / 2;
    by = sy - BH;
  }

  if (btnPressed(BTN_RST)) { inGame = false; delay(150); return; }

  // Enemy moves down
  ey += es;
  if (ey > FIELD_Y + FIELD_H) {
    miss++;
    ex = rex(); ey = FIELD_Y - EH;
    if (miss >= 3) { inGame = false; return; }
  }

  // Bullet moves up
  if (fire) {
    by -= bs;
    if (by + BH < FIELD_Y) fire = false;  // <-- lowercase
  }

  // Bullet hits enemy?
  if (fire && hit(bx,by,BW,BH,ex,ey,EW,EH)) {
    fire = false;   // <-- lowercase
    sc++;
    ex = rex(); ey = FIELD_Y - EH;
    if (sc % 5 == 0 && es < 3) es++;
  }

  // Enemy hits ship?
  if (hit(sx,sy,SW,SH,ex,ey,EW,EH)) { inGame = false; return; }

  // Draw
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  display.fillRect(ex, ey, EW, EH, SSD1306_WHITE);
  // Ship (T-shape)
  display.fillRect(sx + 3, sy, 4, SH, SSD1306_WHITE);
  display.fillRect(sx, sy + SH - 3, SW, 3, SSD1306_WHITE);
  if (fire) display.fillRect(bx, by, BW, BH, SSD1306_WHITE);

  drawScoreLabel("I:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.setCursor(FIELD_X + FIELD_W - 18, FIELD_Y + 2);
  display.print("L:");
  display.print(3 - miss);

  display.display();
}
