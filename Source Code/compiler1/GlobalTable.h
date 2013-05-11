#pragma once
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;
using std::pair;
typedef unordered_map<string, int> GlobalMap;


// Нужно ли знать, функия или переменная? думаю да
class GlobalTable{
public:
	// return true if inserted, and false otherwise (already have this variable)
	void insert(string idName, int paramCount);
	// Return number of params, -1 if not function, -2 if not found
	int check(string idName);
private:
	GlobalMap globals;
};