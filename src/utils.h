#pragma once
#include "screen_actions.h"
#include <Arduino.h>

#define printDebug(var) \
    Serial.print(#var ": "); \
    Serial.println(var)

namespace utils {
  bool allowAction(unsigned long *lastPressTime, int interval = 250);
  void print2serial(Calculator::Screen::CalculatorState *state, unsigned long *lastPrintTime, const int printInterval);
}
