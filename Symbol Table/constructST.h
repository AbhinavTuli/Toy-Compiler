#include "ast.h"
#include "table_id.h"
#include "table_rec.h"
#include "table_func.h"

typedef struct _globalVar {
	char* id;
	char* type;
	int offset;
	struct _globalVar *next;
}globalVar;

void typeExtractionLocalVars(astNode *f);
/*fills local ID table of passed function
*/

void typeExtractionGlobalVars(astNode *ast);
/*fills global ID table
ERRORS detected
1) same global variable being re-declared
ignores everything else (some local variable having same name as a global will be caught in typeExtractionID)
*/

void typeExtractionRec(astNode *ast);
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

void printExtractedRecs();
void printGlobalVarList();
void printFuncs(astNode * root);
#endif