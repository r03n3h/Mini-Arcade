#include "DisplayConfig.h"

static const int COLS=GRID_WIDTH, ROWS=GRID_HEIGHT;
static byte lvl[ROWS][COLS]; static int pellets; static int sc=0;
static int px,py,dx,dy,wx,wy; static unsigned long lastP; static const unsigned long P_MS=120;
static int gx,gy; static unsigned long lastG; static const unsigned long G_MS=160;

static bool wall(int y,int x){ if(x<0||x>=COLS||y<0||y>=ROWS) return true; return lvl[y][x]==1; }

static void build(){ pellets=0;
  for(int y=0;y<ROWS;y++){ for(int x=0;x<COLS;x++){ bool border=(x==0||x==COLS-1||y==0||y==ROWS-1); byte v=border?1:0;
      if(!border){ if(y%4==0&&x>1&&x<COLS-2) v=1; if(x==COLS/2 && (y%6)>=2 && (y%6)<=3) v=1; }
      lvl[y][x]=v?1:2; if(lvl[y][x]==2) pellets++; } }
  for(int y=ROWS-4;y<ROWS-1;y++) for(int x=COLS/2-1;x<=COLS/2+1;x++){ if(lvl[y][x]==2){lvl[y][x]=0; pellets--;} if(lvl[y][x]==1) lvl[y][x]=0; }
  for(int y=2;y<=3;y++) for(int x=COLS/2-1;x<=COLS/2+1;x++){ if(lvl[y][x]==2){lvl[y][x]=0; pellets--;} if(lvl[y][x]==1) lvl[y][x]=0; }
}

static void respawn(){ px=COLS/2; py=ROWS-3; dx=0; dy=0; wx=0; wy=0; gx=COLS/2; gy=2; lastP=lastG=millis(); }

void startPacmanGame(){ sc=0; build(); respawn(); }

static void tryWish(){ int nx=px+wx, ny=py+wy; if(!wall(ny,nx)){ dx=wx; dy=wy; } }

static void stepP(){ tryWish(); int nx=px+dx, ny=py+dy; if(!wall(ny,nx)){ px=nx; py=ny; }
  if(lvl[py][px]==2){ lvl[py][px]=0; sc+=1; pellets--; if(pellets<=0){ sc+=10; build(); respawn(); } } }

static void stepG(){ int bestD=1e9, bdx=0,bdy=0; const int dirs[4][2]={{-1,0},{1,0},{0,-1},{0,1}}; for(int i=0;i<4;i++){ int ndx=dirs[i][0], ndy=dirs[i][1]; int nx=gx+ndx, ny=gy+ndy; if(wall(ny,nx)) continue; int d=abs(nx-px)+abs(ny-py); if(d<bestD){bestD=d; bdx=ndx; bdy=ndy;} } gx+=bdx; gy+=bdy; }

void pacmanLoopTick(){
  if(btnPressed(BTN_LFT)){wx=-1; wy=0;} if(btnPressed(BTN_RHT)){wx=1; wy=0;} if(btnPressed(BTN_UP)){wx=0; wy=-1;} if(btnPressed(BTN_DWN)){wx=0; wy=1;}
  if(btnPressed(BTN_RST)){inGame=false;delay(150);return;}
  unsigned long now=millis(); if(now-lastP>=P_MS){ stepP(); lastP=now; } if(now-lastG>=G_MS){ stepG(); lastG=now; }
  if(px==gx && py==gy){ inGame=false; return; }
  display.clearDisplay(); display.drawRect(FIELD_X-1,FIELD_Y-1,FIELD_W+2,FIELD_H+2,SSD1306_WHITE);
  for(int y=0;y<ROWS;y++){ for(int x=0;x<COLS;x++){ int sx=FIELD_X+x*SEG_SIZE, sy=FIELD_Y+y*SEG_SIZE; if(lvl[y][x]==1){ display.fillRect(sx,sy,SEG_SIZE,SEG_SIZE,SSD1306_WHITE);} else if(lvl[y][x]==2){ int d=2, o=(SEG_SIZE-d)/2; display.fillRect(sx+o,sy+o,d,d,SSD1306_WHITE);} } }
  display.fillRect(FIELD_X+px*SEG_SIZE, FIELD_Y+py*SEG_SIZE, SEG_SIZE, SEG_SIZE, SSD1306_WHITE);
  int gxpx=FIELD_X+gx*SEG_SIZE, gypy=FIELD_Y+gy*SEG_SIZE; display.fillRect(gxpx,gypy,SEG_SIZE,SEG_SIZE,SSD1306_WHITE); display.fillRect(gxpx+1,gypy+1,SEG_SIZE-2,SEG_SIZE-3,SSD1306_BLACK); display.fillRect(gxpx+1,gypy+3,SEG_SIZE-2,SEG_SIZE-3,SSD1306_WHITE);
  drawScoreLabel("Pa:", sc, FIELD_X+2, FIELD_Y+2); display.display();
}
