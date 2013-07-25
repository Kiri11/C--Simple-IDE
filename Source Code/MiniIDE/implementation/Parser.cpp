#include "Parser.h"
// Идея: param2() сделать обязательным, с возможностью пустой транзиции
// Идея: разобраться с разными вариантами декларации функций
// Идея: написать функцию, которая бы обобщила все функции парсера

std::string Parser::AsmCode()
{
   return mAsmCode;
}

void Parser::ReadNextToken()
{
   mCurrentToken = mLexer.GetToken();
   mTokens += mCurrentToken.ToString() + " ";
}

void Parser::OutputVariable( bool isParameter )
{
   mVariables += "Level " + std::to_string( static_cast< long double >( mSymbolTable.GetLevel() ) ) + ": " + mCurrentToken.token_string;
   if( mSymbolTable.GetLevel() > 0 )
   {
      if( isParameter ) mVariables += " (parameter)";
      else mVariables += " (local)";
   }
   else
   {
      if( CheckIsFunction() ) mVariables += " (function)";
      else mVariables += " (global)";
   }
   mVariables += '\n';
}

void Parser::OutputDepth( std::string funcName )
{
   ++mDepth;
   for( int i = 0; i < mDepth; ++i ) 
   {
      mParsingTree += '.';
   }
   mParsingTree += funcName + '\n';
}

bool Parser::CheckIsFunction()
{
   if( mLexer.SeeToken().type == LEFT_PAREN )
   {
      return true;
   }
   else
   {
      return false;
   }
}

std::string Parser::Parse( std::string parseStr )
{
   mDepth = 0;
   mLexLevel = 0;
   mVariables = "";
   mParsingTree = "";
   mTokens = "";

   mLexer.SetInputString( parseStr );
   ReadNextToken();
   mGenerator.GenProgramStart();
   Program();
   mGenerator.GenProgramEnd();
   mAsmCode = mGenerator.AsmCode();
   return "Source Code Accepted. ";
}

void Parser::Program()
{	
   OutputDepth( "program" );

   if( mCurrentToken.type == IDENTIFIER || mCurrentToken.type == KW_INT )
   {
      ExtDef();
      Program2();
      --mDepth; return;
      
   }
   throw ( std::string )"Parsing error in Program() function";
}

void Parser::Program2()
{
   OutputDepth( "program2" );

   if( mCurrentToken.type == IDENTIFIER || mCurrentToken.type == KW_INT )
   {
      Program();
      --mDepth; return;
   }
   if( mCurrentToken.type == END )
   {
      --mDepth; return; // empty transition
   }
   throw ( std::string )"Parsing error in Program2() function";
}

void Parser::ExtDef()
{
   OutputDepth( "Ext_def" );

   // function
   if( mCurrentToken.type == IDENTIFIER )
   {
      OutputVariable( false );
      FuncDefNoType();
      --mDepth; return;
   }
   if( mCurrentToken.type == KW_INT )
   {
      Type();
      if( mCurrentToken.type == IDENTIFIER ){
         OutputVariable( false );
         std::string idName = mCurrentToken.token_string;
         ReadNextToken();
         FuncOrDataDefNotypeNoident( idName );
         --mDepth; return;
      }
   }
   throw ( std::string )"Parsing error in Ext_def() function";
}

void Parser::FuncDefNoType()
{
   OutputDepth( "func_def_notype" );

   if( mCurrentToken.type == IDENTIFIER )
   {
      std::string funcName = mCurrentToken.token_string;
      ReadNextToken();
      if( mCurrentToken.type == LEFT_PAREN )
      {
         ReadNextToken();
         FuncDefPart2( funcName );
         --mDepth; return;
      }
   }
   throw ( std::string )"Parsing error in func_def_notype() function";
}

void Parser::FuncDefPart2( std::string funcName )
{	
   OutputDepth( "func_def_part2" );
      
   int paramCount;
   mSymbolTable.CreateLevel();
   if( mCurrentToken.type == RIGHT_PAREN )
   {
      paramCount = 0;
      FuncDefMain( funcName, paramCount );
      --mDepth; return;
   }
   if( mCurrentToken.type == KW_INT )
   {
      paramCount = Params( 0 );
      FuncDefMain( funcName, paramCount );
      --mDepth; return;
   }
   
   throw ( std::string )"Parsing error in func_def_parT2() function";
}

