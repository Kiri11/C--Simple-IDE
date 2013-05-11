#include "CodeGenerator.h"

string toString(int n){
	return std::to_string((long double)n);
}

CodeGenerator::CodeGenerator(){
	strCount = 0;
	labelCount = 0;
	stackReverseCount = 0;
	asmCode = "";
}

void CodeGenerator::genLine(string line){
	asmCode += line + '\n';
}

void CodeGenerator::genLine(){
	asmCode += '\n';
}

void CodeGenerator::genProgramStart(){
	genLine(".386P");
	genLine(".MODEL FLAT");
	genLine("INCLUDELIB LIBCMT");
	genLine("EXTRN _printf : NEAR");
	genLine(".code\n");
}

void CodeGenerator::genProgramEnd(){
	genLine("\nEND");
}

string CodeGenerator::getAsmCode(){
	return asmCode;
}

int CodeGenerator::genFuncStart(string funcName){
	genBlockHeader("Function " + funcName + " Declaration");
	funcName = '_' + funcName;
	genLine("PUBLIC " + funcName);
	genLine(funcName + " PROC");
	genPush("ebp");
	genMov("ebp","esp");
	genLine();
	return createLabel();
}

void CodeGenerator::genBlockHeader(string blockName){
	genLine("\n;--------------- " + blockName + " ---------------");
}

void CodeGenerator::genFuncEnd(string funcName, int retLabel){
	funcName = '_' + funcName;
	genLine();
	genLabel(retLabel);
	genMov("esp", "ebp");
	genPop("ebp");
	genLine("RET");
	genLine(funcName + " ENDP");
}

int CodeGenerator::createLabel(){
	return ++labelCount;
}

void CodeGenerator::genMov(string toStr, string fromStr){
	genLine("mov " + toStr + ", " + fromStr);
}

void CodeGenerator::genPop(string reg){
	genLine("pop " + reg);
}

void CodeGenerator::genLabel(int labelNum){
	genLine("L" + toString(labelNum) + ":"); 
}

void CodeGenerator::genPush(string reg){
	genLine("push " + reg);
}

void CodeGenerator::genPush(int n){
	genLine("push " + toString(n) + "d");
}

void CodeGenerator::genStringPush(string str){
	string strVar = "S" + toString(strCount++);
	genLine(".data");
	genLine(strVar + " BYTE \"" + str + "\", 00h");
	genLine(".code");
	genPush("OFFSET " + strVar);
}

void CodeGenerator::genFuncCall(string funcName, int paramCountCall){
	genReverseStack(paramCountCall);
	genBlockHeader("Call " + funcName + " Function");
	genLine("CALL _" + funcName);
	genLine("add esp, " + toString(4 * paramCountCall) + "d");
	genPush("eax");
}

void CodeGenerator::genReverseStack(int depth){
	if (depth < 2) return;
	genBlockHeader("Reversing last " + toString(depth) + " stack items");
	genLine(".data");
	// allocate enough memory
	for (int i = 1; i <= depth; ++i){
		genLine("stackReveseVar" + toString(stackReverseCount++) + " DWORD ?");
	}
	genLine(".code");
	// pop into memory
	for (int i = depth; i >= 1; --i){
		genPop("stackReveseVar" + toString(stackReverseCount - i));
	}
	for (int i = depth; i >= 1; --i){
		genPush("stackReveseVar" + toString(stackReverseCount - i));
	}
}

// genBinaryOperation
void CodeGenerator::genBinaryOperation(string operationName){
	genBlockHeader(operationName);
	genPop("ecx");
	genPop("eax");
	genLine(operationName + " eax, ecx");
	genPush("eax");
}

void CodeGenerator::genMultiplication(){
	genBlockHeader("Multiply");
	genPop("ecx");
	genPop("eax");
	genLine("imul ecx");
	genPush("eax");
}

