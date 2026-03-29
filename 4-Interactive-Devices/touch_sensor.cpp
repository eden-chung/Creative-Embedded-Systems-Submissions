#include <Arduino.h>

int threshold = 40;
int middleThreshold = 60;

bool touch1active = false;
bool touch2active = false;
bool touch3active = false;

unsigned long touch1pendingAt = 0;
unsigned long touch2pendingAt = 0;
unsigned long touch3pendingAt = 0;
bool touch1pending = false;
bool touch2pending = false;
bool touch3pending = false;

const unsigned long MIN_HOLD_MS = 50; // gets around noise issue when ESP is firing multiple times on a single press

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Touch Test Ready");
  Serial.print("Resting T8: "); Serial.println(touchRead(T8));
  Serial.print("Resting T3: "); Serial.println(touchRead(T3));
  Serial.print("Resting T4: "); Serial.println(touchRead(T4));
}

void loop() {
  unsigned long now = millis();

  // pad 1 (T8, middle button)
  bool p1down = touchRead(T8) < middleThreshold;

  if (p1down && !touch1active && !touch1pending) {
    touch1pending = true;
    touch1pendingAt = now;
  }
  if (!p1down && touch1pending && !touch1active) {
    // ghost touch dont do anything
    touch1pending = false;
  }
  if (touch1pending && !touch1active && (now - touch1pendingAt >= MIN_HOLD_MS)) {
    touch1pending = false;
    touch1active = true;
    Serial.println("TOUCH:1");
  }
  if (touch1active && !p1down) {
    touch1active = false;
    Serial.println("RELEASE:1");
  }

  // pad 2 (T3, left button)
  bool p2down = touchRead(T3) < threshold;

  if (p2down && !touch2active && !touch2pending) {
    touch2pending = true;
    touch2pendingAt = now;
  }
  if (!p2down && touch2pending && !touch2active) {
    touch2pending = false;
  }
  if (touch2pending && !touch2active && (now - touch2pendingAt >= MIN_HOLD_MS)) {
    touch2pending = false;
    touch2active = true;
    Serial.println("TOUCH:2");
  }
  if (touch2active && !p2down) {
    touch2active = false;
    Serial.println("RELEASE:2");
  }

  // pad 3 (T4, right button)
  bool p3down = touchRead(T4) < threshold;

  if (p3down && !touch3active && !touch3pending) {
    touch3pending = true;
    touch3pendingAt = now;
  }
  if (!p3down && touch3pending && !touch3active) {
    touch3pending = false;
  }
  if (touch3pending && !touch3active && (now - touch3pendingAt >= MIN_HOLD_MS)) {
    touch3pending = false;
    touch3active = true;
    Serial.println("TOUCH:3");
  }
  if (touch3active && !p3down) {
    touch3active = false;
    Serial.println("RELEASE:3");
  }

  delay(10);
}