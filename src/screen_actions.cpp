#include "screen_actions.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

// defined in local.cpp
extern LiquidCrystal lcd;

namespace Calculator {
    namespace Screen {
      void init_sequence()
      {
        lcd.print("Welcome to Add3r");
        lcd.setCursor(5, 1);
        for (int i = 0; i < 16; i++)
        {
          lcd.setCursor(i, 1);
          lcd.print('=');
          delay(125);
        }
      }
      
      void start_screen()
      {
        lcd.setCursor(2, 0);
        lcd.print("00+00=000");
        
        lcd.setCursor(12, 0);
        lcd.print("ADD>");
        
        lcd.setCursor(0, 1);
        lcd.print("<ENTR");
        
        lcd.setCursor(8, 1);
        lcd.print("AC");
        
        lcd.setCursor(12, 1);
        lcd.print("SUB>");
      }
      
      // an additional 'mode'/'m' button could trigger a mode selection screen to 
      // let the user do different operations
      // this would mean the start_screen (base_screen ?) would need a operation param
      
      /**
       * this function assumes the col is initialy set on the smallest digit
       * */
      int update_numberfield(const number_field *field)
      {
        if (field->value < field->lowerLimit || field->value > field->upperLimit){
          return 1;
        };

        if (field->value >= 100){
          lcd.setCursor(field->col-2, field->row);
          lcd.print(field->value);
          return 0;
        }

        if (field->value >= 10){
          lcd.setCursor(field->col-1, field->row);
          lcd.print(field->value);
          return 0;
        }

        lcd.setCursor(field->col-1, field->row);
        lcd.print("0");
        lcd.setCursor(field->col, field->row);
        lcd.print(field->value);
        return 0;
      }
      
      int write_result(int val, CalculatorState *state)
      {
        if (val < state->result.lowerLimit || val > state->result.upperLimit)
        {
        	return 1;
        };
        lcd.setCursor(state->result.col, state->result.row);
        lcd.print(val);
        return 0;
      }
      
      void reset(CalculatorState *state)
      {
         lcd.clear();
         start_screen();
        
         state->first.active = true;
         state->first.value = 0;
        
         state->second.active = false;
         state->second.value = 0;
        
         state->result.active = false;
         state->result.value = 0;
        
         lcd.setCursor(state->first.col, state->first.row);
      }

  }
}
