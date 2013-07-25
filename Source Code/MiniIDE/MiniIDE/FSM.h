#pragma once
#include <string>
#include "Token.h"

enum CharType
{
   ctAlpha,
   ctDigit,
   ctSpace,
   ctSmth
};

enum State
{
   sBlock,
   sHalt,
   sProgress
};

// Finite-state Machine
class FSM
{
public:

   FSM();
   void Reset();
   token_type GetCurrentState();
   // Transition returns 0 if block, 1 if halt, 2 if in sProgress
   State Transition( char input );  
   std::string GetCurrentString();

private:

   CharType CheckCharType( char ch );

   token_type mCurrentState;
   std::string mCurrentString;
};