#pragma once

namespace Calculator {
    namespace Screen {

	// structs
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
	
	// functions
	void init_sequence();
        void start_screen();  
	int update_numberfield(const number_field *field);
	int write_result(int val, CalculatorState *state);
	void reset(CalculatorState *state);
    }
}
