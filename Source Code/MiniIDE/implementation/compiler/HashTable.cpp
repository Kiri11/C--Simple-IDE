#include "HashTable.h"

bool HashTable::Insert( std::string string_for_inserting )
{
   if( Check( string_for_inserting ) )
   {
      return 0;
   }
   else
   {
      mSet.insert( string_for_inserting );
      return 1;
   }
}

bool HashTable::Check( std::string string_for_check )
{
   return mSet.count( string_for_check );
}