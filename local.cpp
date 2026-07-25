#include "local.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

#define printDebug(var) \
    Serial.print(#var ": "); \
    Serial.println(var)

/** LCD setup **/

// control Pins
const int register_select = 2, read_or_write = 3, enable = 4;

// data Pins
const int db4 = 5, db5 = 6, db6 = 7, db7 = 8;

// init lcd class
LiquidCrystal lcd(register_select, enable, db4, db5, db6, db7);

const int lcd_rows = 2;
const int lcd_cols = 16;

struct number_field {
    const int col;
    const int row;
    int value;
    int upperLimit;
    int lowerLimit;
    bool active;

    // Constructor
    number_field(int col, int row, int val, int upper, int lower, bool active)
        : col(col), row(row), value(val), upperLimit(upper), lowerLimit(lower), active(active) {}
};

// Instances with a struct to handle the whole state
struct CalculatorState {
    number_field first{3, 0, 0, 99, 0, true};
    number_field second{6, 0, 0, 99, 0, false};
    number_field result{8, 0, 0, 999, 0, false};
};

CalculatorState state;

//pins
const int add_pin = 12;
const int sub_pin = 11;
const int entr_pin = 10;
const int ac_pin = 9;

// button timings
const int press_intervall = 50; // in milliseconds
unsigned long lastAddPress = 0; // is updated when ADD is pressed
unsigned long lastSubPress = 0; // is updated when SUB is pressed
unsigned long lastAcPress = 0; // is updated when AC is pressed
unsigned long lastEntrPress = 0; // is updated when ENTR is pressed

// print setup
unsigned long startMillisPrint = 0; // is updated when print2serial is called
const int printInterval = 250; // in milliseconds

bool allowAction(unsigned long *lastPressTime, int interval = 250)
{
  if (millis() - *lastPressTime < interval)
  {
     return false;
  }
  *lastPressTime = millis();
  return true;
};

void print2serial(CalculatorState *state, unsigned long *lastPrintTime){
  // prints only happen when given interval is exceeded
  if(!allowAction(lastPrintTime)) { return; }
  printDebug(state->first.value);
  printDebug(state->second.value);
  printDebug(state->result.value);
  Serial.println();
}

void init_squence()
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
  lcd.print("00+00=0");
  
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

int update_numberfield(const number_field *field)
{
  if (field->value < field->lowerLimit || field->value > field->upperLimit)
  {
  	return 1;
  };
  if (field->value % 10 == 0)
  {
    int tenner = field->value / 10;
    lcd.setCursor(field->col-1, field->row);
    lcd.print(tenner);
    lcd.setCursor(field->col, field->row);
  	lcd.print(0);
    return 0;
  }
  lcd.setCursor(field->col, field->row);
  lcd.print(field->value%10);
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

void catch_add_press(unsigned long *lastAddPress, CalculatorState *state)
{ 
   if (!allowAction(lastAddPress)){ return; }
    
   if(state->first.active)
   {
     //check limits
     state->first.value += 1;
     if(0 != update_numberfield(&(state->first)))
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
     if(0 != update_numberfield(&(state->second)))
     {
       Serial.println("Input would exceed limits");
       state->second.value -= 1;
       return;
     }
  }
}

void catch_sub_press(unsigned long *lastSubPress, CalculatorState *state)
{ 
   if (!allowAction(lastSubPress)){ return; }
    
   if(state->first.active)
   {
     // check limits
     state->first.value -= 1;
     if(0 != update_numberfield(&(state->first)))
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
     if(0 != update_numberfield(&(state->second)))
     {
       Serial.println("Input would exceed limits");
       state->second.value += 1;
       return;
     }
   }
}

void catch_entr_press(unsigned long *lastEntrPress, CalculatorState *state)
{ 
   if (!allowAction(lastEntrPress)){ return; }
  
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
      // this also means that the limit check in the update_numberfield can't catch these so
      // it has to be done once before
      if (sum < state->result.lowerLimit || sum > state->result.upperLimit)
      {
  	Serial.println("Result would exceed limits");
        return;
      };
      write_result(sum, state);

      state->second.active = false;
      state->result.active = true;
      state->result.value = sum;
      return;
   } 
  
   if (state->result.active)
   {
     reset(state);
     return;
   }
}

void catch_ac_press(unsigned long *lastAcPress, CalculatorState *state)
{ 
   if (!allowAction(lastAcPress)){ return; }
   reset(state);
}

void _setup()
{  
  // set pins
  lcd.begin(lcd_cols, lcd_rows);
  pinMode(add_pin, INPUT);
  pinMode(sub_pin, INPUT);
  pinMode(entr_pin, INPUT);
  pinMode(ac_pin, INPUT);
  
  // init sequence
  init_squence();
  lcd.clear();
  
  // base screen
  start_screen();
  
  Serial.begin(9600);
}


void _loop()
{
  int add_update = digitalRead(add_pin);
  int sub_update = digitalRead(sub_pin);
  int ac_update = digitalRead(ac_pin);
  int entr_update = digitalRead(entr_pin);
  
  if(add_update == HIGH)
  {
    catch_add_press(&lastAddPress, &state);
  }
  
  if(sub_update == HIGH)
  {
    catch_sub_press(&lastSubPress, &state);
  }
  
  if(entr_update == HIGH)
  {
    catch_entr_press(&lastEntrPress, &state);
  }
  
  if(ac_update == HIGH)
  {
    catch_ac_press(&lastAcPress, &state);
  }
  
  print2serial(&state,  &startMillisPrint);
}
