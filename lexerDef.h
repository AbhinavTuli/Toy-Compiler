// #ifndef lexerDef.h
// #define lexerDef.h
#include <stdbool.h>

typedef union {
	int i;			//constant integer value
	float f;		//constant real value
	bool b;
	char s[25];		//function names, id names
} value;

typedef enum 
{
INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, 	
DRIVERDEF,DRIVERENDDEF, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT, 
USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, 
IN, SWITCH, CASE, BREAK, DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE, 
GE, GT, EQ, NE, DEF, ENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, 
SQBO, SQBC, BO, BC, COMMENTMARK, ID, NUM, RNUM, $, e, HEAD
} term;
// take care of nullpoint

typedef struct TempToken
{
	term tokterm;

	struct TempToken* prev;
	struct TempToken* next;
	
	value val;
	int tag;

	int lineno;
}token;

// #endif