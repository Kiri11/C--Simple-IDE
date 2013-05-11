#include "Parser.h"
// Идея: param2() сделать обязательным, с возможностью пустой транзиции
// Идея: разобраться с разными вариантами декларации функций
// Идея: написать функцию, которая бы обобщила все функции парсера

string Parser::getAsmCode(){
	return asmCode;
}

void Parser::readNextToken(){
	currentToken = myLexer.get_token();
	tokens += currentToken.ToString() + " ";
}

void Parser::outputVariable(bool isParameter){
	variables += "Level " + std::to_string((long double)symbolTable.getLevel()) + ": " + currentToken.token_string;
	if (symbolTable.getLevel() > 0){
		if (isParameter) variables += " (parameter)";
		else variables += " (local)";
	}
	else {
		if (checkIsFunction()) variables += " (function)";
		else variables += " (global)";
	}
	variables += '\n';
}

void Parser::OutputDepth(string funcName){
	++depth;
	for (int i = 0; i < depth; ++i) { parsingTree += '.'; }
	parsingTree += funcName + '\n';
}

bool Parser::checkIsFunction(){
	if (myLexer.see_token().type == LEFT_PAREN){
		return true;
	}
	else {
		return false;
	}
}

string Parser::parse(string parseStr){
	depth = 0;
	lexLevel = 0;
	variables = "";
	parsingTree = "";
	tokens = "";

	myLexer.setInputString(parseStr);
	readNextToken();
	generator.genProgramStart();
	program();
	generator.genProgramEnd();
	asmCode = generator.getAsmCode();
	return "Source Code Accepted. ";
}

void Parser::program(){
	
	OutputDepth("program");

	if (currentToken.type == IDENTIFIER || currentToken.type == KW_INT){
		ext_def();
		program2();
		--depth; return;
		
	}
	throw (string)"Parsing error in program() function";
}

void Parser::program2(){
	
	OutputDepth("program2");

	if (currentToken.type == IDENTIFIER || currentToken.type == KW_INT){
		program();
		--depth; return;
	}
	if (currentToken.type == END){
		--depth; return; // empty transition
	}
	throw (string)"Parsing error in program2() function";
}

void Parser::ext_def(){
	
	OutputDepth("ext_def");

	// function
	if (currentToken.type == IDENTIFIER){
		outputVariable(false);
		func_def_notype();
		--depth; return;
	}
	if (currentToken.type == KW_INT){
		type();
		if (currentToken.type == IDENTIFIER){
			outputVariable(false);
			string idName = currentToken.token_string;
			readNextToken();
			func_or_data_def_notype_noident(idName);
			--depth; return;
		}
	}
	throw (string)"Parsing error in ext_def() function";
}

void Parser::func_def_notype(){
	
	OutputDepth("func_def_notype");

	if (currentToken.type == IDENTIFIER){
		string funcName = currentToken.token_string;
		readNextToken();
		if (currentToken.type == LEFT_PAREN){
			readNextToken();
			func_def_part2(funcName);
			--depth; return;
		}
	}
	throw (string)"Parsing error in func_def_notype() function";
}

void Parser::func_def_part2(string funcName){
	
	OutputDepth("func_def_part2");
	
	
	int paramCount;
	symbolTable.createLevel();
	if (currentToken.type == RIGHT_PAREN){
		paramCount = 0;
		func_def_main(funcName, paramCount);
		--depth; return;
	}
	if (currentToken.type == KW_INT){
		paramCount = params(0);
		func_def_main(funcName, paramCount);
		--depth; return;
	}
	
	throw (string)"Parsing error in func_def_part2() function";
}

void Parser::func_def_main(string funcName, int paramCount){
	OutputDepth("func_def_main");

	if (currentToken.type == RIGHT_PAREN){
		globalTable.insert(funcName, paramCount);
		int returnLabel = generator.genFuncStart(funcName);
		readNextToken();
		comp_statement(returnLabel, returnLabel, returnLabel);
		symbolTable.deleteLevel();
		generator.genFuncEnd(funcName, returnLabel);
		--depth; return;
	}
	throw (string)"Parsing error in func_def_main() function";
}


