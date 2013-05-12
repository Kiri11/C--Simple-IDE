#include "CodeGenerator.h"

#include <stack>

std::string toString( int n )
{
   return std::to_string( static_cast< long double >( n ) );
}

CodeGenerator::CodeGenerator()
{
   mStrCount = 0;
   mLabelCount = 0;
   mStackReverseCount = 0;
   mAsmCode = "";
}

void CodeGenerator::GenLine( std::string line )
{
   mAsmCode += line + '\n';
}

void CodeGenerator::GenLine()
{
   mAsmCode += '\n';
}

void CodeGenerator::GenProgramStart()
{
   GenLine( ".386P" );
   GenLine( ".MODEL FLAT" );
   GenLine( "INCLUDELIB LIBCMT" );
   GenLine( "EXTRN _printf : NEAR" );
   GenLine( ".code\n" );
}

void CodeGenerator::GenProgramEnd()
{
   GenLine( "\nEND" );
}

std::string CodeGenerator::AsmCode()
{
   return mAsmCode;
}

int CodeGenerator::GenFuncStart( std::string funcName )
{
   GenBlockHeader( "Function " + funcName + " Declaration" );
   funcName = '_' + funcName;
   GenLine( "PUBLIC " + funcName );
   GenLine( funcName + " PROC" );
   GenPush( "ebp" );
   GenMov( "ebp","esp" );
   GenLine();
   return CreateLabel();
}

void CodeGenerator::GenBlockHeader( std::string blockName )
{
   GenLine( "\n;--------------- " + blockName + " ---------------" );
}

void CodeGenerator::GenFuncEnd( std::string funcName, int retLabel )
{
   funcName = '_' + funcName;
   GenLine();
   GenLabel( retLabel );
   GenMov( "esp", "ebp" );
   GenPop( "ebp" );
   GenLine( "RET" );
   GenLine( funcName + " ENDP" );
}

int CodeGenerator::CreateLabel()
{
   return ++mLabelCount;
}

void CodeGenerator::GenMov( std::string toStr, std::string fromStr )
{
   GenLine( "mov " + toStr + ", " + fromStr );
}

void CodeGenerator::GenPop( std::string reg )
{
   GenLine( "pop " + reg );
}

void CodeGenerator::GenLabel( int labelNum )
{
   GenLine( "L" + toString( labelNum ) + ":" ); 
}

void CodeGenerator::GenPush( std::string reg )
{
   GenLine( "push " + reg );
}

void CodeGenerator::GenPush( int n )
{
   GenLine( "push " + toString( n ) + "d" );
}

void CodeGenerator::GenStringPush( std::string str )
{
   std::string strVar = "S" + toString( mStrCount++ );
   GenLine( ".data" );
   GenLine( strVar + " BYTE \"" + str + "\", 00h" );
   GenLine( ".code" );
   GenPush( "OFFSET " + strVar );
}

void CodeGenerator::GenFuncCall( std::string funcName, int paramCountCall )
{
   GenReverseStack( paramCountCall );
   GenBlockHeader( "Call " + funcName + " Function" );
   GenLine( "CALL _" + funcName );
   GenLine( "add esp, " + toString( 4 * paramCountCall ) + "d" );
   GenPush( "eax" );
}

void CodeGenerator::GenReverseStack( int depth )
{
   if( depth < 2 ) return;
   GenBlockHeader( "Reversing last " + toString( depth ) + " stack items" );
   GenLine( ".data" );
   // allocate enough memory
   for( int i = 1; i <= depth; ++i )
   {
      GenLine( "stackReveseVar" + toString( mStackReverseCount++ ) + " DWORD ?" );
   }
   GenLine( ".code" );
   // pop into memory
   for( int i = depth; i >= 1; --i )
   {
      GenPop( "stackReveseVar" + toString( mStackReverseCount - i ) );
   }
   for( int i = depth; i >= 1; --i )
   {
      GenPush( "stackReveseVar" + toString( mStackReverseCount - i ) );
   }
}

// GenBinaryOperation
void CodeGenerator::GenBinaryOperation( std::string operationName )
{
   GenBlockHeader( operationName );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenLine( operationName + " eax, ecx" );
   GenPush( "eax" );
}

void CodeGenerator::GenMultiplication()
{
   GenBlockHeader( "Multiply" );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenLine( "imul ecx" );
   GenPush( "eax" );
}

