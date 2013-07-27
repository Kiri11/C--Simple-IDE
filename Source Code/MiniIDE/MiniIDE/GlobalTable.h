#pragma once
#include <string>
#include <unordered_map>

typedef std::unordered_map< std::string, int > GlobalMap;


// Нужно ли знать, функия или переменная? думаю да
class GlobalTable
{
public:

	// return true if inserted, and false otherwise (already have this variable)
	void Insert( std::string idName, int paramCount );
	// Return number of params, -1 if not function, -2 if not found
	int Check( std::string idName );
private:
	GlobalMap mGlobals;
};