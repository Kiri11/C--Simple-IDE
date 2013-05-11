#include "FSM.h"

FSM::FSM(){
	reset();
}

token_type FSM::get_current_state(){
	return current_state;
}

void FSM::reset(){
	current_state = INITIAL;
	current_string = "";
}

char_type FSM::check_char_type(char ch){
	if (isdigit(ch)) return digit;
	if (isalpha(ch)) return alpha;
	if (isspace(ch) || ch == EOF) return space;
	return smth;
}

string FSM::get_current_string(){
	return current_string;
}

state FSM::transition(char input){

	char_type input_type = check_char_type(input);
	switch (current_state)
	{
	case INITIAL:
		if (input == EOF) {
			current_state = END;
			return halt;
		}
		if (input_type == space) {
			return progress;
		}
		if (input_type == digit) {
			current_state = NUMBER;
			current_string += input;
			return progress;
		}
		if (input_type == alpha || input == '_') {
			current_state = IDENTIFIER;
			current_string += input;
			return progress;
		}
		if (input == '"') {
			current_state = STRING_IN_PROC;
			return progress;
		}
		if (input == '&') {
			current_state = AMPERSAND;
			//current_string += input;
			return progress;
		}
		if (input == '|') {
			current_state = BAR;
			//current_string += input;
			return progress;
		}
		if (input == ';') {
			current_state = SEMICOLON;
			//current_string += input;
			return progress;
		}
		if (input == '*') {
			current_state = MULT;
			//current_string += input;
			return progress;
		}
		if (input == '+') {
			current_state = PLUS;
			//current_string += input;
			return progress;
		}
		if (input == '-') {
			current_state = MINUS;
			//current_string += input;
			return progress;
		}
		if (input == '(') {
			current_state = LEFT_PAREN;
			//current_string += input;
			return progress;
		}
		if (input == ')') {
			current_state = RIGHT_PAREN;
			//current_string += input;
			return progress;
		}
		if (input == '{') {
			current_state = LEFT_BRACE;
			//current_string += input;
			return progress;
		}
		if (input == '}') {
			current_state = RIGHT_BRACE;
			//current_string += input;
			return progress;
		}
		if (input == ',') {
			current_state = COMMA;
			//current_string += input;
			return progress;
		}
		if (input == '=') {
			current_state = ASSIGN;
			//current_string += input;
			return progress;
		}
		if (input == '!') {
			current_state = NOT;
			//current_string += input;
			return progress;
		}
		if (input == '<') {
			current_state = LESS;
			//current_string += input;
			return progress;
		}
		if (input == '>') {
			current_state = MORE;
			//current_string += input;
			return progress;
		}
		if (input == '/') {
			current_state = DIVISION;
			//current_string += input;
			return progress;
		}
		return block;
	case STRING_IN_PROC:
		if (input == '\\') {
			current_state = STRING_ESCAPE;
			return progress;
		}
		if (input == '"') {
			current_state = COMPLETED_STRING;
			return progress;
		}
		if (input == '\n'){
			return block;
		}
		current_string += input;
		return progress;
	case STRING_ESCAPE:
		if (input == 't'){
			current_state = STRING_IN_PROC;
			current_string += '\t';
			return progress;
		}
		// передвигаем каретку назад, чтобы избежать пустых строк. По идее не должно влиять
		if (input == 'n'){
			current_state = STRING_IN_PROC;
			current_string += "\b\", 0Ah, \"\b";
			return progress;
		}
		if (input == '\\'){
			current_state = STRING_IN_PROC;
			current_string += '\\';
			return progress;
		}
		if (input == '"'){
			current_state = STRING_IN_PROC;
			current_string += '"';
			return progress;
		}
		if (input == 'b'){
			current_state = STRING_IN_PROC;
			current_string += '\b';
			return progress;
		}
		return block;
	case COMM_LINE:
		if (input == '\n'){
			current_state = INITIAL;
		}
		return progress;
	case COMM_IN_PROC:
		if (input == EOF){
			return block;
		}
		if (input == '*'){
			current_state = COMM_ABOUT_TO_FINISH;
		}
		return progress;
	case COMM_ABOUT_TO_FINISH:
		if (input == EOF){
			return block;
		}
		if (input == '/'){
			current_state = INITIAL;
			return progress;
		}
		current_state = COMM_IN_PROC;
		return progress;
	case AMPERSAND:
		if (input == '&'){
			//current_string += input;
			current_state = AND;
			return progress;
		}
		return block;
	case BAR:
		if (input == '|'){
			//current_string += input;
			current_state = OR;
			return progress;
		}
		return block;
	case IDENTIFIER:
		if (input_type == alpha || input_type == digit || input == '_'){
			current_string += input;
			return progress;
		}
		return halt;
	case NUMBER:
		if (input_type == digit){
			current_string += input;
			return progress;
		}
		if (input_type == alpha || input == '_'){
			return block;
		}
		return halt;
	case SEMICOLON:
		return halt;
	case MULT:
		return halt;
	case PLUS:
		return halt;
	case MINUS:
		return halt;
	case LEFT_PAREN:
		return halt;
	case RIGHT_PAREN:
		return halt;
	case LEFT_BRACE:
		return halt;
	case RIGHT_BRACE:
		return halt;
	case COMMA:
		return halt;;
	case ASSIGN:
		if (input == '='){
			current_state = EQUALS;
			//current_string += input;
			return progress;
		}
		return halt;
	case NOT:
		if (input == '='){
			current_state = NOT_EQUALS;
			//current_string += input;
			return progress;
		}
		if (input_type == space || input == '/'){
			return block;
		}
		return halt;
	case LESS:
		if (input == '='){
			current_state = LESS_EQUALS;
			//current_string += input;
			return progress;
		}
		return halt;
	case MORE:
		if (input == '='){
			current_state = MORE_EQUALS;
			//current_string += input;
			return progress;
		}
		return halt;
	case EQUALS:
		return halt;
	case NOT_EQUALS:
		return halt;
	case LESS_EQUALS:
		return halt;
	case MORE_EQUALS:
		return halt;
	case AND:
		return halt;
	case OR:
		return halt;
	case COMPLETED_STRING:
		return halt;
	case DIVISION:
		if (input == '/'){
			current_state = COMM_LINE;
			current_string = "";
			return progress;
		}
		if (input == '*'){
			current_state = COMM_IN_PROC;
			current_string = "";
			return progress;
		}
		return halt;
		break;
	case END:
		return halt;
	default:
		break;
	}
	return block;
}