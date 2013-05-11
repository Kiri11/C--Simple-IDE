#include "HashTable.h"

bool HashTable::insert(string string_for_inserting){
	if (check(string_for_inserting)){
		return 0;
	}
	else{
		set.insert(string_for_inserting);
		return 1;
	}
}

bool HashTable::check(string string_for_check){
	return set.count(string_for_check);
}