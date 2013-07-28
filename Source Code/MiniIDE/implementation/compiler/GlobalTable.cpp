#include "GlobalTable.h"

int GlobalTable::Check( std::string idName )
{
   if( mGlobals.count( idName ) )
   {
      return mGlobals.at( idName );
   }
   else
   {
      // not found
      return -2;
   }
}

void GlobalTable::Insert( std::string idName, int isFunction )
{
   if( mGlobals.count( idName ) == 0 )
   { 
      mGlobals.insert( std::make_pair( idName, isFunction ) );
   }
   else
   {
      std::string errorStr = "Semantic Error: Redifinition of global variable " + idName;
      throw errorStr;
   }
}