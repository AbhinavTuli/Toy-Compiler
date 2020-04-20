/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/

#ifndef AST_H
#define AST_H

#include "lexerDef.h"

struct astNode* generateAST(struct treeNode* root);

struct astNode* makeAstNode(char* name, value val,int tag,struct astNode* child);

void printExpression(struct astNode* root);

void printGivenLevel(struct astNode* root, int level);

void printLevelOrder(struct astNode* root);

int runAST(FILE* testFile);

#endif

