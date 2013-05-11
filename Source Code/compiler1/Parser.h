#pragma once
#include "Lexer.h"
#include "SymbolTable.h"
#include "GlobalTable.h"
#include "CodeGenerator.h"


class Parser{
public:
	string variables;
	string parsingTree;
	string tokens;
	string parse(string parseStr);
	string getAsmCode();
private:
	string asmCode;
	string parseStr;
	int depth;
	int lexLevel;

	CodeGenerator generator;
	Lexer myLexer;
	token currentToken;
	SymbolTable symbolTable;
	GlobalTable globalTable;
	void readNextToken();
	void OutputDepth(string funcName);
	bool checkIsFunction();
	void outputVariable(bool isParameter);
	string getIdAddress(string idName);
	int getFuncParamsNumber(string funcName);

	void program();
	void ext_def();
	void program2();
	void func_def_notype();
	void func_or_data_def_notype_noident(string funcName);
	void type();
	void func_def_part2(string funcName);
	void func_def_main(string funcName, int paramCount);
	int params(int paramCount);
	int params2(int paramCount);
	void declarators2();
	void declarators();
	void comp_statement(int returnLabel, int loopStartLabel, int loopEndLabel);
	void comp_statement_inside(int returnLabel, int loopStartLabel, int loopEndLabel);
	void declarations();
	void declarations2();
	void statements(int returnLabel, int loopStartLabel, int loopEndLabel);
	void statements2(int returnLabel, int loopStartLabel, int loopEndLabel);
	void statement(int returnLabel, int loopStartLabel, int loopEndLabel);
	void if_statement(int returnLabel, int loopStartLabel, int loopEndLabel);
	void else2(int ifEndlabel, int returnLabel, int loopStartLabel, int loopEndLabel);
	void while_statement(int returnLabel);
	void do_statement(int returnLabel);
	void expr2();
	int  exprs2(string funcName);
	int  exprs(int paramCount, string funcName);
	int  exprs3(int paramCount, string funcName);
	void data_def();
	void func_call(string funcName);
	void identifier(string idName);

	void expr();
	void L0();
	void L1();
	void t0();
	void L2();
	void t1();
	void L3();
	void t2();
	void L4();
	void t3();
	void L5();
	void t4();
	void L6();
	void t5();
	void L7();
	void L8(string idName);
	
};