void CodeGenerator::GenDivision()
{
   GenBlockHeader( "Divide" );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenLine( "CDQ" );
   GenLine( "idiv ecx" );
   GenPush( "eax" );
}

void CodeGenerator::GenCompare( std::string sign )
{
   GenBlockHeader( "Compare" );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenMov( "edx", "0" );
   GenLine( "cmp eax, ecx" );
   if( sign == "==" ) mAsmCode += "JNE ";
   if( sign == "!=" ) mAsmCode += "JE";
   if( sign == ">" ) mAsmCode += "JLE";
   if( sign == "<" ) mAsmCode += "JGE";
   if( sign == ">=" ) mAsmCode += "JL";
   if( sign == "<=" ) mAsmCode += "JG";
   mAsmCode += " L" + toString( ++mLabelCount ) + "\n";
   GenLine( "inc edx" );
   GenLabel( mLabelCount );
   GenPush( "edx" );
}

void CodeGenerator::GenAnd()
{
   GenBlockHeader( "And" );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenLine( "imul ecx" );
   GenMov( "edx", "0" );
   GenLine( "cmp eax, 0" );
   mAsmCode += "JE";
   mAsmCode += " L" + toString( ++mLabelCount ) + "\n";
   GenLine( "inc edx" );
   GenLabel( mLabelCount );
   GenPush( "edx" );
}

void CodeGenerator::GenOr()
{
   GenBlockHeader( "Or" );
   GenPop( "ecx" );
   GenPop( "eax" );
   GenMov( "edx", "0" );
   GenLine( "or eax, ecx" );
   GenLine( "cmp eax, 0" );
   GenLine( "JE L" + toString( ++mLabelCount ) );
   GenLine( "inc edx" );
   GenLabel( mLabelCount );
   GenPush( "edx" );
}

void CodeGenerator::GenNegate()
{
   GenBlockHeader( "Negate" );
   GenPop( "eax" );
   GenLine( "neg eax" );
   GenPush( "eax" );
}

void CodeGenerator::GenNot()
{
   GenBlockHeader( "Not" );
   GenPop( "eax" );
   GenMov( "edx", "0" );
   GenLine( "cmp eax, 0" );
   GenLine( "JNE L" + toString( ++mLabelCount ) );
   GenLine( "inc edx" );
   GenLabel( mLabelCount );
   GenPush( "edx" );
}

int CodeGenerator::GenIfStart()
{
   GenBlockHeader( "If statement" );
   GenPop( "eax" );
   GenLine( "cmp eax, 0" );
   int endIflabel = CreateLabel();
   GenLine( "JE L" + toString( endIflabel ) );
   return endIflabel;
}

int CodeGenerator::GenJmp()
{
   int newLabel = CreateLabel();
   GenLine( "JMP L" + toString( newLabel ) );
   return newLabel;
}

void CodeGenerator::GenJmp( int label )
{
   GenLine( "JMP L" + toString( label ) );
}

int CodeGenerator::GenWhileStart()
{
   GenBlockHeader( "While loop" );
   int newLabel = CreateLabel();
   GenLabel( newLabel );
   return newLabel;
}

int CodeGenerator::GenWhileMiddle()
{
   int whileFinishLabel = CreateLabel();
   GenPop( "eax" );
   GenLine( "cmp eax, 0" );
   GenLine( "JE L" + toString( whileFinishLabel ) );
   return whileFinishLabel;
}

int CodeGenerator::GenDoStart()
{
   GenBlockHeader( "Do loop" );
   int newLabel = CreateLabel();
   GenLabel( newLabel );
   return newLabel;
}

void CodeGenerator::GenDoMiddle( int doStartLabel )
{
   GenPop( "eax" );
   GenLine( "cmp eax, 0" );
   GenLine( "JNE L" + toString( doStartLabel ) );
}

void CodeGenerator::GenAssign( std::string addrTo )
{
   GenBlockHeader( "Assignment" );
   GenPop( "eax" );
   GenMov( addrTo, "eax" );
   GenPush( "eax" );
}

void CodeGenerator::GenLocalVarDef()
{
   GenLine( "sub esp, 4" );
}

void CodeGenerator::GenGlobalVarDef( std::string varName )
{
   GenLine( ".data" );
   GenLine( '_' + varName + " DWORD ?" );
   GenLine( ".code" );
}

void CodeGenerator::GenReturn( int loopEndLabel )
{
   GenBlockHeader( "Return" );
   GenPop( "eax" );
   GenJmp( loopEndLabel );
}