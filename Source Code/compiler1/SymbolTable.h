#pragma once
#include <string>
#include <list>
#include <unordered_map>
using std::string;
using std::list;
using std::unordered_map;
using std::pair;
typedef unordered_map<string, int> HashMap;

// возвращать наружу Identifier или kind/address по отдельности?
class SymbolTable{
public:
//	SymbolTable();
	void createLevel();
	void deleteLevel();
	// return address
	int insert(string idName, bool isIdParameter);
	// if not found, returns -1
	int getIdentifierAddress(string idName);
	int getLevel();
	
private:
	list<HashMap> localTable;
	list<HashMap> paramTable;
	// returns 1 if data are already inserted and 0 otherwise
	bool isIdDeclaredOnLastLevel(string checkId);
	int getNewLocalAddress();
	int getNewParamAddress();
};