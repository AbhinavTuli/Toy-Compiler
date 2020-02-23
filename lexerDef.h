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

const char* GetLexeme(term t) 
{
   switch (t) 
   {
      case INTEGER: return "INTEGER"; 
      case REAL: return "REAL"; 
      case BOOLEAN : return "BOOLEAN"; 
      case OF : return "OF"; 
      case ARRAY : return "ARRAY"; 
      case START : return "START"; 
      case END : return "END"; 
      case DECLARE : return "DECLARE"; 
      case MODULE : return "MODULE";
      case DRIVER : return "DRIVER";
      case DRIVERDEF : return "DRIVERDEF";
      case DRIVERENDDEF : return "DRIVERENDDEF";
      case PROGRAM : return "PROGRAM";
      case TAGGED : return "TAGGED";
      case UNION : return "UNION";
      case GET_VALUE : return "GET_VALUE";
      case PRINT : return "PRINT";
      case USE : return "USE";
      case WITH : return "WITH";
      case PARAMETERS : return "PARAMETERS";
      case TRUE : return "TRUE";
      case FALSE :return "FALSE" ;
      case TAKES : return "TAKES";
      case INPUT : return "INPUT";
      case RETURNS : return "RETURNS";
      case AND : return "AND" ;
      case OR : return "OR";
      case FOR : return "FOR";
      case IN : return "IN" ; 
      case SWITCH : return "SWITCH";
      case CASE : return "CASE";
      case BREAK: return "BREAK";
      case DEFAULT : return "DEFAULT";
      case WHILE : return "WHILE";
      case PLUS : return "PLUS";
      case MINUS : return "MINUS";
      case MUL : return "MUL";
      case DIV : return "DIV";
      case LT : return "LT";
      case LE : return "LE";
      case GE : return "GE";
      case GT : return "GT";
      case EQ : return "EQ";
      case NE : return "NE";
      case DEF :  return "DEF";
      case ENDDEF :return "ENDDEF";
      case COLON : return "COLON";
      case RANGEOP : return "RANGEOP";
      case SEMICOL : return "SEMICOL";
      case COMMA : return "COMMA";
      case ASSIGNOP : return "ASSIGNOP";
      case SQBO : return "SQBO";
      case SQBC : return "SQBC";
      case BO : return "BO";
      case BC : return "BC";
      case COMMENTMARK : return "COMMENTMARK";
      case ID : return "ID";
      case NUM : return "NUM";
      case RNUM : return "RNUM";
      case $ : return "$";
      case e: return "ε";
      case HEAD : return "HEAD";
      default : return "\0";
   }
}

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