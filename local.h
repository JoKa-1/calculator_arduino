#ifndef LOCAL_H
#define LOCAL_H

#include "src/screen_actions.h"
#include "src/button_actions.h"
#include "src/utils.h"
#include <Arduino.h>

/** LCD setup **/
// control Pins
const int register_select = 2, read_or_write = 3, enable = 4;
// data Pins
const int db4 = 5, db5 = 6, db6 = 7, db7 = 8;

/** general setup **/
// lcd dimensions
const int lcd_rows = 2;
const int lcd_cols = 16;

//pins
const int add_pin = 12;
const int sub_pin = 11;
const int entr_pin = 10;
const int ac_pin = 9;

// action intervalls
const int press_intervall = 20; // in milliseconds
const int printInterval = 250; // in milliseconds

// functions
void _setup();
void _loop();

#endif
