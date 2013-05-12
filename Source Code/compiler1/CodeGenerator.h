#pragma once
#include <string>

class CodeGenerator
{
public:
	            CodeGenerator();
	std::string AsmCode();

	       void GenProgramStart();
	       void GenProgramEnd();
	       void GenPrintf();
	        int GenFuncStart( std::string funcName );
	       void GenFuncEnd( std::string funcName, int retLabel );
	       void GenFuncCall( std::string funcName, int paramCountCall );
	       void GenPush( int n );
	       void GenPush( std::string reg );
	       void GenStringPush( std::string str );
	       void GenBinaryOperation( std::string operationName );
	       void GenMultiplication();
	       void GenDivision();
	       void GenCompare( std::string sign );
	       void GenAnd();
	       void GenOr();
	       void GenNegate();
	       void GenNot();
	       void GenLabel( int labelNum );
	        int GenIfStart();
	        int GenJmp();
	       void GenJmp( int label );
	       void GenPop( std::string reg );
	        int GenWhileStart();
	        int GenWhileMiddle();
	        int GenDoStart();
	       void GenDoMiddle( int doStartLabel );
	       void GenAssign( std::string addrTo );
	       void GenLocalVarDef();
	       void GenGlobalVarDef( std::string varName );
	        int CreateLabel();
	       void GenReturn( int loopEndLabel );

private:
   	
           void GenBlockHeader( std::string blockName );
	        void GenMov( std::string toStr, std::string fromStr );
	        void GenReverseStack( int itemsCount );
	        void GenLine( std::string line );
	        void GenLine();

	         int mStackReverseCount;
	         int mStrCount;
	         int mLabelCount;
	 std::string mAsmCode;
};