void Parser::FuncDefMain( std::string funcName, int paramCount )
{
   OutputDepth( "func_def_main" );

   if( mCurrentToken.type == RIGHT_PAREN )
   {
      mGlobalTable.Insert( funcName, paramCount );
      int returnLabel = mGenerator.GenFuncStart( funcName );
      ReadNextToken();
      CompStatement( returnLabel, returnLabel, returnLabel );
      mSymbolTable.DeleteLevel();
      mGenerator.GenFuncEnd( funcName, returnLabel );
      --mDepth; return;
   }
   throw ( std::string )"Parsing error in func_def_main() function";
}


int Parser::Params( int paramCount )
{
   OutputDepth( "params" );

   if( mCurrentToken.type == KW_INT )
   {
      Type();
      if( mCurrentToken.type == IDENTIFIER )
      {
         mSymbolTable.Insert( mCurrentToken.token_string, true );
         OutputVariable( true );
         ReadNextToken();
         paramCount = Params2( paramCount + 1 );
         --mDepth; return paramCount;
      }
   }
   throw ( std::string )"Parsing error in params() function";
}

int Parser::Params2( int paramCount )
{
   OutputDepth( "params2" );

   if( mCurrentToken.type == COMMA )
   {
      ReadNextToken();
      paramCount = Params( paramCount );
      --mDepth; return paramCount;
   }
   if( mCurrentToken.type == RIGHT_PAREN )
   {
      --mDepth; return paramCount; // Empty transition
   }
   throw ( std::string )"Parsing error in params2() function";
}

void Parser::Type()
{
   OutputDepth( "type" );

   if( mCurrentToken.type == KW_INT )
   {
      ReadNextToken();
      --mDepth; return; 
   }
   throw ( std::string )"Parsing error in type() function";
}

void Parser::FuncOrDataDefNotypeNoident( std::string idName )
{
   OutputDepth( "func_or_data_def_notype_noident" );
   
   //declarators
   if( mCurrentToken.type == COMMA || mCurrentToken.type == SEMICOLON )
   {
      mGlobalTable.Insert( idName, -1 );
      mGenerator.GenGlobalVarDef( idName );
      Declarators2();
      if( mCurrentToken.type == SEMICOLON )
      {
         ReadNextToken();
         --mDepth; return;
      }
   }
   // function
   if( mCurrentToken.type == LEFT_PAREN )
   {
      ReadNextToken();
      FuncDefPart2( idName );
      --mDepth; return;
   }
   throw ( std::string )"Parsing error in func_or_data_def_notype_noident() function";
}

void Parser::Declarators2()
{
   OutputDepth( "Declarators2" );

   if( mCurrentToken.type == COMMA )
   {
      ReadNextToken();
      Declarators();
      --mDepth;
      return; 
   }
   if( mCurrentToken.type == SEMICOLON )
   {
      --mDepth;
      return; // empty transition
   }
   throw ( std::string )"Parsing error in declarators2() function";
}

void Parser::Declarators()
{
   OutputDepth( "declarators" );

   if( mCurrentToken.type == IDENTIFIER )
   {
      if( mSymbolTable.GetLevel() > 0 )
      {
         mSymbolTable.Insert( mCurrentToken.token_string, false );
         mGenerator.GenLocalVarDef();
      }
      else
      {
         mGenerator.GenGlobalVarDef( mCurrentToken.token_string );
         mGlobalTable.Insert( mCurrentToken.token_string, -1 );
      }
      // getlevel and output
      OutputVariable( false );

      ReadNextToken();
      Declarators2();
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in declarators() function";
}

void Parser::CompStatement( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "comp_statement" );
   
   if( mCurrentToken.type == LEFT_BRACE )
   {
      ReadNextToken();
      CompStatementInside( returnLabel, loopStartLabel, loopEndLabel );
      if( mCurrentToken.type == RIGHT_BRACE )
      {
         ReadNextToken();
         
         --mDepth;
         return;
      }
   }
   throw ( std::string )"Parsing error in comp_Statement() function";
}

