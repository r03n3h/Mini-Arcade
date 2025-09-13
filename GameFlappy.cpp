#include "DisplayConfig.h"

static const int SZ = 6;
static float fx, fy, fv;
static float GR = 0.08f, FL = -1.8f;

static const int PW = 8;
static const int NP = 1;
static int px[NP], gy[NP];
static bool scd[NP];
static int GAP = 32, SP = 34;
static int sc = 0;

static int rg() {
  int mn = FIELD_Y + 8, mx = FIELD_Y + FIELD_H - GAP - 8;
  if (mx <= mn) return FIELD_Y + (FIELD_H - GAP) / 2;
  return random(mn, mx + 1);
}

void startFlappyGame() {
  sc = 0;
  fx = FIELD_X + 12;
  fy = FIELD_Y + FIELD_H / 2;
  fv = 0;
  for (int i = 0; i < NP; i++) {
    px[i] = FIELD_X + FIELD_W + 10 + i * SP;
    gy[i] = rg();
    scd[i] = false;  // <-- lowercase
  }
}

void flappyLoopTick() {
  if (btnPressed(BTN_UP) || btnPressed(BTN_MID)) fv = FL;
  if (btnPressed(BTN_RST)) { inGame = false; delay(150); return; }

  fv += GR;
  fy += fv;

  if (fy <= FIELD_Y) { fy = FIELD_Y; inGame = false; }
  if (fy + SZ >= FIELD_Y + FIELD_H) { fy = FIELD_Y + FIELD_H - SZ; inGame = false; }
  if (!inGame) return;

  for (int i = 0; i < NP; i++) {
    px[i] -= 1;

    if (px[i] + PW < FIELD_X) {
      int mx = FIELD_X + FIELD_W;
      for (int k = 0; k < NP; k++) if (px[k] > mx) mx = px[k];
      px[i] = mx + SP;
      gy[i] = rg();
      scd[i] = false;  // <-- lowercase
    }

    int cx = (int)(fx + SZ / 2);
    if (!scd[i] && px[i] + PW < cx) { sc++; scd[i] = true; }

    bool ox = ((int)fx < px[i] + PW) && ((int)fx + SZ > px[i]);
    if (ox) {
      bool ok = ((int)fy >= gy[i]) && ((int)fy + SZ <= gy[i] + GAP);
      if (!ok) { inGame = false; return; }
    }
  }

  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  for (int i = 0; i < NP; i++) {
    display.fillRect(px[i], FIELD_Y, PW, gy[i] - FIELD_Y, SSD1306_WHITE);
    int by = gy[i] + GAP;
    display.fillRect(px[i], by, PW, FIELD_Y + FIELD_H - by, SSD1306_WHITE);
  }
  display.fillRect((int)fx, (int)fy, SZ, SZ, SSD1306_WHITE);
  drawScoreLabel("F:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.display();
}
