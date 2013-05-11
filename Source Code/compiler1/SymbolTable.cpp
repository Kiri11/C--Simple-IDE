#include "SymbolTable.h"

//SymbolTable::SymbolTable(){
	//createLevel(); // ?
//}

void SymbolTable::createLevel(){
	HashMap newLevel;
	localTable.push_front(newLevel);
	paramTable.push_front(newLevel);
}

void SymbolTable::deleteLevel(){
	localTable.pop_front();
	paramTable.pop_front();
}

int SymbolTable::insert(string idName, bool isIdParameter){
	// check last level
	// if not exist, insert
	// else throw exception
	if (getLevel() == 0) throw (string) "Error: trying to insert into empty symbol table.";

	if (isIdDeclaredOnLastLevel(idName) == 0){
		int newAddress;
		if (isIdParameter){
			newAddress = getNewParamAddress();
			paramTable.front().insert(pair<string, int>(idName, newAddress));
		}
		else{
			newAddress = getNewLocalAddress();
			localTable.front().insert(pair<string, int>(idName, newAddress));
		}
		return newAddress;
	}
	else{
		string errorStr = "Semantic error: redefinition of identifier " + idName ;
		throw errorStr;
	}
}

bool SymbolTable::isIdDeclaredOnLastLevel(string checkIdName){
	return (localTable.front().count(checkIdName) || paramTable.front().count(checkIdName));
}

int SymbolTable::getNewParamAddress(){
	int size = paramTable.front().size();
	int address = 4 * (size + 2);
	return address;
}

int SymbolTable::getNewLocalAddress(){
	int size = localTable.front().size();
	int address = -4 * (size + 1);
	return address;
}

int SymbolTable::getIdentifierAddress(string idName){
	if (getLevel() > 0){

		list<HashMap>::iterator itLocal = localTable.begin();
		list<HashMap>::iterator itParam = paramTable.begin();
		while (itLocal != localTable.end()){
			// если нашли на этом уровне
			if (itLocal->count(idName)) return itLocal->at(idName);
			if (itParam->count(idName)) return itParam->at(idName);
			++itLocal;
			++itParam;
		}	
	}
	return -1;
}

int SymbolTable::getLevel(){
	return localTable.size();
}