#include "Lexer.h"


// не забыть, что у нас две такие функции
Token Lexer::GetToken()
{
   FSM fsm;
   State fsm_state;
   int l = mFullInput.length();


   // go through states until it halts or blocks
   while( mInputIterator < l )
   {
      if( ( fsm_state = fsm.Transition( mFullInput[mInputIterator] ) ) != sProgress )
         break;
      else 
         ++mInputIterator;
   }

   if( mInputIterator == l )
   {
      fsm_state = fsm.Transition( EOF );
   }
   
   // building error string and throwing it
   if( fsm_state == sBlock )
   {
      std::string error_str;
      error_str = "\nLexical error when reading symbol: " + 
      error_str += mFullInput[mInputIterator];
      throw error_str;
   }

   // get token
   Token result_token;
   result_token.type = fsm.GetCurrentState();
   result_token.token_string = fsm.GetCurrentString();
   CheckKeyWord( result_token );
   // adding to string table. Хотя зачем вообще это нужно?
   if( result_token.type == IDENTIFIER )
   { 
      mStringTable.Insert( result_token.token_string );
   }

   return result_token;
}

Token Lexer::SeeToken()
{
   FSM fsm;
   State fsm_state;
   int l = mFullInput.length();
   int innerIterator = mInputIterator;

   // go through states until it halts or blocks
   while( innerIterator < l )
   {
      if( ( fsm_state = fsm.Transition( mFullInput[innerIterator] ) ) != sProgress )
         break;
      else 
         ++innerIterator;
   }

   if( innerIterator == l )
   {
      fsm_state = fsm.Transition( EOF );
   }
   
   // building error string and throwing it
   if( fsm_state == sBlock )
   {
      std::string error_str;
      error_str = "\nLexical error when reading symbol: " + 
      error_str += mFullInput[innerIterator];
      throw error_str;
   }

   // get token
   Token result_token;
   result_token.type = fsm.GetCurrentState();
   result_token.token_string = fsm.GetCurrentString();
   CheckKeyWord( result_token );
   // adding to string table. Хотя зачем вообще это нужно?
   if( result_token.type == IDENTIFIER )
   { 
      mStringTable.Insert( result_token.token_string );
   }

   return result_token;
}

Lexer::Lexer( std::string input_string )
{
   mFullInput = input_string;
   mInputIterator = 0;
}

Lexer::Lexer()
{
   mFullInput = "";
   mInputIterator = 0;
}

void Lexer::CheckKeyWord( Token &kw_token )
{
   if( kw_token.type != IDENTIFIER )
      return;
   if( kw_token.token_string == "int" )
   { 
      kw_token.type = KW_INT;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "if" )
   {
      kw_token.type = KW_IF;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "else" )
   { 
      kw_token.type = KW_ELSE;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "while" )
   { 
      kw_token.type = KW_WHILE;
      kw_token.token_string = "";
      return; }
   if( kw_token.token_string == "do" )
   { 
      kw_token.type = KW_DO;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "continue" )
   { 
      kw_token.type = KW_CONTINUE;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "break" )
   { 
      kw_token.type = KW_BREAK;
      kw_token.token_string = "";
      return; 
   }
   if( kw_token.token_string == "return" )
   {
      kw_token.type = KW_RETURN;
      kw_token.token_string = "";
      return; 
   }
   if(kw_token.token_string == "auto"		||	kw_token.token_string == "for"		||
      kw_token.token_string == "typedef"	||	kw_token.token_string == "double"	||
      kw_token.token_string == "goto"		||	kw_token.token_string == "short"	   ||
      kw_token.token_string == "union"	   ||	kw_token.token_string == "case"		||
      kw_token.token_string == "sizeof"	||	kw_token.token_string == "unsigned"	||
      kw_token.token_string == "char"		||	kw_token.token_string == "enum"		||
      kw_token.token_string == "static"	||	kw_token.token_string == "extern"	||
      kw_token.token_string == "long"		||	kw_token.token_string == "struct"	||
      kw_token.token_string == "default"	||	kw_token.token_string == "float"	   ||
      kw_token.token_string == "register"	||	kw_token.token_string == "switch"	)
   { 
      kw_token.type = KW_RESERVED; 
   }
   return;
}

void Lexer::SetInputString( std::string inputString )
{
   mFullInput = inputString;
}