void Parser::CompStatementInside( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "comp_statement_inside" );

   if( mCurrentToken.type == KW_INT )
   {
      Declarations();
      Statements2( returnLabel, loopStartLabel, loopEndLabel );
      --mDepth; return;
   }
   if(	mCurrentToken.type == LEFT_BRACE  || mCurrentToken.type == IDENTIFIER		  ||
         mCurrentToken.type == NUMBER		 || mCurrentToken.type == LEFT_PAREN		  || 
         mCurrentToken.type == MINUS		 || mCurrentToken.type == NOT			        || 
         mCurrentToken.type == KW_IF		 || mCurrentToken.type == KW_WHILE		     ||
         mCurrentToken.type == KW_DO		 || mCurrentToken.type == KW_CONTINUE	     ||
         mCurrentToken.type == KW_BREAK	 || mCurrentToken.type == KW_RETURN		     ||
         mCurrentToken.type == SEMICOLON	 || mCurrentToken.type == RIGHT_BRACE	  )
   {
      Statements2( returnLabel, loopStartLabel, loopEndLabel );
      --mDepth;
      return;
   }
   
   throw ( std::string )"Parsing error in comp_statement_inside() function";
}

void Parser::Declarations()
{
   OutputDepth( "declarations" );

   if( mCurrentToken.type == KW_INT )
   {
//		ReadNextToken();
      DataDef();
      Declarations2();
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in declarations() function";
}

void Parser::DataDef()
{
   OutputDepth( "data_def" );

   if( mCurrentToken.type == KW_INT )
   {
      Type();
      Declarators();
      if( mCurrentToken.type == SEMICOLON )
      {
         ReadNextToken();
         --mDepth;
         return;
      }
   }
   throw ( std::string )"Parsing error in data_def() function";
}

void Parser::Declarations2()
{
   OutputDepth( "declarations2" );

   if( mCurrentToken.type == KW_INT )
   {
      Declarations();
      --mDepth;
      return;
   }
   if(	mCurrentToken.type == LEFT_BRACE  || mCurrentToken.type == IDENTIFIER		  ||
         mCurrentToken.type == NUMBER		 || mCurrentToken.type == LEFT_PAREN		  || 
         mCurrentToken.type == MINUS		 || mCurrentToken.type == NOT			        || 
         mCurrentToken.type == KW_IF		 || mCurrentToken.type == KW_WHILE		     ||
         mCurrentToken.type == KW_DO		 || mCurrentToken.type == KW_CONTINUE	     ||
         mCurrentToken.type == KW_BREAK	 || mCurrentToken.type == KW_RETURN		     ||
         mCurrentToken.type == SEMICOLON	 || mCurrentToken.type == RIGHT_BRACE	  )
   {
      --mDepth;
      return; // Empty transition
   }
   throw ( std::string )"Parsing error in declarations2() function";
}

void Parser::Statements( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "statements" );

   if(	mCurrentToken.type == LEFT_BRACE  || mCurrentToken.type == IDENTIFIER		  ||
         mCurrentToken.type == NUMBER		 || mCurrentToken.type == LEFT_PAREN		  || 
         mCurrentToken.type == MINUS		 || mCurrentToken.type == NOT			        || 
         mCurrentToken.type == KW_IF		 || mCurrentToken.type == KW_WHILE		     ||
         mCurrentToken.type == KW_DO		 || mCurrentToken.type == KW_CONTINUE	     ||
         mCurrentToken.type == KW_BREAK	 || mCurrentToken.type == KW_RETURN		     ||
         mCurrentToken.type == SEMICOLON	 										  )
   {
      Statement( returnLabel, loopStartLabel, loopEndLabel );
      Statements2( returnLabel, loopStartLabel, loopEndLabel );
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in Statements() function";
}

void Parser::Statements2( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "statements2" );

   if(	mCurrentToken.type == LEFT_BRACE  || mCurrentToken.type == IDENTIFIER		  ||
         mCurrentToken.type == NUMBER		 || mCurrentToken.type == LEFT_PAREN		  || 
         mCurrentToken.type == MINUS		 || mCurrentToken.type == NOT			        || 
         mCurrentToken.type == KW_IF		 || mCurrentToken.type == KW_WHILE		     ||
         mCurrentToken.type == KW_DO		 || mCurrentToken.type == KW_CONTINUE	     ||
         mCurrentToken.type == KW_BREAK	 || mCurrentToken.type == KW_RETURN		     ||
         mCurrentToken.type == SEMICOLON	 										  )
   {
      Statements( returnLabel, loopStartLabel, loopEndLabel );
      --mDepth;
      return;
   }
   if( mCurrentToken.type == RIGHT_BRACE )
   {
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in Statements2() function";
}

void Parser::Statement( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "statement" );

   switch( mCurrentToken.type )
   {
   case LEFT_BRACE:
      mSymbolTable.CreateLevel();
      CompStatement( returnLabel, loopStartLabel, loopEndLabel );
      mSymbolTable.DeleteLevel();
      break;
   case IDENTIFIER: case NUMBER: case LEFT_PAREN: case MINUS: case NOT:
      Expr();
      if( mCurrentToken.type == SEMICOLON )
      {
         mGenerator.GenPop( "eax" );
         ReadNextToken();
      }
      else throw ( std::string )"Parsing error in Statement() function (SEMICOLON not found after expression)";
      break;
   case KW_IF:
      IfStatement( returnLabel, loopStartLabel, loopEndLabel );
      break;
   case KW_WHILE:
      WhileStatement( returnLabel );
      break;
   case KW_DO:
      DoStatement( returnLabel );
      break;
   case KW_CONTINUE:
      ReadNextToken();
      mGenerator.GenJmp( loopStartLabel );
      if( mCurrentToken.type == SEMICOLON ) ReadNextToken();
      else throw ( std::string )"Parsing error in Statement() function (SEMICOLON not found after continue)";
      break;
   case KW_BREAK:
      ReadNextToken();
      mGenerator.GenJmp( loopEndLabel );
      if( mCurrentToken.type == SEMICOLON ) ReadNextToken();
      else throw ( std::string )"Parsing error in Statement() function (SEMICOLON not found after break)";
      break;
   case KW_RETURN:
      ReadNextToken();
      Expr2();
      mGenerator.GenReturn( returnLabel );
      if( mCurrentToken.type == SEMICOLON )
      {
         ReadNextToken();
      }
      else throw ( std::string )"Parsing error in Statement() function (SEMICOLON not found after return statement)";
      break;
   case SEMICOLON:
      ReadNextToken();
      break;
   default:
      throw ( std::string )"Parsing error in Statement() function";
      break;
   } 
}

