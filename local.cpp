#include "local.h"
#include <LiquidCrystal.h>

using namespace Calculator;

// init lcd singleton
LiquidCrystal lcd(register_select, enable, db4, db5, db6, db7);

// init state singleton
Screen::CalculatorState state;

// button timings
unsigned long lastAddPress = 0; // is updated when ADD is pressed
unsigned long lastSubPress = 0; // is updated when SUB is pressed
unsigned long lastAcPress = 0; // is updated when AC is pressed
unsigned long lastEntrPress = 0; // is updated when ENTR is pressed

// print timing
unsigned long startMillisPrint = 0; // is updated when print2serial is called

void _setup()
{  
  // set pins
  lcd.begin(lcd_cols, lcd_rows);
  pinMode(add_pin, INPUT);
  pinMode(sub_pin, INPUT);
  pinMode(entr_pin, INPUT);
  pinMode(ac_pin, INPUT);
  
  // init sequence
  Screen::init_sequence();
  lcd.clear();
  
  // base screen
  Screen::start_screen();
  
  Serial.begin(9600);
}


void _loop()
{
  int add_update = digitalRead(add_pin);
  int sub_update = digitalRead(sub_pin);
  int ac_update = digitalRead(ac_pin);
  int entr_update = digitalRead(entr_pin);
  
  if(add_update == HIGH && utils::allowAction(&lastAddPress))
  {
    Button::catch_add_press(&state);
  }
  
  if(sub_update == HIGH && utils::allowAction(&lastSubPress))
  {
    Button::catch_sub_press(&state);
  }
  
  if(entr_update == HIGH && utils::allowAction(&lastEntrPress))
  {
    Button::catch_entr_press(&state);
  }
  
  if(ac_update == HIGH && utils::allowAction(&lastAcPress))
  {
    Button::catch_ac_press(&state);
  }
  
  utils::print2serial(&state,  &startMillisPrint, printInterval);
}
