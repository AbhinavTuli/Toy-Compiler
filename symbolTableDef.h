#ifndef SYMBOLTABLEDEF_H
#define SYMBOLTABLEDEF_H

#include "parserDef.h"
#include "lexerDef.h"


#define FUNCTABLESIZE 10 // size of function symbol table
#define VARTABLESIZE 20 // size of variable symbol table

typedef struct variableTableEntry{
	bool isEmpty;
	char key[30];					// the name of the variable
	
	bool isArray; 				// true if array false if not
	bool isArrayStatic;			// true if static array false if not
	int lowerBound, upperBound;	// lower and upper bounds of array (inclusive) - numeric
	char lowerBoundID[20], upperBoundID[20]; // lower and upper bounds of array (inclusive) - numeric

	int tag;					// 1-integer, 2-float, 3-bool
	int width;					// size
	int offset; 				// offset of variable within scope of this table
	int nestingLevel;
	bool isInput;
} variableTableEntry;

typedef struct variableTable {
	variableTableEntry* table;  	// pointer to a dynamically allocated list of size(below) to variableTableEntries
	int size;						//	size of array
	//struct functionTableEntry* funcPointer;
	char funcName[30];
	int scopeStart;
	int scopeEnd;
	int typeOfTable;				// 1-function, 2-conditional, 3-for, 4-while
	int count;						// order number in the list of children
	struct variableTable* parent;
	struct variableTable* child;
	struct variableTable* next;
	bool done;
} variableTable;

typedef struct parameter{
	char key[30];
	int offset;
	int tag;
	bool isArray;
	bool isArrayStatic;
	int lowerBound, upperBound;
	char lowerBoundID[30], upperBoundID[30];
	struct parameter* next;

	bool isRedifined;    	// To be only used when input 
	int width;				// To be only used when input
}parameter;

typedef struct functionTableEntry {
	bool isEmpty;
	bool isDefined;					//  True if was defined before, False if was not defined
	char key[30];                  		// name of function
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