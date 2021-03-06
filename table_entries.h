/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/

#include "lexerDef.h"

// for functions : 
typedef struct _functionTableEntry {
	char *key;                  	//
	token id;                 	  	// defined in lexer ?
	typeInfo *inputPars, *outputPars;
	int frameSize;              	//total sizes of all local variables+parameters
	int numInput, numOutput;    	// number of input and output pars
	idTable lidt;               	//local ID table
} functionTableEntry;


// for identifiers : 
typedef struct _idTableEntry {
	char *key;
	token id;
	gterm dataType;
	int size;
	int offset; //offset of variable within scope of this table
	recordTableEntry *rptr;
} idTableEntry;


// for records : 
typedef struct _typeInfo {
	gterm dataType;
	char* recordID; //in case dataType is TK_RECORDID
	token id;
	int size;
	struct _typeInfo *next;
} typeInfo;

typedef struct _recordTableEntry {
	char *key; //record ID
	token id;
	typeInfo *fields; //list of struct fields
	int size;
} recordTableEntry;