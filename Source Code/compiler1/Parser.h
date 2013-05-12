#pragma once
#include "Lexer.h"
#include "SymbolTable.h"
#include "GlobalTable.h"
#include "CodeGenerator.h"


class Parser
{
public:

	std::string mVariables;
	std::string mParsingTree;
	std::string mTokens;

	std::string Parse( std::string parseStr );
	std::string AsmCode();

private:

	std::string mAsmCode;
	std::string mParseStr;
	int mDepth;
	int mLexLevel;

	CodeGenerator mGenerator;
	Lexer mLexer;
	Token mCurrentToken;
	SymbolTable mSymbolTable;
	GlobalTable mGlobalTable;

	void ReadNextToken();
	void OutputDepth( std::string funcName );
	bool CheckIsFunction();
	void OutputVariable( bool isParameter );
	std::string GetIdAddress( std::string idName );
	int GetFuncParamsNumber( std::string funcName );

	void Program();
	void ExtDef();
	void Program2();
	void FuncDefNoType();
	void FuncOrDataDefNotypeNoident( std::string funcName );
	void Type();
	void FuncDefPart2( std::string funcName );
	void FuncDefMain( std::string funcName, int paramCount );
	int Params( int paramCount );
	int Params2( int paramCount );
	void Declarators2();
	void Declarators();
	void CompStatement( int returnLabel, int loopStartLabel, int loopEndLabel );
	void CompStatementInside( int returnLabel, int loopStartLabel, int loopEndLabel );
	void Declarations();
	void Declarations2();
	void Statements( int returnLabel, int loopStartLabel, int loopEndLabel );
	void Statements2( int returnLabel, int loopStartLabel, int loopEndLabel );
	void Statement( int returnLabel, int loopStartLabel, int loopEndLabel );
	void IfStatement( int returnLabel, int loopStartLabel, int loopEndLabel );
	void Else2( int ifEndlabel, int returnLabel, int loopStartLabel, int loopEndLabel );
	void WhileStatement( int returnLabel );
	void DoStatement( int returnLabel );
	void Expr2();
	int  Exprs2( std::string funcName );
	int  Exprs( int paramCount, std::string funcName );
	int  Exprs3( int paramCount, std::string funcName );
	void DataDef();
	void FuncCall( std::string funcName );
	void Identifier( std::string idName );

	void Expr();
	void L0();
	void L1();
	void T0();
	void L2();
	void T1();
	void L3();
	void T2();
	void L4();
	void T3();
	void L5();
	void T4();
	void L6();
	void T5();
	void L7();
	void L8( std::string idName );
	
};