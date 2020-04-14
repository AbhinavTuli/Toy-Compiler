#ifndef SYMBOLTABLEDEF_H
#define SYMBOLTABLEDEF_H

#include "parserDef.h"
#include "lexerDef.h"


#define FUNCTABLESIZE 10 // size of function symbol table
#define VARTABLESIZE 20 // size of variable symbol table

typedef struct variableTableEntry{
	bool isEmpty;
	char *key;					// the name of the variable
	
	bool isArray; 				// true if array false if not
	bool isArrayStatic;			// true if static array false if not
	bool rangeVariables;		// true if range variables false if number lexemes
	int lowerBound, upperBound;	// lower and upper bounds of array (inclusive)

	int tag;					// 1-integer, 2-float, 3-bool
	int width;					// size
	int offset; 				// offset of variable within scope of this table
	int nestingLevel;
	

} variableTableEntry;

typedef struct variableTable {
	variableTableEntry* table;  	// pointer to a dynamically allocated list of size(below) to variableTableEntries
	int size;						//	size of array
	
	int count;						// order number in the list of children
	struct variableTable* parent;
	struct variableTable* child;
	struct variableTable* next;
} variableTable;

typedef struct parameter{
	bool isArray;
	int tag;
	int lowerBound, upperBound;
	struct parameter* next;
}parameter;

typedef struct functionTableEntry {
	bool isEmpty;
	char *key;                  		// name of function
	parameter *inputList, *outputList;  // pointers to heads of lists of input and output parameters, NULL if empty
	int numInput, numOutput;			// number of input parameters, number of output lists

	int frameSize;              		// size of input and output pars
	variableTable* localVarTable;       // local variable table

} functionTableEntry;

typedef struct functionTable {
	functionTableEntry* table;
	int size;
}functionTable;

#endif