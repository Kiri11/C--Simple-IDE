#pragma once
#include <string>
#include <unordered_set>
using std::string;
using std::unordered_set;

class HashTable{
public:
	// returns 1 if data inserted and 0 if data are already there
	bool insert(string string_for_inserting);
	// returns 1 if data are already inserted and 0 otherwise
	bool check(string string_for_check);
	
private:
	unordered_set<string> set;
};