#ifndef lexerDef.h
#define lexerDef.h
union
	{
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
SQBO, SQBC, BO, BC, COMMENTMARK, ID, NUM, RNUM, $, e, nullpoint = -1
} term;
// take care of nullpoint

typedef struct 
{
	term tokterm; 

	value val;

	//lineno = line number
	int lineno;
    //int df; Shall probably be required later	- df = cnt for constant int/real number

	token *next;
	token *prev;
}  token;

#endif
