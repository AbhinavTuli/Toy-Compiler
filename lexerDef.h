/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
    Rohit Bohra             -   2017A7PS0225P
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include <stdbool.h>
#include <math.h>
#include <errno.h>

#define BUFFER_MAX 100
#define MAX_TABLE 300
#define MAX_IDENTIFIER_LENGTH 20

#ifndef LEXERDEF_H
#define LEXERDEF_H

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
    char value[50];
    struct node* next;
    term tt;
    //token* t; //token
};

FILE* programFile;

int buffPtr=0;

int lineNumber=1;
char buff[BUFFER_MAX];

bool error = false;
bool first = true;

token* currentToken;
token* prevToken = NULL;
token* head; 

bool endfile = false;

struct node* keywords[MAX_TABLE];

int lexicalErrors[20];
int numLexicalErrors;

bool printFlag = true;

#endif