int Parser::params(int paramCount){
	
	OutputDepth("params");

	if (currentToken.type == KW_INT){
		type();
		if (currentToken.type == IDENTIFIER){
			symbolTable.insert(currentToken.token_string, true);
			outputVariable(true);
			readNextToken();
			paramCount = params2(paramCount + 1);
			--depth; return paramCount;
		}
	}
	throw (string)"Parsing error in params() function";
}

int Parser::params2(int paramCount){
	
	OutputDepth("params2");

	if (currentToken.type == COMMA){
		readNextToken();
		paramCount = params(paramCount);
		--depth; return paramCount;
	}
	if (currentToken.type == RIGHT_PAREN){
		--depth; return paramCount; // Empty transition
	}
	throw (string)"Parsing error in params2() function";
}

void Parser::type(){
	
	OutputDepth("type");

	if (currentToken.type == KW_INT){
		readNextToken();
		--depth; return; 
	}
	throw (string)"Parsing error in type() function";
}

void Parser::func_or_data_def_notype_noident(string idName){
	
	OutputDepth("func_or_data_def_notype_noident");
	
	//declarators
	if (currentToken.type == COMMA || currentToken.type == SEMICOLON){
		globalTable.insert(idName, -1);
		generator.genGlobalVarDef(idName);
		declarators2();
		if (currentToken.type == SEMICOLON){
			readNextToken();
			--depth; return;
		}
	}
	// function
	if (currentToken.type == LEFT_PAREN){
		readNextToken();
		func_def_part2(idName);
		--depth; return;
	}
	throw (string)"Parsing error in func_or_data_def_notype_noident() function";
}

void Parser::declarators2(){
	
	OutputDepth("declarators2");

	if (currentToken.type == COMMA){
		readNextToken();
		declarators();
		--depth; return; 
	}
	if (currentToken.type == SEMICOLON){
		--depth; return; // empty transition
	}
	throw (string)"Parsing error in declarators2() function";
}

void Parser::declarators(){
	
	OutputDepth("declarators");

	if (currentToken.type == IDENTIFIER){
		if (symbolTable.getLevel() > 0) {
			symbolTable.insert(currentToken.token_string, false);
			generator.genLocalVarDef();
		}
		else {
			generator.genGlobalVarDef(currentToken.token_string);
			globalTable.insert(currentToken.token_string, -1);
		}
		// getlevel and output
		outputVariable(false);

		readNextToken();
		declarators2();
		--depth; return;
	}
	throw (string)"Parsing error in declarators() function";
}

void Parser::comp_statement(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("comp_statement");
	

	if (currentToken.type == LEFT_BRACE){
		readNextToken();
		comp_statement_inside(returnLabel, loopStartLabel, loopEndLabel);
		if (currentToken.type == RIGHT_BRACE){
			readNextToken();
			
			--depth; return;
		}
	}
	throw (string)"Parsing error in comp_statement() function";
}

void Parser::comp_statement_inside(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("comp_statement_inside");

	if (currentToken.type == KW_INT){
		declarations();
		statements2(returnLabel, loopStartLabel, loopEndLabel);
		--depth; return;
	}
	if (	currentToken.type == LEFT_BRACE  || currentToken.type == IDENTIFIER		  ||
			currentToken.type == NUMBER		 || currentToken.type == LEFT_PAREN		  || 
			currentToken.type == MINUS		 || currentToken.type == NOT			  || 
			currentToken.type == KW_IF		 || currentToken.type == KW_WHILE		  ||
			currentToken.type == KW_DO		 || currentToken.type == KW_CONTINUE	  ||
			currentToken.type == KW_BREAK	 || currentToken.type == KW_RETURN		  ||
			currentToken.type == SEMICOLON	 || currentToken.type == RIGHT_BRACE	  ){
		statements2(returnLabel, loopStartLabel, loopEndLabel);
		--depth; return;
	}
	
	throw (string)"Parsing error in comp_statement_inside() function";
}

void Parser::declarations(){
	
	OutputDepth("declarations");

	if (currentToken.type == KW_INT){
//		readNextToken();
		data_def();
		declarations2();
		--depth; return;
	}
	throw (string)"Parsing error in declarations() function";
}

