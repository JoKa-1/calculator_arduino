#pragma once
#include "screen_actions.h"

namespace Calculator {
    namespace Button {
      void catch_add_press(Screen::CalculatorState *state);
      void catch_sub_press(Screen::CalculatorState *state);
      void catch_entr_press(Screen::CalculatorState *state);
      void catch_ac_press(Screen::CalculatorState *state);
    }
}


