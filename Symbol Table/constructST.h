#include "ast.h"
#include "table_id.h"
#include "table_rec.h"
#include "table_func.h"

typedef struct variable_Entry
{
    char map[20];                                          // key in the hash table will be mapped to the name
    char dtype[20];                                         // data type of variable 
    int check;                                              // check if given data type is an array
    int range[2];
    int size; 
    int offset; 
}var_Entry;

typedef struct function_Entry
{
    char map[20];
    char ret_type[20];
    
}func_Entry;



typedef struct globalTable{
    TableEntry *head; 
}GT;

typedef struct table_Entry{
    char map[20];
    bool check;                                             // check for variabl/function 
    entry 
}tableEntry;

typedef union entry { 
    variable_Entry var; 
    function_Entry func;
}entry; 


typedef struct function_table{


}function_table;
















typedef struct _globalVar {
	char* id;
	char* type;
	int offset;
	struct _globalVar *next;
}globalVar;

// void varLocal(astNode *f);
void typeExtractionLocalVars(astNode *f);
/*fills local ID table of passed function
*/

void typeExtractionGlobalVars(astNode *ast);
/*fills global ID table
ERRORS detected
1) same global variable being re-declared
ignores everything else (some local variable having same name as a global will be caught in typeExtractionID)
*/

// void typeExtractionRec(astNode *ast);
/*extracts record info and fills record table
ERRORS detected
1) record re-defintion
2) field of same name redeclared within a record
*/

void typeExtractionFunc(astNode *f);
/* extracts types for functions and makes entry in globally defined function table (input and output parameters)
also inserts input and otput pars into local ID table
ERRORS detected:
1) function re-declaration
2) input or output parameter has same name as a global variable
*/

// void printExtractedRecs();
void printGlobalVarList();
void printFuncs(astNode * root);
#endif