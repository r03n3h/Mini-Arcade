#include "DisplayConfig.h"
static int bx, by, dx, dy, py, sc;
static float sp;
void startPongGame() {
  sc = 0;
  bx = FIELD_X + FIELD_W / 2;
  by = FIELD_Y + FIELD_H / 2;
  dx = -1;
  dy = random(-1, 2);
  if (!dy) dy = 1;
  py = FIELD_Y + FIELD_H / 2 - 10;
  sp = 0.6;
}
void pongLoopTick() {
  if (btnPressed(BTN_UP)) py -= 2;
  if (btnPressed(BTN_DWN)) py += 2;
  if (btnPressed(BTN_RST)) {
    inGame = false;
    delay(150);
    return;
  }
  if (py < FIELD_Y) py = FIELD_Y;
  if (py + 20 > FIELD_Y + FIELD_H) py = FIELD_Y + FIELD_H - 20;
  bx += dx * sp;
  by += dy * sp;
  if (by <= FIELD_Y) {
    by = FIELD_Y;
    dy = -dy;
  }
  if (by >= FIELD_Y + FIELD_H - 2) {
    by = FIELD_Y + FIELD_H - 2;
    dy = -dy;
  }
  if (bx <= FIELD_X + 3 && by >= py && by <= py + 20) {
    bx = FIELD_X + 3;
    dx = -dx;
    int r = random(-1, 2);
    if (r) dy = r;
    sp += 0.2;
    if (sp > 3.5) sp = 3.5;
    sc++;
  }
  if (bx >= FIELD_X + FIELD_W - 2) {
    bx = FIELD_X + FIELD_W - 2;
    dx = -dx;
  }
  if (bx < FIELD_X) { startPongGame(); }
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  display.fillRect(bx, by, 2, 2, SSD1306_WHITE);
  display.fillRect(FIELD_X + 2, py, 2, 20, SSD1306_WHITE);
  drawScoreLabel("P:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.display();
}
