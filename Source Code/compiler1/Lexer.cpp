#include "Lexer.h"


// не забыть, что у нас две такие функции
token Lexer::get_token(){
	FSM fsm;
	state fsm_state;
	int l = full_input.length();


	// go through states until it halts or blocks
	while (input_iterator < l) {
		if ((fsm_state = fsm.transition(full_input[input_iterator])) != progress)
		break;
		else 
			++input_iterator;
	}

	if (input_iterator == l){
		fsm_state = fsm.transition(EOF);
	}
	
	// building error string and throwing it
	if (fsm_state == block){
		string error_str;
		error_str = "\nLexical error when reading symbol: " + 
		error_str += full_input[input_iterator];
		throw error_str;
	}

	// get token
	token result_token;
	result_token.type = fsm.get_current_state();
	result_token.token_string = fsm.get_current_string();
	CheckKeyWord(result_token);
	// adding to string table. Хотя зачем вообще это нужно?
	if(result_token.type == IDENTIFIER){ 
		stringTable.insert(result_token.token_string);
	}

	return result_token;
}

token Lexer::see_token(){
	FSM fsm;
	state fsm_state;
	int l = full_input.length();
	int innerIterator = input_iterator;

	// go through states until it halts or blocks
	while (innerIterator < l) {
		if ((fsm_state = fsm.transition(full_input[innerIterator])) != progress)
		break;
		else 
			++innerIterator;
	}

	if (innerIterator == l){
		fsm_state = fsm.transition(EOF);
	}
	
	// building error string and throwing it
	if (fsm_state == block){
		string error_str;
		error_str = "\nLexical error when reading symbol: " + 
		error_str += full_input[innerIterator];
		throw error_str;
	}

	// get token
	token result_token;
	result_token.type = fsm.get_current_state();
	result_token.token_string = fsm.get_current_string();
	CheckKeyWord(result_token);
	// adding to string table. Хотя зачем вообще это нужно?
	if(result_token.type == IDENTIFIER){ 
		stringTable.insert(result_token.token_string);
	}

	return result_token;
}

Lexer::Lexer(std::string input_string){
	full_input = input_string;
	input_iterator = 0;
}

Lexer::Lexer(){
	full_input = "";
	input_iterator = 0;
}

void Lexer::CheckKeyWord(token &kw_token){
	if (kw_token.type != IDENTIFIER) return;
	if (kw_token.token_string == "int"){ 
		kw_token.type = KW_INT;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "if"){
		kw_token.type = KW_IF;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "else"){ 
		kw_token.type = KW_ELSE;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "while"){ 
		kw_token.type = KW_WHILE;
		kw_token.token_string = "";
		return; }
	if (kw_token.token_string == "do"){ 
		kw_token.type = KW_DO;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "continue"){ 
		kw_token.type = KW_CONTINUE;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "break"){ 
		kw_token.type = KW_BREAK;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "return"){
		kw_token.type = KW_RETURN;
		kw_token.token_string = "";
		return; 
	}
	if (kw_token.token_string == "auto"		||	kw_token.token_string == "for"		||
		kw_token.token_string == "typedef"	||	kw_token.token_string == "double"	||
		kw_token.token_string == "goto"		||	kw_token.token_string == "short"	||
		kw_token.token_string == "union"	||	kw_token.token_string == "case"		||
		kw_token.token_string == "sizeof"	||	kw_token.token_string == "unsigned"	||
		kw_token.token_string == "char"		||	kw_token.token_string == "enum"		||
		kw_token.token_string == "static"	||	kw_token.token_string == "extern"	||
		kw_token.token_string == "long"		||	kw_token.token_string == "struct"	||
		kw_token.token_string == "default"	||	kw_token.token_string == "float"	||
		kw_token.token_string == "register"	||	kw_token.token_string == "switch"	)
	{ 
		kw_token.type = KW_RESERVED; 
	}
	return;
}

void Lexer::setInputString(string inputString){
	full_input = inputString;
}