void Parser::data_def(){
	
	OutputDepth("data_def");

	if (currentToken.type == KW_INT){
		type();
		declarators();
		if (currentToken.type == SEMICOLON){
			readNextToken();
			--depth; return;
		}
	}
	throw (string)"Parsing error in data_def() function";
}

void Parser::declarations2(){
	
	OutputDepth("declarations2");

	if (currentToken.type == KW_INT){
		declarations();
		--depth; return;
	}
	if (	currentToken.type == LEFT_BRACE  || currentToken.type == IDENTIFIER		  ||
			currentToken.type == NUMBER		 || currentToken.type == LEFT_PAREN		  || 
			currentToken.type == MINUS		 || currentToken.type == NOT			  || 
			currentToken.type == KW_IF		 || currentToken.type == KW_WHILE		  ||
			currentToken.type == KW_DO		 || currentToken.type == KW_CONTINUE	  ||
			currentToken.type == KW_BREAK	 || currentToken.type == KW_RETURN		  ||
			currentToken.type == SEMICOLON	 || currentToken.type == RIGHT_BRACE	  ){
		--depth; return; // Empty transition
	}
	throw (string)"Parsing error in declarations2() function";
}

void Parser::statements(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("statements");

	if (	currentToken.type == LEFT_BRACE  || currentToken.type == IDENTIFIER		  ||
			currentToken.type == NUMBER		 || currentToken.type == LEFT_PAREN		  || 
			currentToken.type == MINUS		 || currentToken.type == NOT			  || 
			currentToken.type == KW_IF		 || currentToken.type == KW_WHILE		  ||
			currentToken.type == KW_DO		 || currentToken.type == KW_CONTINUE	  ||
			currentToken.type == KW_BREAK	 || currentToken.type == KW_RETURN		  ||
			currentToken.type == SEMICOLON	 										  ){
		statement(returnLabel, loopStartLabel, loopEndLabel);
		statements2(returnLabel, loopStartLabel, loopEndLabel);
		--depth; return;
	}
	throw (string)"Parsing error in statements() function";
}

void Parser::statements2(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("statements2");

	if (	currentToken.type == LEFT_BRACE  || currentToken.type == IDENTIFIER		  ||
			currentToken.type == NUMBER		 || currentToken.type == LEFT_PAREN		  || 
			currentToken.type == MINUS		 || currentToken.type == NOT			  || 
			currentToken.type == KW_IF		 || currentToken.type == KW_WHILE		  ||
			currentToken.type == KW_DO		 || currentToken.type == KW_CONTINUE	  ||
			currentToken.type == KW_BREAK	 || currentToken.type == KW_RETURN		  ||
			currentToken.type == SEMICOLON	 										  ){
		statements(returnLabel, loopStartLabel, loopEndLabel);
		--depth; return;
	}
	if (currentToken.type == RIGHT_BRACE){
		--depth; return;
	}
	throw (string)"Parsing error in statements2() function";
}

void Parser::statement(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("statement");

	switch (currentToken.type)
	{
	case LEFT_BRACE:
		symbolTable.createLevel();
		comp_statement(returnLabel, loopStartLabel, loopEndLabel);
		symbolTable.deleteLevel();
		break;
	case IDENTIFIER: case NUMBER: case LEFT_PAREN: case MINUS: case NOT:
		expr();
		if (currentToken.type == SEMICOLON){
			generator.genPop("eax");
			readNextToken();
		}
		else throw (string)"Parsing error in statement() function (SEMICOLON not found after expression)";
		break;
	case KW_IF:
		if_statement(returnLabel, loopStartLabel, loopEndLabel);
		break;
	case KW_WHILE:
		while_statement(returnLabel);
		break;
	case KW_DO:
		do_statement(returnLabel);
		break;
	case KW_CONTINUE:
		readNextToken();
		generator.genJmp(loopStartLabel);
		if (currentToken.type == SEMICOLON) readNextToken();
		else throw (string)"Parsing error in statement() function (SEMICOLON not found after continue)";
		break;
	case KW_BREAK:
		readNextToken();
		generator.genJmp(loopEndLabel);
		if (currentToken.type == SEMICOLON) readNextToken();
		else throw (string)"Parsing error in statement() function (SEMICOLON not found after break)";
		break;
	case KW_RETURN:
		readNextToken();
		expr2();
		generator.genReturn(returnLabel);
		if (currentToken.type == SEMICOLON){
			readNextToken();
		}
		else throw (string)"Parsing error in statement() function (SEMICOLON not found after return statement)";
		break;
	case SEMICOLON:
		readNextToken();
		break;
	default:
		throw (string)"Parsing error in statement() function";
		break;
	} 
}

