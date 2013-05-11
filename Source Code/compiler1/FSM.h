#pragma once
#include <string>
#include "Token.h"
using std::string;

enum char_type{
	alpha, digit, space, smth
};

enum state {
	block, halt, progress
};

// Finite-state Machine
class FSM{
public:
	FSM();
	void reset();
	token_type get_current_state();
	// Transition returns 0 if block, 1 if halt, 2 if in progress
	state transition(char input);  
	string get_current_string();
private:
	token_type current_state;
	string current_string;
	char_type check_char_type(char ch);
};