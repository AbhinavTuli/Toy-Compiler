/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
    Rohit Bohra             -   2017A7PS0225P
*/

#ifndef LEXER_H
#define LEXER_H

 //functions
#include"lexerDef.h"

int hash(char * str);
void insertEntry(char* c,term te);
int findEntry(char* c);
struct node* retNode(char *c);
void printHashtable();
void initializeHashtable();
const char* getLexeme(term t);
void populateKeywordTable();
FILE *getStream(FILE *fp);
token* retTokenINT(int value,term t);
token* retTokenREAL(float value,term t);
token* retTokenBOOL(bool value,term t);
token* retTokenSTR(char value[],term t);
token* checkASSIGNOP();
token* checkEQ();
token* checkGT();
token* checkLT();
token* checkRANGEOP();
int isAlpha(char c);
int isDigit(char c);
void removeComments(FILE* fptr);
void getNextToken();
token* lexerRun(FILE* fptr);
void lexerFree();

#endif