void CodeGenerator::genDivision(){
	genBlockHeader("Divide");
	genPop("ecx");
	genPop("eax");
	genLine("CDQ");
	genLine("idiv ecx");
	genPush("eax");
}

void CodeGenerator::genCompare(string sign){
	genBlockHeader("Compare");
	genPop("ecx");
	genPop("eax");
	genMov("edx", "0");
	genLine("cmp eax, ecx");
	if (sign == "==") asmCode += "JNE ";
	if (sign == "!=") asmCode += "JE";
	if (sign == ">") asmCode += "JLE";
	if (sign == "<") asmCode += "JGE";
	if (sign == ">=") asmCode += "JL";
	if (sign == "<=") asmCode += "JG";
	asmCode += " L" + toString(++labelCount) + "\n";
	genLine("inc edx");
	genLabel(labelCount);
	genPush("edx");
}

void CodeGenerator::genAnd(){
	genBlockHeader("And");
	genPop("ecx");
	genPop("eax");
	genLine("imul ecx");
	genMov("edx", "0");
	genLine("cmp eax, 0");
	asmCode += "JE";
	asmCode += " L" + toString(++labelCount) + "\n";
	genLine("inc edx");
	genLabel(labelCount);
	genPush("edx");
}

void CodeGenerator::genOr(){
	genBlockHeader("Or");
	genPop("ecx");
	genPop("eax");
	genMov("edx", "0");
	genLine("or eax, ecx");
	genLine("cmp eax, 0");
	genLine("JE L" + toString(++labelCount));
	genLine("inc edx");
	genLabel(labelCount);
	genPush("edx");
}

void CodeGenerator::genNegate(){
	genBlockHeader("Negate");
	genPop("eax");
	genLine("neg eax");
	genPush("eax");
}

void CodeGenerator::genNot(){
	genBlockHeader("Not");
	genPop("eax");
	genMov("edx", "0");
	genLine("cmp eax, 0");
	genLine("JNE L" + toString(++labelCount));
	genLine("inc edx");
	genLabel(labelCount);
	genPush("edx");
}

int CodeGenerator::genIfStart(){
	genBlockHeader("If statement");
	genPop("eax");
	genLine("cmp eax, 0");
	int endIflabel = createLabel();
	genLine("JE L" + toString(endIflabel));
	return endIflabel;
}

int CodeGenerator::genJmp(){
	int newLabel = createLabel();
	genLine("JMP L" + toString(newLabel));
	return newLabel;
}

void CodeGenerator::genJmp(int label){
	genLine("JMP L" + toString(label));
}

int CodeGenerator::genWhileStart(){
	genBlockHeader("While loop");
	int newLabel = createLabel();
	genLabel(newLabel);
	return newLabel;
}

int CodeGenerator::genWhileMiddle(){
	int whileFinishLabel = createLabel();
	genPop("eax");
	genLine("cmp eax, 0");
	genLine("JE L" + toString(whileFinishLabel));
	return whileFinishLabel;
}

int CodeGenerator::genDoStart(){
	genBlockHeader("Do loop");
	int newLabel = createLabel();
	genLabel(newLabel);
	return newLabel;
}

void CodeGenerator::genDoMiddle(int doStartLabel){
	genPop("eax");
	genLine("cmp eax, 0");
	genLine("JNE L" + toString(doStartLabel));
}

void CodeGenerator::genAssign(string addrTo){
	genBlockHeader("Assignment");
	genPop("eax");
	genMov(addrTo, "eax");
	genPush("eax");
}

void CodeGenerator::genLocalVarDef(){
	genLine("sub esp, 4");
}

void CodeGenerator::genGlobalVarDef(string varName){
	genLine(".data");
	genLine('_' + varName + " DWORD ?");\
	genLine(".code");
}

void CodeGenerator::genReturn(int loopEndLabel){
	genBlockHeader("Return");
	genPop("eax");
	genJmp(loopEndLabel);
}