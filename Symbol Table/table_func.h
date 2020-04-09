// Header file containing all Function Table modules 
// Eqiuivalent modules in table_id and table_rec

#include "table_entries.h"

// Undefined structures : linkedListFunc, FunctionTableEntry, 


typedef struct _functionTable {
	linkedListFunc *bins;											// Purpose??? linkedlist of functions? or nesting? 
	int size;          	 											// number of bins - number of functions? 
	int total;          											// total number of elements
} functionTable;

//hash function in hash_table.c

functionTable createFT(int size);

functionTable destroyFT(functionTable ft);

functionTableEntry * searchInFT(functionTable ft, char *key);

functionTable insertInFT(functionTable ft, functionTableEntry e);

void printFT(functionTable ft);