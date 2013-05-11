#include "GlobalTable.h"

int GlobalTable::check(string idName){
	if (globals.count(idName)){
		return globals.at(idName);
	}
	else{
		// not found
		return -2;
	}
}

void GlobalTable::insert(string idName, int isFunction){
	if (globals.count(idName) == 0){ 
		globals.insert(pair<string, int>(idName, isFunction));
	}
	else {
		string errorStr = "Semantic Error: Redifinition of global variable " + idName;
		throw errorStr;
	}
}