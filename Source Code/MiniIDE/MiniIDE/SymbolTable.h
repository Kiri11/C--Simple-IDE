#pragma once
#include <string>
#include <list>
#include <unordered_map>

typedef std::unordered_map< std::string, int > HashMap;

// возвращать наружу Identifier или kind/address по отдельности?
class SymbolTable
{
public:

   //	SymbolTable();
   void CreateLevel();
   void DeleteLevel();
   // return address
   int Insert( std::string idName, bool isIdParameter );
   // if not found, returns -1
   int GetIdentifierAddress( std::string idName );
   int GetLevel();
   
private:

   // returns 1 if data are already inserted and 0 otherwise
   bool IsIdDeclaredOnLastLevel( std::string checkId );
   int GetNewLocalAddress();
   int GetNewParamAddress();

   std::list< HashMap > mLocalTable;
   std::list< HashMap > mParamTable;
};