#include <Arduino.h>
#include <TFT_eSPI.h>
#include "words.h"

TFT_eSPI tft = TFT_eSPI();

uint32_t totalW = 0;

uint16_t pickIndex() {
  uint32_t r = random(totalW);
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
  return tft.color565((uint8_t)random(80, 256), (uint8_t)random(80, 256), (uint8_t)random(80, 256));
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
  for (int i = 0; i < N; i++) {
    if (!words[i].alive) return i;
  }
  return random(N);
}

void fade() {
  int W = tft.width();
  int H = tft.height();
  for (int i = 0; i < 900; i++) {
    tft.drawPixel(random(W), random(H), TFT_BLACK);
  }
}

void spawnWord() {
  Word &w = words[freeSlot()];

  w.alive = true;
  readWord(pickIndex(), w.s, sizeof(w.s));
  w.s[18] = '\0';

  w.col = randColor();

  w.vx = random(-4, 5);
  if (w.vx == 0) w.vx = 2;

  w.vy = random(-3, 4);
  if (w.vy == 0) w.vy = 1;

  w.x = random(max(1, tft.width() - 30));
  w.y = random(max(1, tft.height() - 30));

  w.w = tft.textWidth(w.s);
  w.h = tft.fontHeight();

  w.life = random(120, 220);
}

void stepWord(Word &w) {
  if (!w.alive) return;

  w.x += w.vx;
  w.y += w.vy;

  if (w.x < 0) {
    w.x = 0;
    w.vx = -w.vx;
  } else if (w.x + w.w >= tft.width()) {
    w.x = tft.width() - w.w - 1;
    w.vx = -w.vx;
  }

  if (w.y < 0) {
    w.y = 0;
    w.vy = -w.vy;
  } else if (w.y + w.h >= tft.height()) {
    w.y = tft.height() - w.h - 1;
    w.vy = -w.vy;
  }

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

  for (uint16_t i = 0; i < WORD_COUNT; i++) {
    totalW += (uint32_t)pgm_read_word(&WEIGHTS[i]);
  }

  for (int i = 0; i < N; i++) {
    words[i].alive = false;
  }

  spawnWord();
  spawnWord();
}

void loop() {
  static uint32_t lastSpawn = 0;
  static uint32_t lastFade = 0;
  uint32_t now = millis();

  if (now - lastSpawn >= 2000) {
    spawnWord();
    lastSpawn = now;
  }

  if (now - lastFade >= 120) {
    fade();
    lastFade = now;
  }

  for (int i = 0; i < N; i++) {
    stepWord(words[i]);
  }

  delay(30);
}