void Parser::IfStatement( int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "if_statement" );

   if( mCurrentToken.type == KW_IF )
   {
      ReadNextToken();
      if( mCurrentToken.type == LEFT_PAREN )
      {
         ReadNextToken();
         Expr();
         int ifEndlabel = mGenerator.GenIfStart();
         if( mCurrentToken.type == RIGHT_PAREN )
         {
            ReadNextToken();
            Statement( returnLabel, loopStartLabel, loopEndLabel );
            Else2( ifEndlabel, returnLabel, loopStartLabel, loopEndLabel );
            --mDepth; return;
         }
      }
   }
   throw ( std::string )"Parsing error in if_Statement() function";
}

void Parser::Else2( int ifEndlabel, int returnLabel, int loopStartLabel, int loopEndLabel )
{
   OutputDepth( "else2" );

   if( mCurrentToken.type == KW_ELSE )
   {
      int elseEndLabel = mGenerator.GenJmp();
      mGenerator.GenLabel( ifEndlabel );
      ReadNextToken();
      Statement( returnLabel, loopStartLabel, loopEndLabel );
      mGenerator.GenLabel( elseEndLabel );
      --mDepth; return;
   }
   if(	mCurrentToken.type == LEFT_BRACE  || mCurrentToken.type == IDENTIFIER		  ||
         mCurrentToken.type == NUMBER		 || mCurrentToken.type == LEFT_PAREN		  || 
         mCurrentToken.type == MINUS		 || mCurrentToken.type == NOT			        || 
         mCurrentToken.type == KW_IF		 || mCurrentToken.type == KW_WHILE		     ||
         mCurrentToken.type == KW_DO		 || mCurrentToken.type == KW_CONTINUE	     ||
         mCurrentToken.type == KW_BREAK	 || mCurrentToken.type == KW_RETURN		     ||
         mCurrentToken.type == SEMICOLON	 || mCurrentToken.type == RIGHT_BRACE	  )
   {
      mGenerator.GenLabel( ifEndlabel );
      --mDepth;
      return; // Empty transition
   }
   throw ( std::string )"Parsing error in else2() function";
}

