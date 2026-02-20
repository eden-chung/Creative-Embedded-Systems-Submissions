#include <Arduino.h>
#include <TFT_eSPI.h>
#include "words.h"

TFT_eSPI tft = TFT_eSPI();

static uint32_t rng = 0xC0FFEE12;
uint32_t ru32() { rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5; return rng; }
int rint(int lo, int hi) { return lo + (int)(ru32() % (uint32_t)(hi - lo)); }

uint32_t totalW = 0;

uint16_t pickIndex() {
  uint32_t r = ru32() % totalW;
  uint32_t acc = 0;
  for (uint16_t i = 0; i < WORD_COUNT; i++) {
    acc += (uint32_t)pgm_read_word(&WEIGHTS[i]);
    if (r < acc) return i;
  }
  return WORD_COUNT - 1;
}

void readWord(uint16_t idx, char* out, size_t cap) {
  const char* p = (const char*)pgm_read_ptr(&WORDS[idx]);
  strncpy_P(out, p, cap - 1);
  out[cap - 1] = '\0';
}

uint16_t randColor() {
  return tft.color565((uint8_t)rint(80,256), (uint8_t)rint(80,256), (uint8_t)rint(80,256));
}

struct Word {
  bool alive;
  char s[20];
  int x, y;
  int vx, vy;
  int w, h;
  int life;
  uint16_t col;
};

static const int N = 4;
Word words[N];

int freeSlot() {
  for (int i = 0; i < N; i++) if (!words[i].alive) return i;
  return rint(0, N);
}

void fade() {
  int flips = 900;
  int W = tft.width();
  int H = tft.height();
  for (int i = 0; i < flips; i++) {
    int x = rint(0, W);
    int y = rint(0, H);
    tft.drawPixel(x, y, TFT_BLACK);
  }
}

void spawnWord() {
  int i = freeSlot();
  Word &w = words[i];

  w.alive = true;
  readWord(pickIndex(), w.s, sizeof(w.s));
  w.s[18] = '\0';

  w.col = randColor();
  w.vx = rint(-4, 5); if (w.vx == 0) w.vx = 2;
  w.vy = rint(-3, 4); if (w.vy == 0) w.vy = 1;

  w.x = rint(0, max(1, tft.width() - 30));
  w.y = rint(0, max(1, tft.height() - 30));

  w.w = tft.textWidth(w.s);
  w.h = tft.fontHeight();

  w.life = rint(120, 220);
}

void stepWord(Word &w) {
  if (!w.alive) return;

  w.x += w.vx;
  w.y += w.vy;

  if (w.x < 0) { w.x = 0; w.vx = -w.vx; }
  else if (w.x + w.w >= tft.width()) { w.x = tft.width() - w.w - 1; w.vx = -w.vx; }

  if (w.y < 0) { w.y = 0; w.vy = -w.vy; }
  else if (w.y + w.h >= tft.height()) { w.y = tft.height() - w.h - 1; w.vy = -w.vy; }

  tft.setTextColor(w.col, TFT_BLACK);
  tft.drawString(w.s, w.x, w.y);

  w.life--;
  if (w.life <= 0) w.alive = false;
}

void setup() {
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);

  for (uint16_t i = 0; i < WORD_COUNT; i++) totalW += (uint32_t)pgm_read_word(&WEIGHTS[i]);
  for (int i = 0; i < N; i++) words[i].alive = false;

  spawnWord();
  spawnWord();
}

void loop() {
  static uint32_t lastSpawn = 0;
  static uint32_t lastFade = 0;

  uint32_t now = millis();

  if (now - lastSpawn >= 2000) {
    spawnWord();
    lastSpawn += 2000;
  }

  if (now - lastFade >= 120) {
    fade();
    lastFade += 120;
  }

  for (int i = 0; i < N; i++) stepWord(words[i]);

  delay(30);
}
