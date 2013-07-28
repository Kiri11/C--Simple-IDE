#include "FSM.h"

FSM::FSM()
{
   Reset();
}

token_type FSM::GetCurrentState()
{
   return mCurrentState;
}

void FSM::Reset()
{
   mCurrentState = INITIAL;
   mCurrentString = "";
}

CharType FSM::CheckCharType( char ch )
{
   if( isdigit( ch ) ) return ctDigit;
   if( isalpha( ch ) ) return ctAlpha;
   if( isspace( ch ) || ch == EOF ) return ctSpace;
   return ctSmth;
}

std::string FSM::GetCurrentString()
{
   return mCurrentString;
}

State FSM::Transition( char input )
{
   CharType input_type = CheckCharType( input );
   switch( mCurrentState )
   {
   case INITIAL:
      if( input == EOF )
      {
         mCurrentState = END;
         return sHalt;
      }
      if( input_type == ctSpace )
      {
         return sProgress;
      }
      if( input_type == ctDigit )
      {
         mCurrentState = NUMBER;
         mCurrentString += input;
         return sProgress;
      }
      if( input_type == ctAlpha || input == '_' )
      {
         mCurrentState = IDENTIFIER;
         mCurrentString += input;
         return sProgress;
      }
      if( input == '"' )
      {
         mCurrentState = STRING_IN_PROC;
         return sProgress;
      }
      if( input == '&' )
      {
         mCurrentState = AMPERSAND;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '|' )
      {
         mCurrentState = BAR;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == ';' )
      {
         mCurrentState = SEMICOLON;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '*' )
      {
         mCurrentState = MULT;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '+' )
      {
         mCurrentState = PLUS;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '-' )
      {
         mCurrentState = MINUS;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '(' )
      {
         mCurrentState = LEFT_PAREN;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == ')')
      {
         mCurrentState = RIGHT_PAREN;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '{' )
      {
         mCurrentState = LEFT_BRACE;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '}' )
      {
         mCurrentState = RIGHT_BRACE;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == ',' )
      {
         mCurrentState = COMMA;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '=' )
      {
         mCurrentState = ASSIGN;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '!' )
      {
         mCurrentState = NOT;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '<' )
      {
         mCurrentState = LESS;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '>' )
      {
         mCurrentState = MORE;
         //mCurrentString += input;
         return sProgress;
      }
      if( input == '/' )
      {
         mCurrentState = DIVISION;
         //mCurrentString += input;
         return sProgress;
      }
      return sBlock;
   case STRING_IN_PROC:
      if( input == '\\' ) 
      {
         mCurrentState = STRING_ESCAPE;
         return sProgress;
      }
      if( input == '"' )
      {
         mCurrentState = COMPLETED_STRING;
         return sProgress;
      }
      if( input == '\n' )
      {
         return sBlock;
      }
      mCurrentString += input;
      return sProgress;
   case STRING_ESCAPE:
      if( input == 't' )
      {
         mCurrentState = STRING_IN_PROC;
         mCurrentString += '\t';
         return sProgress;
      }
      // передвигаем каретку назад, чтобы избежать пустых строк. По идее не должно влиять
      if( input == 'n' )
      {
         mCurrentState = STRING_IN_PROC;
         mCurrentString += "\b\", 0Ah, \"\b";
         return sProgress;
      }
      if( input == '\\' )
      {
         mCurrentState = STRING_IN_PROC;
         mCurrentString += '\\';
         return sProgress;
      }
      if( input == '"' )
      {
         mCurrentState = STRING_IN_PROC;
         mCurrentString += '"';
         return sProgress;
      }
      if( input == 'b' )
      {
         mCurrentState = STRING_IN_PROC;
         mCurrentString += '\b';
         return sProgress;
      }
      return sBlock;
   case COMM_LINE:
      if( input == '\n' )
      {
         mCurrentState = INITIAL;
      }
      return sProgress;
   case COMM_IN_PROC:
      if( input == EOF )
      {
         return sBlock;
      }
      if( input == '*' )
      {
         mCurrentState = COMM_ABOUT_TO_FINISH;
      }
      return sProgress;
   case COMM_ABOUT_TO_FINISH:
      if( input == EOF )
      {
         return sBlock;
      }
      if( input == '/' )
      {
         mCurrentState = INITIAL;
         return sProgress;
      }
      mCurrentState = COMM_IN_PROC;
      return sProgress;
   case AMPERSAND:
      if( input == '&' )
      {
         //mCurrentString += input;
         mCurrentState = AND;
         return sProgress;
      }
      return sBlock;
   case BAR:
      if( input == '|' )
      {
         //mCurrentString += input;
         mCurrentState = OR;
         return sProgress;
      }
      return sBlock;
   case IDENTIFIER:
      if( input_type == ctAlpha || input_type == ctDigit || input == '_' )
      {
         mCurrentString += input;
         return sProgress;
      }
      return sHalt;
   case NUMBER:
      if( input_type == ctDigit )
      {
         mCurrentString += input;
         return sProgress;
      }
      if( input_type == ctAlpha || input == '_' )
      {
         return sBlock;
      }
      return sHalt;
   case SEMICOLON:
      return sHalt;
   case MULT:
      return sHalt;
   case PLUS:
      return sHalt;
   case MINUS:
      return sHalt;
   case LEFT_PAREN:
      return sHalt;
   case RIGHT_PAREN:
      return sHalt;
   case LEFT_BRACE:
      return sHalt;
   case RIGHT_BRACE:
      return sHalt;
   case COMMA:
      return sHalt;
   case ASSIGN:
      if( input == '=' )
      {
         mCurrentState = EQUALS;
         //mCurrentString += input;
         return sProgress;
      }
      return sHalt;
   case NOT:
      if( input == '=' )
      {
         mCurrentState = NOT_EQUALS;
         //mCurrentString += input;
         return sProgress;
      }
      if( input_type == ctSpace || input == '/' )
      {
         return sBlock;
      }
      return sHalt;
   case LESS:
      if( input == '=' )
      {
         mCurrentState = LESS_EQUALS;
         //mCurrentString += input;
         return sProgress;
      }
      return sHalt;
   case MORE:
      if( input == '=' )
      {
         mCurrentState = MORE_EQUALS;
         //mCurrentString += input;
         return sProgress;
      }
      return sHalt;
   case EQUALS:
      return sHalt;
   case NOT_EQUALS:
      return sHalt;
   case LESS_EQUALS:
      return sHalt;
   case MORE_EQUALS:
      return sHalt;
   case AND:
      return sHalt;
   case OR:
      return sHalt;
   case COMPLETED_STRING:
      return sHalt;
   case DIVISION:
      if( input == '/' )
      {
         mCurrentState = COMM_LINE;
         mCurrentString = "";
         return sProgress;
      }
      if( input == '*' )
      {
         mCurrentState = COMM_IN_PROC;
         mCurrentString = "";
         return sProgress;
      }
      return sHalt;
      break;
   case END:
      return sHalt;
   default:
      break;
   }
   return sBlock;
}