void Parser::if_statement(int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("if_statement");

	if (currentToken.type == KW_IF){
		readNextToken();
		if (currentToken.type == LEFT_PAREN){
			readNextToken();
			expr();
			int ifEndlabel = generator.genIfStart();
			if (currentToken.type == RIGHT_PAREN){
				readNextToken();
				statement(returnLabel, loopStartLabel, loopEndLabel);
				else2(ifEndlabel, returnLabel, loopStartLabel, loopEndLabel);
				--depth; return;
			}
		}
	}
	throw (string)"Parsing error in if_statement() function";
}

void Parser::else2(int ifEndlabel, int returnLabel, int loopStartLabel, int loopEndLabel){
	
	OutputDepth("else2");

	if (currentToken.type == KW_ELSE){
		int elseEndLabel = generator.genJmp();
		generator.genLabel(ifEndlabel);
		readNextToken();
		statement(returnLabel, loopStartLabel, loopEndLabel);
		generator.genLabel(elseEndLabel);
		--depth; return;
	}
	if (	currentToken.type == LEFT_BRACE  || currentToken.type == IDENTIFIER		  ||
			currentToken.type == NUMBER		 || currentToken.type == LEFT_PAREN		  || 
			currentToken.type == MINUS		 || currentToken.type == NOT			  || 
			currentToken.type == KW_IF		 || currentToken.type == KW_WHILE		  ||
			currentToken.type == KW_DO		 || currentToken.type == KW_CONTINUE	  ||
			currentToken.type == KW_BREAK	 || currentToken.type == KW_RETURN		  ||
			currentToken.type == SEMICOLON	 || currentToken.type == RIGHT_BRACE	  ){
				generator.genLabel(ifEndlabel);
		--depth; return; // Empty transition
	}
	throw (string)"Parsing error in else2() function";
}

void Parser::while_statement(int returnLabel){
	
	OutputDepth("while_statement");

	if (currentToken.type == KW_WHILE){
		readNextToken();
		if (currentToken.type == LEFT_PAREN){
			readNextToken();
			int whileStartLabel = generator.genWhileStart();
			expr();
			int whileFinishLabel = generator.genWhileMiddle();
			if (currentToken.type == RIGHT_PAREN){
				readNextToken();
				statement(returnLabel, whileStartLabel, whileFinishLabel);
				generator.genJmp(whileStartLabel);
				generator.genLabel(whileFinishLabel);
				--depth; return;
			}
		}
	}
	throw (string)"Parsing error in while_statement() function";
}

void Parser::do_statement(int returnLabel){
	
	OutputDepth("do_statement");

	if (currentToken.type == KW_DO){
		readNextToken();
		int doStartLabel = generator.genDoStart();
		int doEvaluationLabel = generator.createLabel();
		int doFinishLabel = generator.createLabel();
		statement(returnLabel, doEvaluationLabel, doFinishLabel);
		if (currentToken.type == KW_WHILE){
			readNextToken();
			if (currentToken.type == LEFT_PAREN){
				readNextToken();
				generator.genLabel(doEvaluationLabel);
				expr();
				generator.genDoMiddle(doStartLabel);
				if (currentToken.type == RIGHT_PAREN){
					readNextToken();
					if (currentToken.type == SEMICOLON){
						readNextToken();
						generator.genLabel(doFinishLabel);
						--depth; return;
					}
				}
			}
		}
	}
	throw (string)"Parsing error in do_statement() function";
}

void Parser::expr2(){
	
	OutputDepth("expr2");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			expr();
			--depth; return;
	}
	if (currentToken.type == SEMICOLON){
		--depth; return; // do nothing
	}
	throw (string)"Parsing error in expr2() function";
}

int Parser::exprs2(string funcName){
	
	OutputDepth("exprs2");
	int paramCount = 0;
	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN || currentToken.type == COMPLETED_STRING){
			paramCount = exprs(paramCount, funcName);
			--depth; return paramCount;
	}
	if (currentToken.type == RIGHT_PAREN){
		--depth; return paramCount; // do nothing
	}
	throw (string)"Parsing error in exprs2() function";
}

