//#ifndef lexerDef.h
//#define lexerDef.h
typedef union
	{
		int i;			//constant integer value
		float f;		//constant real value
        bool b;
		char s[25];		//function names, id names
	} value;

typedef enum 
{
INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER,   //10 
DRIVERDEF,DRIVERENDDEF, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT,  //8 
USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR,  //11
IN, SWITCH, CASE, BREAK, DEFAULT, WHILE, PLUS, MINUS, MUL, DIV, LT, LE,  //12
GE, GT, EQ, NE, DEF, ENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, //11
SQBO, SQBC, BO, BC, COMMENTMARK, ID, NUM, RNUM, $, e, HEAD //11
} term;


typedef struct node1 
{
	term tokterm; 

	value val;
	int tag;

	struct node1* prev;
	struct node1* next;

	int lineno; 
}  token;

struct node{
    char value[30];
    struct node* next;
    term tt;
    //token* t; //token
};
//#endif