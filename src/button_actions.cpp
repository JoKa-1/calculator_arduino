#include "button_actions.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

// defined in local.cpp
extern LiquidCrystal lcd;

namespace Calculator {
    namespace Button {
      void catch_add_press(Screen::CalculatorState *state)
      { 
         if(state->first.active)
         {
           //check limits
           state->first.value += 1;
           if(0 != Screen::update_numberfield(&(state->first)))
           {
             Serial.println("Input would exceed limits");
             state->first.value -= 1;
             return;
           }
         }
        
         if(state->second.active)
         {
           //check limits
           state->second.value += 1;
           if(0 != Screen::update_numberfield(&(state->second)))
           {
             Serial.println("Input would exceed limits");
             state->second.value -= 1;
             return;
           }
        }
      }
      
      void catch_sub_press(Screen::CalculatorState *state)
      { 
         if(state->first.active)
         {
           // check limits
           state->first.value -= 1;
           if(0 != Screen::update_numberfield(&(state->first)))
           {
             Serial.println("Input would exceed limits");
             state->first.value += 1;
             return;
           }
         }
      
         if(state->second.active)
         {
           // check limits
           state->second.value -= 1;
           if(0 != Screen::update_numberfield(&(state->second)))
           {
             Serial.println("Input would exceed limits");
             state->second.value += 1;
             return;
           }
         }
      }
      
      void catch_entr_press(Screen::CalculatorState *state)
      { 
         if(state->first.active)
         {
            state->first.active = false;
            state->second.active = true;
            lcd.setCursor(state->second.col, state->second.row);
            return;
         }
        
         if(state->second.active){
            int sum = state->first.value + state->second.value;
            // TODO: fix this inconsistency in how result-field is written compared to the others
            // because the result field isn't written incremetally it has to be simulated to written so
            // this also means that the limit check in the Screen::update_numberfield can't catch these so
            // it has to be done once before
            //if (sum < state->result.lowerLimit || sum > state->result.upperLimit)
            //{
            //    Serial.println("Result would exceed limits");
            //  return;
            //};
            //Screen::write_result(sum, state);
            state->result.value = sum;
            if(0 != Screen::update_numberfield(&(state->result)))
            {
              Serial.println("Input would exceed limits");
              state->result.value = 0;
              return;
            }
      
            state->second.active = false;
            state->result.active = true;
            return;
         } 
        
         if (state->result.active)
         {
           Screen::reset(state);
           return;
         }
      }
      
      void catch_ac_press(Screen::CalculatorState *state)
      { 
         Screen::reset(state);
      }
    }
}