int Parser::exprs(int paramCount, string funcName){
	
	OutputDepth("exprs");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			expr();
			paramCount = exprs3(++paramCount, funcName);
			--depth; return paramCount;
	}
	// string
	if (currentToken.type == COMPLETED_STRING){
		if (funcName != "printf"){
			string error = "Type mismatch in function call " + funcName;
			throw error;
		}
		generator.genStringPush(currentToken.token_string);
		readNextToken();
		paramCount = exprs3(++paramCount, funcName);
		--depth; return paramCount; 
	}
	throw (string)"Parsing error in exprs() function";
}

int Parser::exprs3(int paramCount, string funcName){
	
	OutputDepth("exprs3");

	if (currentToken.type == COMMA){
			readNextToken();
			paramCount = exprs(paramCount, funcName);
			--depth; return paramCount;
	}
	if (currentToken.type == RIGHT_PAREN){
		// do nothing
		--depth; return paramCount; 
	}
	throw (string)"Parsing error in exprs3() function";
}

void Parser::expr(){
	
	OutputDepth("expr");

	if (currentToken.type == IDENTIFIER){
		// хак LL(2), подсматриваем следующий токен
		if (myLexer.see_token().type == ASSIGN){
			string addrStr = getIdAddress(currentToken.token_string);
			readNextToken();
			readNextToken();
			expr();
			generator.genAssign(addrStr);
			--depth; return; // вторую ветку уже не надо проверять
		}
	}
	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L0();
			--depth; return;
	}
	throw (string)"Parsing error in expr() function";
}

void Parser::L0(){
	
	OutputDepth("L0");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L1();
			t0();
	}
	else throw (string)"Parsing error in L0() function";
	--depth; return;
}

void Parser::t0(){
	
	OutputDepth("t0");

	if (currentToken.type == OR){
		readNextToken();
		L0();
		generator.genOr();
	} else 
		if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON ||
			currentToken.type == COMMA || currentToken.type == COMPLETED_STRING){
				   --depth; return; // do nothing
	}else throw (string)"Parsing error in t0() function";
}

void Parser::L1(){
	
	OutputDepth("L1");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L2();
			t1();
	}
	else throw (string)"Parsing error in L1() function";
	--depth; return;
}

void Parser::t1(){
	
	OutputDepth("t1");

	if (currentToken.type == AND){
		readNextToken();
		L2();
		generator.genAnd();
		t1();
	} else 
		if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON ||
			currentToken.type == COMMA || currentToken.type == COMPLETED_STRING || 
			currentToken.type == OR){
				   --depth; return; // do nothing
	}else throw (string)"Parsing error in t1() function";
}

void Parser::L2(){
	
	OutputDepth("L2");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L3();
			t2();
	}
	else throw (string)"Parsing error in L2() function";
	--depth; return;
}

void Parser::t2(){
	
	OutputDepth("t2");

	if (currentToken.type == EQUALS){
		readNextToken();
		L3();
		generator.genCompare("==");
		t2();
		--depth; return;
	} 
	if (currentToken.type == NOT_EQUALS){
		readNextToken();
		L3();
		generator.genCompare("!=");
		t2();
		--depth; return;
	}
	if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON ||
			currentToken.type == COMMA || currentToken.type == COMPLETED_STRING || 
			currentToken.type == OR || currentToken.type == AND){
		--depth; return; // do nothing
	}
	throw (string)"Parsing error in t2() function";
}

void Parser::L3(){
	
	OutputDepth("L3");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L4();
			t3();
	}
	else throw (string)"Parsing error in L3() function";
	--depth; return;
}

