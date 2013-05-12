#include "SymbolTable.h"

//SymbolTable::SymbolTable(){
   //createLevel(); // ?
//}

void SymbolTable::CreateLevel()
{
   HashMap newLevel;
   mLocalTable.push_front( newLevel );
   mParamTable.push_front( newLevel );
}

void SymbolTable::DeleteLevel()
{
   mLocalTable.pop_front();
   mParamTable.pop_front();
}

int SymbolTable::Insert( std::string idName, bool isIdParameter )
{
   // check last level
   // if not exist, insert
   // else throw exception
   if( GetLevel() == 0 ) 
      throw ( std::string ) "Error: trying to insert into empty symbol table.";

   if( IsIdDeclaredOnLastLevel( idName ) == 0 )
   {
      int newAddress;
      if( isIdParameter )
      {
         newAddress = GetNewParamAddress();
         mParamTable.front().insert( std::make_pair( idName, newAddress ) );
      }
      else
      {
         newAddress = GetNewLocalAddress();
         mLocalTable.front().insert( std::make_pair( idName, newAddress ) );
      }
      return newAddress;
   }
   else
   {
      std::string errorStr = "Semantic error: redefinition of identifier " + idName ;
      throw errorStr;
   }
}

bool SymbolTable::IsIdDeclaredOnLastLevel( std::string checkIdName )
{
   return ( mLocalTable.front().count( checkIdName ) || mParamTable.front().count( checkIdName ) );
}

int SymbolTable::GetNewParamAddress()
{
   int size = mParamTable.front().size();
   int address = 4 * ( size + 2 );
   return address;
}

int SymbolTable::GetNewLocalAddress()
{
   int size = mLocalTable.front().size();
   int address = -4 * ( size + 1 );
   return address;
}

int SymbolTable::GetIdentifierAddress( std::string idName )
{
   if( GetLevel() > 0 )
   {
      std::list< HashMap >::iterator itLocal = mLocalTable.begin();
      std::list< HashMap >::iterator itParam = mParamTable.begin();
      while( itLocal != mLocalTable.end() )
      {
         // если нашли на этом уровне
         if( itLocal->count( idName ) ) return itLocal->at( idName );
         if( itParam->count( idName ) ) return itParam->at( idName );
         ++itLocal;
         ++itParam;
      }	
   }
   return -1;
}

int SymbolTable::GetLevel()
{
   return mLocalTable.size();
}