void Parser::WhileStatement( int returnLabel )
{
   OutputDepth( "while_statement" );

   if( mCurrentToken.type == KW_WHILE )
   {
      ReadNextToken();
      if( mCurrentToken.type == LEFT_PAREN )
      {
         ReadNextToken();
         int whileStartLabel = mGenerator.GenWhileStart();
         Expr();
         int whileFinishLabel = mGenerator.GenWhileMiddle();
         if( mCurrentToken.type == RIGHT_PAREN )
         {
            ReadNextToken();
            Statement( returnLabel, whileStartLabel, whileFinishLabel );
            mGenerator.GenJmp( whileStartLabel );
            mGenerator.GenLabel( whileFinishLabel );
            --mDepth;
            return;
         }
      }
   }
   throw ( std::string )"Parsing error in while_Statement() function";
}

void Parser::DoStatement( int returnLabel )
{
   OutputDepth( "do_statement" );

   if( mCurrentToken.type == KW_DO )
   {
      ReadNextToken();
      int doStartLabel = mGenerator.GenDoStart();
      int doEvaluationLabel = mGenerator.CreateLabel();
      int doFinishLabel = mGenerator.CreateLabel();
      Statement( returnLabel, doEvaluationLabel, doFinishLabel );
      if( mCurrentToken.type == KW_WHILE )
      {
         ReadNextToken();
         if( mCurrentToken.type == LEFT_PAREN )
         {
            ReadNextToken();
            mGenerator.GenLabel( doEvaluationLabel );
            Expr();
            mGenerator.GenDoMiddle( doStartLabel );
            if( mCurrentToken.type == RIGHT_PAREN )
            {
               ReadNextToken();
               if( mCurrentToken.type == SEMICOLON )
               {
                  ReadNextToken();
                  mGenerator.GenLabel( doFinishLabel );
                  --mDepth;
                  return;
               }
            }
         }
      }
   }
   throw ( std::string )"Parsing error in do_Statement() function";
}

void Parser::Expr2()
{	
   OutputDepth( "expr2" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      Expr();
      --mDepth;
      return;
   }
   if( mCurrentToken.type == SEMICOLON )
   {
      --mDepth;
      return; // do nothing
   }
   throw ( std::string )"Parsing error in Expr2() function";
}

int Parser::Exprs2( std::string funcName )
{	
   OutputDepth( "exprs2" );
   int paramCount = 0;
   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN || mCurrentToken.type == COMPLETED_STRING)
   {
      paramCount = Exprs( paramCount, funcName );
      --mDepth;
      return paramCount;
   }
   if( mCurrentToken.type == RIGHT_PAREN )
   {
      --mDepth;
      return paramCount; // do nothing
   }
   throw ( std::string )"Parsing error in exprs2() function";
}

int Parser::Exprs( int paramCount, std::string funcName )
{
   OutputDepth( "exprs" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      Expr();
      paramCount = Exprs3( ++paramCount, funcName );
      --mDepth;
      return paramCount;
   }
   // string
   if( mCurrentToken.type == COMPLETED_STRING )
   {
      if( funcName != "printf" )
      {
         std::string error = "Type mismatch in function call " + funcName;
         throw error;
      }
      mGenerator.GenStringPush( mCurrentToken.token_string );
      ReadNextToken();
      paramCount = Exprs3( ++paramCount, funcName );
      --mDepth;
      return paramCount; 
   }
   throw ( std::string )"Parsing error in exprs() function";
}

int Parser::Exprs3( int paramCount, std::string funcName )
{	
   OutputDepth( "exprs3" );

   if( mCurrentToken.type == COMMA )
   {
         ReadNextToken();
         paramCount = Exprs( paramCount, funcName );
         --mDepth;
         return paramCount;
   }
   if( mCurrentToken.type == RIGHT_PAREN )
   {
      // do nothing
      --mDepth;
      return paramCount; 
   }
   throw ( std::string )"Parsing error in exprs3() function";
}

void Parser::Expr()
{
   OutputDepth( "expr" );

   if( mCurrentToken.type == IDENTIFIER )
   {
      // хак LL( 2 ), подсматриваем следующий токен
      if( mLexer.SeeToken( ).type == ASSIGN)
      {
         std::string addrStr = GetIdAddress( mCurrentToken.token_string );
         ReadNextToken();
         ReadNextToken();
         Expr();
         mGenerator.GenAssign( addrStr );
         --mDepth;
         return; // вторую ветку уже не надо проверять
      }
   }
   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L0();
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in Expr() function";
}

