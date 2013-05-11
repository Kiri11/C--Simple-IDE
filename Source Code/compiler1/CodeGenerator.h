#pragma once
#include <string>
#include <stack>
using std::string;
using std::stack;

class CodeGenerator{
public:
	CodeGenerator();
	string getAsmCode();

	void genProgramStart();
	void genProgramEnd();
	void genPrintf();
	int  genFuncStart(string funcName);
	void genFuncEnd(string funcName, int retLabel);
	void genFuncCall(string funcName, int paramCountCall);
	void genPush(int n);
	void genPush(string reg);
	void genStringPush(string str);
	void genBinaryOperation(string operationName);
	void genMultiplication();
	void genDivision();
	void genCompare(string sign);
	void genAnd();
	void genOr();
	void genNegate();
	void genNot();
	void genLabel(int labelNum);
	int genIfStart();
	int genJmp();
	void genJmp(int label);
	void genPop(string reg);
	int genWhileStart();
	int genWhileMiddle();
	int genDoStart();
	void genDoMiddle(int doStartLabel);
	void genAssign(string addrTo);
	void genLocalVarDef();
	void genGlobalVarDef(string varName);
	int  createLabel();
	void genReturn(int loopEndLabel);

private:
	int stackReverseCount;
	int strCount;
	int labelCount;
	string asmCode;

	void genBlockHeader(string blockName);
	void genMov(string toStr, string fromStr);
	void genReverseStack(int itemsCount);
	void genLine(string line);
	void genLine();
};