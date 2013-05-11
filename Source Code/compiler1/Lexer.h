#pragma once
#include <string>
#include "FSM.h"
#include "Token.h"
#include "HashTable.h"

class Lexer {
public:
	Lexer(string input_string);
	token get_token();
	token see_token(); 
	void setInputString(string inputString);
	Lexer();
private:
//	Lexer(); // don't allow to create empty lexer
	string full_input;
	int input_iterator;
	void CheckKeyWord(token &kw_token);
	HashTable stringTable;
};