#pragma once
#include <string>
#include <unordered_set>

class HashTable
{
public:
	// returns 1 if data inserted and 0 if data are already there
	bool Insert( std::string string_for_inserting );
	// returns 1 if data are already inserted and 0 otherwise
	bool Check( std::string string_for_check );
	
private:
	std::unordered_set< std::string > mSet;
};