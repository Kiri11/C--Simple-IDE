#pragma once
#include <string>

enum token_type
{
   INITIAL, 
   STRING_IN_PROC, STRING_ESCAPE, 
   COMM_LINE, COMM_IN_PROC, COMM_ABOUT_TO_FINISH,											// Comments states
   AMPERSAND, BAR,																			// Can only halt after the second same symbol
   IDENTIFIER, NUMBER, 
   SEMICOLON, MULT, PLUS, MINUS, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA,	// Can only halt right away
   ASSIGN, NOT, LESS, MORE,																// Can either halt right away or after the equals sign
   EQUALS, NOT_EQUALS, LESS_EQUALS, MORE_EQUALS,											// Same as previous group but with equals sign
   AND, OR,																				// &&, ||
   COMPLETED_STRING,
   DIVISION,																				// Can halt as division sign, or proceed to one of two comment types
   BLOCK,																					// Only for outside use
   END,
   KW_INT, KW_IF, KW_ELSE, KW_WHILE, KW_DO, KW_CONTINUE, KW_BREAK, KW_RETURN,				// key words
   KW_RESERVED
};

class Token
{
public:
   token_type type;
   std::string token_string;
   std::string ToString();
};