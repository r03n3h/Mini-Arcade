#include "DisplayConfig.h"
#include <string.h>
#define C 10
#define R 20
#define B 6
static byte f[R][C];
static int tx, ty, sc, iv = 500;
static byte sh[4][4];
static unsigned long lm;
static const byte SH[7][4][4] = { { { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 1, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 1, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 1, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, { { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } } };
static void copy() {
  memcpy(sh, SH[random(0, 7)], sizeof(sh));
}
static bool can(int dx, int dy) {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (sh[y][x]) {
        int nx = tx + x + dx, ny = ty + y + dy;
        if (nx < 0 || nx >= C || ny >= R || (ny >= 0 && f[ny][nx])) return false;
      }
  return true;
}
static void rot() {
  byte t[4][4];
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++) t[x][3 - y] = sh[y][x];
  byte bkp[4][4];
  memcpy(bkp, sh, sizeof(sh));
  memcpy(sh, t, sizeof(sh));
  if (!can(0, 0)) memcpy(sh, bkp, sizeof(sh));
}
static void clearL() {
  int lines = 0;
  for (int y = R - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < C; x++)
      if (!f[y][x]) {
        full = false;
        break;
      }
    if (full) {
      for (int r = y; r > 0; r--)
        for (int c = 0; c < C; c++) f[r][c] = f[r - 1][c];
      for (int c = 0; c < C; c++) f[0][c] = 0;
      lines++;
      y++;
    }
  }
  switch (lines) {
    case 1: sc += 10; break;
    case 2: sc += 30; break;
    case 3: sc += 50; break;
    case 4: sc += 80; break;
  }
  if (lines > 0 && iv > 150) iv -= 10 * lines;
}
static void drop() {
  if (can(0, 1)) ty++;
  else {
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        if (sh[y][x]) {
          int fx = tx + x, fy = ty + y;
          if (fy < 0) {
            inGame = false;
            return;
          }
          f[fy][fx] = 1;
        }
    clearL();
    copy();
    tx = 3;
    ty = 0;
  }
}
void startTetrisGame() {
  memset(f, 0, sizeof(f));
  sc = 0;
  iv = 500;
  copy();
  tx = 3;
  ty = 0;
  lm = millis();
}
void tetrisLoopTick() {
  if (btnPressed(BTN_LFT)) {
    if (can(-1, 0)) tx--;
    delay(120);
  }
  if (btnPressed(BTN_RHT)) {
    if (can(1, 0)) tx++;
    delay(120);
  }
  if (btnPressed(BTN_DWN)) {
    drop();
    delay(100);
  }
  if (btnPressed(BTN_MID)) {
    rot();
    delay(120);
  }
  if (btnPressed(BTN_RST)) {
    inGame = false;
    delay(150);
    return;
  }
  if (millis() - lm > (unsigned long)iv) {
    drop();
    lm = millis();
  }
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  for (int y = 0; y < R; y++)
    for (int x = 0; x < C; x++)
      if (f[y][x]) {
        int px = FIELD_X + x * B, py = FIELD_Y + y * B;
        display.fillRect(px, py, B, B, SSD1306_WHITE);
      }
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (sh[y][x]) {
        int px = FIELD_X + (tx + x) * B, py = FIELD_Y + (ty + y) * B;
        if (py >= FIELD_Y) display.fillRect(px, py, B, B, SSD1306_WHITE);
      }
  drawScoreLabel("T:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.display();
}