void Parser::L0()
{
   OutputDepth( "L0" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
         L1();
         T0();
   }
   else throw ( std::string )"Parsing error in L0() function";
   --mDepth;
   return;
}

void Parser::T0()
{
   OutputDepth( "t0" );

   if( mCurrentToken.type == OR )
   {
      ReadNextToken();
      L0();
      mGenerator.GenOr();
   }
   else if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON ||
         mCurrentToken.type == COMMA || mCurrentToken.type == COMPLETED_STRING)
   {
      --mDepth;
      return; // do nothing
   }
   else throw ( std::string )"Parsing error in t0() function";
}

void Parser::L1()
{
   OutputDepth( "L1" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L2();
      T1();
   }
   else throw ( std::string )"Parsing error in L1() function";
   --mDepth;
   return;
}

void Parser::T1()
{
   OutputDepth( "T1" );

   if( mCurrentToken.type == AND )
   {
      ReadNextToken();
      L2();
      mGenerator.GenAnd();
      T1();
   }
   else if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON ||
         mCurrentToken.type == COMMA || mCurrentToken.type == COMPLETED_STRING || 
         mCurrentToken.type == OR)
   {
      --mDepth;
      return; // do nothing
   }
   else throw ( std::string )"Parsing error in T1() function";
}

void Parser::L2()
{
   OutputDepth( "L2" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L3();
      T2();
   }
   else throw ( std::string )"Parsing error in L2() function";
   --mDepth;
   return;
}

void Parser::T2()
{
   OutputDepth( "t2" );

   if( mCurrentToken.type == EQUALS )
   {
      ReadNextToken();
      L3();
      mGenerator.GenCompare( "==" );
      T2();
      --mDepth;
      return;
   } 
   if( mCurrentToken.type == NOT_EQUALS )
   {
      ReadNextToken();
      L3();
      mGenerator.GenCompare( "!=" );
      T2();
      --mDepth;
      return;
   }
   if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON ||
         mCurrentToken.type == COMMA || mCurrentToken.type == COMPLETED_STRING || 
         mCurrentToken.type == OR || mCurrentToken.type == AND)
   {
      --mDepth;
      return; // do nothing
   }
   throw ( std::string )"Parsing error in T2() function";
}

void Parser::L3()
{
   OutputDepth( "L3" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L4();
      T3();
   }
   else throw ( std::string )"Parsing error in L3() function";
   --mDepth;
   return;
}

void Parser::T3()
{	
   OutputDepth( "t3" );

   if( mCurrentToken.type == LESS )
   {
      ReadNextToken();
      L4();
      mGenerator.GenCompare( "<" );
      T3();
      --mDepth;
      return;
   } 
   if( mCurrentToken.type == MORE )
   {
      ReadNextToken();
      L4();
      mGenerator.GenCompare( ">" );
      T3();
      --mDepth;
      return;
   }
   if( mCurrentToken.type == LESS_EQUALS )
   {
      ReadNextToken();
      L4();
      mGenerator.GenCompare( "<=" );
      T3();
      --mDepth;
      return;
   }
   if( mCurrentToken.type == MORE_EQUALS )
   {
      ReadNextToken();
      L4();
      mGenerator.GenCompare( ">=" );
      T3();
      --mDepth;
      return;
   }
   if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON ||
         mCurrentToken.type == COMMA || mCurrentToken.type == COMPLETED_STRING || 
         mCurrentToken.type == OR || mCurrentToken.type == AND || 
         mCurrentToken.type == EQUALS || mCurrentToken.type == NOT_EQUALS)
   {
      --mDepth;
      return; // do nothing
   }
   throw ( std::string )"Parsing error in T3() function";
}

void Parser::L4()
{
   OutputDepth( "L4" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L5();
      T4();
   }
   else throw ( std::string )"Parsing error in L4() function";
   --mDepth;
   return;
}

