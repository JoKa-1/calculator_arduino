#include "utils.h"

namespace utils {
  bool allowAction(unsigned long *lastPressTime, int interval = 250)
  {
    if (millis() - *lastPressTime < interval)
    {
       return false;
    }
    *lastPressTime = millis();
    return true;
  };
  
  void print2serial(Calculator::Screen::CalculatorState *state, unsigned long *lastPrintTime, const int printInterval){
    // prints only happen when given interval is exceeded
    if(!allowAction(lastPrintTime)) { return; }
    printDebug(state->first.value);
    printDebug(state->second.value);
    printDebug(state->result.value);
    Serial.println();
  }
}
