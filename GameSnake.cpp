#include "DisplayConfig.h"
struct Point {
  int x, y;
};
static const int MAXN = 100;
static Point s[MAXN];
static int L, dx, dy;
static Point food;
static unsigned long lastMove;
static unsigned long step = 300;
static int sc = 0;
static void place() {
  bool ok;
  do {
    ok = true;
    food.x = random(GRID_WIDTH);
    food.y = random(GRID_HEIGHT);
    for (int i = 0; i < L; i++)
      if (s[i].x == food.x && s[i].y == food.y) {
        ok = false;
        break;
      }
  } while (!ok);
}
void startSnakeGame() {
  sc = 0;
  L = 5;
  dx = 0;
  dy = -1;
  for (int i = 0; i < L; i++) s[i] = { GRID_WIDTH / 2, GRID_HEIGHT / 2 + i };
  place();
  lastMove = millis();
}
void snakeLoopTick() {
  if (btnPressed(BTN_UP) && dy != 1) {
    dx = 0;
    dy = -1;
  }
  if (btnPressed(BTN_DWN) && dy != -1) {
    dx = 0;
    dy = 1;
  }
  if (btnPressed(BTN_LFT) && dx != 1) {
    dx = -1;
    dy = 0;
  }
  if (btnPressed(BTN_RHT) && dx != -1) {
    dx = 1;
    dy = 0;
  }
  if (btnPressed(BTN_RST)) {
    inGame = false;
    delay(150);
    return;
  }
  if (millis() - lastMove > step) {
    lastMove = millis();
    Point h = s[0];
    Point nh = { (h.x + dx + GRID_WIDTH) % GRID_WIDTH, (h.y + dy + GRID_HEIGHT) % GRID_HEIGHT };
    for (int i = 0; i < L; i++)
      if (s[i].x == nh.x && s[i].y == nh.y) {
        inGame = false;
        return;
      }
    for (int i = L; i > 0; i--) s[i] = s[i - 1];
    s[0] = nh;
    if (nh.x == food.x && nh.y == food.y) {
      L = min(L + 1, MAXN);
      sc += 10;
      place();
    }
  }
  display.clearDisplay();
  display.drawRect(FIELD_X - 1, FIELD_Y - 1, FIELD_W + 2, FIELD_H + 2, SSD1306_WHITE);
  for (int i = 0; i < L; i++) {
    int px = FIELD_X + s[i].x * SEG_SIZE;
    int py = FIELD_Y + s[i].y * SEG_SIZE;
    display.fillRect(px, py, SEG_SIZE, SEG_SIZE, SSD1306_WHITE);
  }
  int fx = FIELD_X + food.x * SEG_SIZE;
  int fy = FIELD_Y + food.y * SEG_SIZE;
  display.fillRect(fx, fy, SEG_SIZE, SEG_SIZE, SSD1306_WHITE);
  drawScoreLabel("S:", sc, FIELD_X + 2, FIELD_Y + 2);
  display.display();
}
