#include "DisplayConfig.h"
static const int FW = 6, FH = 6;
static int fx, fy, sc = 0;
struct Car {
  int x, y, w, h, dir, s;
};
static const int LANES = 4;
static Car c[LANES * 2];
static int ly(int i) {
  int top = FIELD_Y + 16, sp = 18;
  return top + i * sp;
}
static void initC() {
  int k = 0;
  for (int l = 0; l < LANES; l++) {
    int y = ly(l), dir = (l % 2 == 0) ? 1 : -1, w = 10, h = 6, s = 1 + (l % 3);
    c[k++] = { FIELD_X + (dir > 0 ? 0 : FIELD_W - w), y, w, h, dir, s };
    c[k++] = { FIELD_X + FIELD_W / 2, y, w, h, dir, s };
  }
}
void startFroggerGame() {
  fx = FIELD_X + FIELD_W / 2 - FW / 2;
  fy = FIELD_Y + FIELD_H - FH - 2;
  initC();
}
void froggerLoopTick() {
  if (btnPressed(BTN_LFT)) fx -= 2;
  if (btnPressed(BTN_RHT)) fx += 2;
  if (btnPressed(BTN_UP)) fy -= 6;
  if (btnPressed(BTN_DWN)) fy += 6;
  if (btnPressed(BTN_RST)) {
    inGame = false;
    delay(150);
    return;
  }
  if (fx < FIELD_X) fx = FIELD_X;
  if (fx + FW > FIELD_X + FIELD_W) fx = FIELD_X + FIELD_W - FW;
  if (fy < FIELD_Y) fy = FIELD_Y;
  if (fy + FH > FIELD_Y + FIELD_H) fy = FIELD_Y + FIELD_H - FH;
  auto hit = [&](int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
  };
  for (int i = 0; i < LANES * 2; i++) {
    c[i].x += c[i].dir * c[i].s;
    if (c[i].dir > 0 && c[i].x > FIELD_X + FIELD_W) c[i].x = FIELD_X - c[i].w;
    if (c[i].dir < 0 && c[i].x + c[i].w < FIELD_X) c[i].x = FIELD_X + FIELD_W;
    if (hit(fx, fy, FW, FH, c[i].x, c[i].y, c[i].w, c[i].h)) {
      fx = FIELD_X + FIELD_W / 2 - FW / 2;
      fy = FIELD_Y + FIELD_H - FH - 2;
      if (sc > 0) sc--;
    }
  }
  if (fy <= FIELD_Y + 2) {
    sc++;
    fx = FIELD_X + FIELD_W / 2 - FW / 2;
    fy = FIELD_Y + FIELD_H - FH - 2;
  }
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  for (int l = 0; l < LANES; l++) {
    int y = ly(l);
    display.drawLine(FIELD_X, y - 1, FIELD_X + FIELD_W, y - 1, SSD1306_WHITE);
  }
  for (int i = 0; i < LANES * 2; i++) display.fillRect(c[i].x, c[i].y, c[i].w, c[i].h, SSD1306_WHITE);
  display.fillRect(fx, fy, FW, FH, SSD1306_WHITE);
  drawScoreLabel("Fr:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.display();
}