void Parser::T4()
{
   OutputDepth( "t4" );

   if( mCurrentToken.type == PLUS )
   {
      ReadNextToken();
      L5();
      mGenerator.GenBinaryOperation( "add" );
      T4();
      --mDepth;
      return;
   } 
   if( mCurrentToken.type == MINUS )
   {
      ReadNextToken();
      L5();
      mGenerator.GenBinaryOperation( "sub" );
      T4();
      --mDepth;
      return;
   }
   if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON		     ||
      mCurrentToken.type == COMMA		 || mCurrentToken.type == COMPLETED_STRING  || 
      mCurrentToken.type == OR			 || mCurrentToken.type == AND			        || 
      mCurrentToken.type == EQUALS		 || mCurrentToken.type == NOT_EQUALS		  ||
      mCurrentToken.type == LESS		    || mCurrentToken.type == MORE			     ||
      mCurrentToken.type == LESS_EQUALS || mCurrentToken.type == MORE_EQUALS	  )
   {
      --mDepth;
      return; // do nothing
   }
   throw ( std::string )"Parsing error in T4() function";
}

void Parser::L5()
{
   OutputDepth( "L5" );

   if(mCurrentToken.type == IDENTIFIER || mCurrentToken.type == MINUS || mCurrentToken.type == NOT ||
      mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN)
   {
      L6();
      T5();
   }
   else throw ( std::string )"Parsing error in L5() function";
   --mDepth;
   return;
}

void Parser::T5()
{
   OutputDepth( "t5" );

   if( mCurrentToken.type == MULT )
   {
      ReadNextToken();
      L6();
      mGenerator.GenMultiplication();
      T5();
      --mDepth;
      return;
   } 
   if( mCurrentToken.type == DIVISION )
   {
      ReadNextToken();
      L6();
      mGenerator.GenDivision();
      T5();
      --mDepth;
      return;
   }
   if(	mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON		     ||
         mCurrentToken.type == COMMA		 || mCurrentToken.type == COMPLETED_STRING  || 
         mCurrentToken.type == OR			 || mCurrentToken.type == AND			        || 
         mCurrentToken.type == EQUALS		 || mCurrentToken.type == NOT_EQUALS		  ||
         mCurrentToken.type == LESS		    || mCurrentToken.type == MORE			     ||
         mCurrentToken.type == LESS_EQUALS || mCurrentToken.type == MORE_EQUALS	     ||
         mCurrentToken.type == MINUS		 || mCurrentToken.type == PLUS			  )
   {
      --mDepth;
      return; // do nothing
   }
   throw ( std::string )"Parsing error in T5() function";
}

