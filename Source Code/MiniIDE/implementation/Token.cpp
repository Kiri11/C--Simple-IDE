#include "Token.h"

std::string Token::ToString()
{
   std::string tostring;
   switch( type )
   {
   case INITIAL:
      tostring = "INITIAL";
      break;
   case STRING_IN_PROC:
      tostring = "STRING_IN_PROC";
      break;
   case STRING_ESCAPE:
      tostring = "STRING_ESCAPE";
      break;
   case COMM_LINE:
      tostring = "COMM_LINE";
      break;
   case COMM_IN_PROC:
      tostring = "COMM_IN_PROC";
      break;
   case COMM_ABOUT_TO_FINISH:
      tostring = "COMM_ABOUT_TO_FINISH";
      break;
   case AMPERSAND:
      tostring = "AMPERSAND";
      break;
   case BAR:
      tostring = "BAR";
      break;
   case IDENTIFIER:
      tostring = "IDENTIFIER";
      break;
   case NUMBER:
      tostring = "NUMBER";
      break;
   case SEMICOLON:
      tostring = "SEMICOLON";
      break;
   case MULT:
      tostring = "MULT";
      break;
   case PLUS:
      tostring = "PLUS";
      break;
   case MINUS:
      tostring = "MINUS";
      break;
   case LEFT_PAREN:
      tostring = "LEFT_PAREN";
      break;
   case RIGHT_PAREN:
      tostring = "RIGHT_PAREN";
      break;
   case LEFT_BRACE:
      tostring = "LEFT_BRACE";
      break;
   case RIGHT_BRACE:
      tostring = "RIGHT_BRACE";
      break;
   case COMMA:
      tostring = "COMMA";
      break;
   case ASSIGN:
      tostring = "ASSIGN";
      break;
   case NOT:
      tostring = "NOT";
      break;
   case LESS:
      tostring = "LESS";
      break;
   case MORE:
      tostring = "MORE";
      break;
   case EQUALS:
      tostring = "EQUALS";
      break;
   case NOT_EQUALS:
      tostring = "NOT_EQUALS";
      break;
   case LESS_EQUALS:
      tostring = "LESS_EQUALS";
      break;
   case MORE_EQUALS:
      tostring = "MORE_EQUALS";
      break;
   case AND:
      tostring = "AND";
      break;
   case OR:
      tostring = "OR";
      break;
   case COMPLETED_STRING:
      tostring = "COMPLETED_STRING";
      break;
   case DIVISION:
      tostring = "DIVISION";
      break;
   case BLOCK:
      tostring = "BLOCK";
      break;
   case END:
      tostring = "END";
      break;
   case KW_INT:
      tostring = "KW_INT";
      break;
   case KW_IF:
      tostring = "KW_IF";
      break;
   case KW_ELSE:
      tostring = "KW_ELSE";
      break;
   case KW_WHILE:
      tostring = "KW_WHILE";
      break;
   case KW_DO:
      tostring = "KW_DO";
      break;
   case KW_CONTINUE:
      tostring = "KW_CONTINUE";
      break;
   case KW_BREAK:
      tostring = "KW_BREAK";
      break;
   case KW_RETURN:
      tostring = "KW_RETURN";
      break;
   case KW_RESERVED:
      tostring = "KW_RESERVED";
      break;
   default:
      tostring =  "CANNOT_CONVERT_TO_STRING!";
   }
   if( token_string != "" )
   {
      tostring += "(" + token_string + ")";
   }
   return tostring;
}