void Parser::t3(){
	
	OutputDepth("t3");

	if (currentToken.type == LESS){
		readNextToken();
		L4();
		generator.genCompare("<");
		t3();
		--depth; return;
	} 
	if (currentToken.type == MORE){
		readNextToken();
		L4();
		generator.genCompare(">");
		t3();
		--depth; return;
	}
	if (currentToken.type == LESS_EQUALS){
		readNextToken();
		L4();
		generator.genCompare("<=");
		t3();
		--depth; return;
	}
	if (currentToken.type == MORE_EQUALS){
		readNextToken();
		L4();
		generator.genCompare(">=");
		t3();
		--depth; return;
	}
	if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON ||
			currentToken.type == COMMA || currentToken.type == COMPLETED_STRING || 
			currentToken.type == OR || currentToken.type == AND || 
			currentToken.type == EQUALS || currentToken.type == NOT_EQUALS){
		--depth; return; // do nothing
	}
	throw (string)"Parsing error in t3() function";
}

void Parser::L4(){
	
	OutputDepth("L4");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L5();
			t4();
	}
	else throw (string)"Parsing error in L4() function";
	--depth; return;
}

void Parser::t4(){
	
	OutputDepth("t4");

	if (currentToken.type == PLUS){
		readNextToken();
		L5();
		generator.genBinaryOperation("add");
		t4();
		--depth; return;
	} 
	if (currentToken.type == MINUS){
		readNextToken();
		L5();
		generator.genBinaryOperation("sub");
		t4();
		--depth; return;
	}
	if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON		  ||
		currentToken.type == COMMA		 || currentToken.type == COMPLETED_STRING || 
		currentToken.type == OR			 || currentToken.type == AND			  || 
		currentToken.type == EQUALS		 || currentToken.type == NOT_EQUALS		  ||
		currentToken.type == LESS		 || currentToken.type == MORE			  ||
		currentToken.type == LESS_EQUALS || currentToken.type == MORE_EQUALS	  ){
			--depth; return; // do nothing
	}
	throw (string)"Parsing error in t4() function";
}

void Parser::L5(){
	
	OutputDepth("L5");

	if (currentToken.type == IDENTIFIER || currentToken.type == MINUS || currentToken.type == NOT ||
		currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
			L6();
			t5();
	}
	else throw (string)"Parsing error in L5() function";
	--depth; return;
}

void Parser::t5(){
	
	OutputDepth("t5");

	if (currentToken.type == MULT){
		readNextToken();
		L6();
		generator.genMultiplication();
		t5();
		--depth; return;
	} 
	if (currentToken.type == DIVISION){
		readNextToken();
		L6();
		generator.genDivision();
		t5();
		--depth; return;
	}
	if (	currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON		  ||
			currentToken.type == COMMA		 || currentToken.type == COMPLETED_STRING || 
			currentToken.type == OR			 || currentToken.type == AND			  || 
			currentToken.type == EQUALS		 || currentToken.type == NOT_EQUALS		  ||
			currentToken.type == LESS		 || currentToken.type == MORE			  ||
			currentToken.type == LESS_EQUALS || currentToken.type == MORE_EQUALS	  ||
			currentToken.type == MINUS		 || currentToken.type == PLUS			  ){
		--depth; return; // do nothing
	}
	throw (string)"Parsing error in t5() function";
}

void Parser::L6(){
	
	OutputDepth("L6");

	if (currentToken.type == MINUS){
		readNextToken();
		L6();
		generator.genNegate();
		--depth; return;
	} 
	if (currentToken.type == NOT){
		readNextToken();
		L6();
		generator.genNot();
		--depth; return;
	}
	if (currentToken.type == IDENTIFIER || currentToken.type == NUMBER || currentToken.type == LEFT_PAREN){
		L7();
		--depth; return;
	}
	throw (string)"Parsing error in L6() function";
}

void Parser::L7(){
	
	OutputDepth("L7");

	// parens
	if (currentToken.type == LEFT_PAREN){
		readNextToken();
		expr();
		if (currentToken.type == RIGHT_PAREN){
			readNextToken();
		}
		else throw (string)"Parsing error in L7() function (RIGHT_PAREN not found)";
		--depth; return;
	} 
	// number
	if (currentToken.type == NUMBER){
		generator.genPush(currentToken.token_string + 'd');
		readNextToken();
		--depth; return;
	}
	// ident, call L8
	if (currentToken.type == IDENTIFIER){
		string idName = currentToken.token_string;
		readNextToken();
		L8(idName);
		--depth; return;
	}
	throw (string)"Parsing error in L7() function";
}