void Parser::L6()
{	
   OutputDepth( "L6" );

   if( mCurrentToken.type == MINUS )
   {
      ReadNextToken();
      L6();
      mGenerator.GenNegate();
      --mDepth;
      return;
   } 
   if( mCurrentToken.type == NOT )
   {
      ReadNextToken();
      L6();
      mGenerator.GenNot();
      --mDepth;
      return;
   }
   if( mCurrentToken.type == IDENTIFIER || mCurrentToken.type == NUMBER || mCurrentToken.type == LEFT_PAREN )
   {
      L7();
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in L6() function";
}

void Parser::L7()
{	
   OutputDepth( "L7" );

   // parens
   if( mCurrentToken.type == LEFT_PAREN )
   {
      ReadNextToken();
      Expr();
      if( mCurrentToken.type == RIGHT_PAREN )
      {
         ReadNextToken();
      }
      else throw ( std::string )"Parsing error in L7() function (RIGHT_PAREN not found)";
      --mDepth;
      return;
   } 
   // number
   if( mCurrentToken.type == NUMBER )
   {
      mGenerator.GenPush( mCurrentToken.token_string + 'd' );
      ReadNextToken();
      --mDepth;
      return;
   }
   // ident, call L8
   if( mCurrentToken.type == IDENTIFIER )
   {
      std::string idName = mCurrentToken.token_string;
      ReadNextToken();
      L8( idName );
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in L7() function";
}

void Parser::L8( std::string idName )
{
   OutputDepth( "L8" );

   // function call
   if( mCurrentToken.type == LEFT_PAREN )
   {
      FuncCall( idName );
      --mDepth;
      return;
   } 
   // identifier
   if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON		     ||
      mCurrentToken.type == COMMA		 || mCurrentToken.type == COMPLETED_STRING  || 
      mCurrentToken.type == OR			 || mCurrentToken.type == AND			        || 
      mCurrentToken.type == EQUALS		 || mCurrentToken.type == NOT_EQUALS		  ||
      mCurrentToken.type == LESS		    || mCurrentToken.type == MORE			     ||
      mCurrentToken.type == LESS_EQUALS || mCurrentToken.type == MORE_EQUALS	     ||
      mCurrentToken.type == MINUS		 || mCurrentToken.type == PLUS			     ||
      mCurrentToken.type == MULT		    || mCurrentToken.type == DIVISION		  )
   {
      Identifier( idName );
      --mDepth;
      return; // do nothing
   }

   throw ( std::string )"Parsing error in L8() function";
}

void Parser::FuncCall( std::string funcName )
{
   OutputDepth( "func_call" );
   
   if( mCurrentToken.type == LEFT_PAREN )
   {
      // int address = mSymbolTable.getIdentifierAddress( idName );
      // достать из Global Table функцию. Тогда мы будем знать заранее, сколько ждать параметров. Исходя из этого и проверять.
      int paramCountDeclaration = GetFuncParamsNumber( funcName );
      ReadNextToken();
      int paramCountCall = Exprs2( funcName );
      if( paramCountDeclaration >= 0 && paramCountDeclaration == paramCountCall || funcName == "printf" )
      {
         mGenerator.GenFuncCall( funcName, paramCountCall );
      }
      else
      {
         std::string error = "Function " + funcName + " requies different set of parameters.\n";
         error += "Declared number: " + std::to_string( static_cast< long double >( paramCountDeclaration ) ) + '\n';
         error += "Called number: " + std::to_string( static_cast< long double >( paramCountCall ) ) + '\n';
         throw error;
      }
      if( mCurrentToken.type == RIGHT_PAREN )
      {
         ReadNextToken();
      }
      else throw ( std::string )"Parsing error in func_call() function (RIGHT_PAREN not found)";
      --mDepth; return;
   }
   throw ( std::string )"Parsing error in func_call() function";
}

void Parser::Identifier( std::string idName )
{
   OutputDepth( "identifier" );

   if(mCurrentToken.type == RIGHT_PAREN || mCurrentToken.type == SEMICOLON		     ||
      mCurrentToken.type == COMMA		 || mCurrentToken.type == COMPLETED_STRING  || 
      mCurrentToken.type == OR			 || mCurrentToken.type == AND			        || 
      mCurrentToken.type == EQUALS		 || mCurrentToken.type == NOT_EQUALS		  ||
      mCurrentToken.type == LESS		    || mCurrentToken.type == MORE		    	  ||
      mCurrentToken.type == LESS_EQUALS || mCurrentToken.type == MORE_EQUALS	     ||
      mCurrentToken.type == MINUS		 || mCurrentToken.type == PLUS			     ||
      mCurrentToken.type == MULT		    || mCurrentToken.type == DIVISION		  )
   {		
      std::string address = GetIdAddress( idName );
      mGenerator.GenPush( address );
      --mDepth;
      return;
   }
   throw ( std::string )"Parsing error in identifier() function";
}

std::string Parser::GetIdAddress( std::string idName )
{
   int address = mSymbolTable.GetIdentifierAddress( idName );
   std::string addrStr = "";
   // if not found
   if( address == -1 )
   {
      int paramCount = mGlobalTable.Check( idName );
      // if function
      if( paramCount >= 0 )
      {
         throw ( std::string ) "Cannot use function " + idName + " as variable.";
      }
      // if not found
      if( paramCount == -2 )
      {
         throw ( std::string ) idName + ": undeclared identifier.";
      }
      addrStr += '_' + idName;
   }
   // if found
   else
   {
      addrStr += "[ebp";
      if( address > 0 ) addrStr += '+';
      addrStr +=	std::to_string( static_cast< long double >( address ) ) + "]";
   }
   return addrStr;
}

int Parser::GetFuncParamsNumber( std::string funcName )
{
   int address = mSymbolTable.GetIdentifierAddress( funcName );
   // if not found
   if( address == -1 )
   {
      int paramCount = mGlobalTable.Check( funcName );
      // if function
      if( paramCount >= 0 )
      {
         return paramCount;
      }
      // if global identifier
      if( paramCount == -1 )
      {
         throw ( std::string ) funcName + ": not a function.";
      }
      // if not found
      if( paramCount == -2 )
      {
         // printf as an external function
         if( funcName == "printf" ) return 1;
         throw ( std::string ) funcName + ": undeclared function.";
      }
      throw ( std::string ) funcName + ": error while getting parameters";
   }
   // if found
   else
   {
      throw ( std::string ) funcName + ": not a function.";
   }
}