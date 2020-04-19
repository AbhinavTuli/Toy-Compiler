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

