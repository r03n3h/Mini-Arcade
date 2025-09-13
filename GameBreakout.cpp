#include "DisplayConfig.h"
#include <math.h>

// Renamed to avoid ESP32 'BR' macro conflict
static const int BRK_COLS = 6;
static const int BRK_ROWS = 7;
static const int BRK_W    = FIELD_W / BRK_COLS;
static const int BRK_H    = 6;
static const int BRK_TOP  = FIELD_Y + 16;

static bool bricks[BRK_ROWS][BRK_COLS];

static const int PAD_W = 18;
static const int PAD_H = 3;
static int padX, padY;

static const int BALL_SIZE = 2;
static float bx, by, bdx, bdy, bspeed;

static bool overlap(int ax,int ay,int aw,int ah,int bx,int by,int bw,int bh){
  return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

static void resetBall() {
  bx = FIELD_X + FIELD_W / 2 - BALL_SIZE / 2;
  by = padY - BALL_SIZE - 1;
  bdx = (random(0, 2) ? 1 : -1);
  bdy = -1;
  bspeed = 1.2f;
}

void startBreakoutGame() {
  for (int r = 0; r < BRK_ROWS; r++)
    for (int c = 0; c < BRK_COLS; c++)
      bricks[r][c] = true;

  padX = FIELD_X + (FIELD_W - PAD_W) / 2;
  padY = FIELD_Y + FIELD_H - 8;
  resetBall();
}

void breakoutLoopTick() {
  if (btnPressed(BTN_LFT)) padX -= 2;
  if (btnPressed(BTN_RHT)) padX += 2;
  if (btnPressed(BTN_RST)) { inGame = false; delay(150); return; }

  if (padX < FIELD_X) padX = FIELD_X;
  if (padX + PAD_W > FIELD_X + FIELD_W) padX = FIELD_X + FIELD_W - PAD_W;

  bx += bdx * bspeed;
  by += bdy * bspeed;

  if (bx <= FIELD_X) { bx = FIELD_X; bdx = -bdx; }
  if (bx + BALL_SIZE >= FIELD_X + FIELD_W) { bx = FIELD_X + FIELD_W - BALL_SIZE; bdx = -bdx; }
  if (by <= FIELD_Y) { by = FIELD_Y; bdy = -bdy; }

  // Paddle bounce
  if (overlap((int)bx, (int)by, BALL_SIZE, BALL_SIZE, padX, padY, PAD_W, PAD_H)) {
    by = padY - BALL_SIZE - 0.01f;
    float pc = padX + PAD_W / 2.0f;
    float bc = bx + BALL_SIZE / 2.0f;
    float off = (bc - pc) / (PAD_W / 2.0f);
    bdx = off * (bspeed * 1.2f);
    bdy = -fabs(bdy);
    bspeed = min(bspeed + 0.05f, 2.8f);
    float m = sqrt(bdx * bdx + bdy * bdy);
    bdx = (bdx / m) * bspeed;
    bdy = (bdy / m) * bspeed;
  }

  // Brick collisions
  for (int r = 0; r < BRK_ROWS; r++) {
    for (int c = 0; c < BRK_COLS; c++) {
      if (!bricks[r][c]) continue;
      int rx = FIELD_X + c * BRK_W;
      int ry = BRK_TOP + r * BRK_H;
      if (overlap((int)bx, (int)by, BALL_SIZE, BALL_SIZE, rx, ry, BRK_W, BRK_H)) {
        bricks[r][c] = false;
        bdy = -bdy;  // simple vertical flip
      }
    }
  }

  // Miss -> restart
  if (by + BALL_SIZE >= FIELD_Y + FIELD_H) resetBall();

  // Draw
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);

  for (int r = 0; r < BRK_ROWS; r++) {
    for (int c = 0; c < BRK_COLS; c++) {
      if (bricks[r][c]) {
        display.fillRect(FIELD_X + c * BRK_W + 1, BRK_TOP + r * BRK_H + 1,
                         BRK_W - 2, BRK_H - 2, SSD1306_WHITE);
      }
    }
  }

  display.fillRect(padX, padY, PAD_W, PAD_H, SSD1306_WHITE);
  display.fillRect((int)bx, (int)by, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

  drawScoreLabel("B:", 0, FIELD_X + 2, FIELD_Y + 2);  // (optional: track bricks cleared)
  display.display();
}