void Parser::L8(string idName){
	
	OutputDepth("L8");

	// function call
	if (currentToken.type == LEFT_PAREN){
		func_call(idName);
		--depth; return;
	} 
	// identifier
	if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON		  ||
		currentToken.type == COMMA		 || currentToken.type == COMPLETED_STRING || 
		currentToken.type == OR			 || currentToken.type == AND			  || 
		currentToken.type == EQUALS		 || currentToken.type == NOT_EQUALS		  ||
		currentToken.type == LESS		 || currentToken.type == MORE			  ||
		currentToken.type == LESS_EQUALS || currentToken.type == MORE_EQUALS	  ||
		currentToken.type == MINUS		 || currentToken.type == PLUS			  ||
		currentToken.type == MULT		 || currentToken.type == DIVISION		  ){
			identifier(idName);
			--depth; return; // do nothing
	}

	throw (string)"Parsing error in L8() function";
}

void Parser::func_call(string funcName){
	OutputDepth("func_call");
	
	if (currentToken.type == LEFT_PAREN){
		// int address = symbolTable.getIdentifierAddress(idName);
		// достать из Global Table функцию. Тогда мы будем знать заранее, сколько ждать параметров. Исходя из этого и проверять.
		int paramCountDeclaration = getFuncParamsNumber(funcName);
		readNextToken();
		int paramCountCall = exprs2(funcName);
		if (paramCountDeclaration >= 0 && paramCountDeclaration == paramCountCall || funcName == "printf"){
			generator.genFuncCall(funcName, paramCountCall);
		}
		else{
			string error = "Function " + funcName + " requies different set of parameters.\n";
			error += "Declared number: " + std::to_string((long double)paramCountDeclaration) + '\n';
			error += "Called number: " + std::to_string((long double)paramCountCall) + '\n';
			throw error;
		}
		if (currentToken.type == RIGHT_PAREN){
			readNextToken();
		}
		else throw (string)"Parsing error in func_call() function (RIGHT_PAREN not found)";
		--depth; return;
	}
	throw (string)"Parsing error in func_call() function";
}

void Parser::identifier(string idName){
	OutputDepth("identifier");

	if (currentToken.type == RIGHT_PAREN || currentToken.type == SEMICOLON		  ||
		currentToken.type == COMMA		 || currentToken.type == COMPLETED_STRING || 
		currentToken.type == OR			 || currentToken.type == AND			  || 
		currentToken.type == EQUALS		 || currentToken.type == NOT_EQUALS		  ||
		currentToken.type == LESS		 || currentToken.type == MORE			  ||
		currentToken.type == LESS_EQUALS || currentToken.type == MORE_EQUALS	  ||
		currentToken.type == MINUS		 || currentToken.type == PLUS			  ||
		currentToken.type == MULT		 || currentToken.type == DIVISION		  ){
			
			string address = getIdAddress(idName);
			generator.genPush(address);
			--depth; return;
	}
	throw (string)"Parsing error in identifier() function";
}

string Parser::getIdAddress(string idName){
	int address = symbolTable.getIdentifierAddress(idName);
	string addrStr = "";
	// if not found
	if (address == -1){
		int paramCount = globalTable.check(idName);
		// if function
		if (paramCount >= 0){
			throw (string) "Cannot use function " + idName + " as variable.";
		}
		// if not found
		if (paramCount == -2){
			throw (string) idName + ": undeclared identifier.";
		}
		addrStr += '_' + idName;
	}
	// if found
	else{
		addrStr += "[ebp";
		if (address > 0) addrStr += '+';
		addrStr +=	std::to_string((long double)address) + "]";
	}
	return addrStr;
}

int Parser::getFuncParamsNumber(string funcName){
	int address = symbolTable.getIdentifierAddress(funcName);
	// if not found
	if (address == -1){
		int paramCount = globalTable.check(funcName);
		// if function
		if (paramCount >= 0){
			return paramCount;
		}
		// if global identifier
		if (paramCount == -1){
			throw (string) funcName + ": not a function.";
		}
		// if not found
		if (paramCount == -2){
			// printf as an external function
			if (funcName == "printf") return 1;
			throw (string) funcName + ": undeclared function.";
		}
		throw (string) funcName + ": error while getting parameters";
	}
	// if found
	else{
		throw (string) funcName + ": not a function.";
	}
}