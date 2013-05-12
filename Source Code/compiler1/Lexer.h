#pragma once
#include <string>
#include "FSM.h"
#include "Token.h"
#include "HashTable.h"

class Lexer
{
public:

   Lexer();
   Lexer( std::string input_string );
   Token GetToken();
   Token SeeToken(); 
   void SetInputString( std::string inputString );
   
private:
//	Lexer(); // don't allow to create empty lexer
   std::string mFullInput;
   int mInputIterator;
   void CheckKeyWord( Token &kw_token );
   HashTable mStringTable;
};