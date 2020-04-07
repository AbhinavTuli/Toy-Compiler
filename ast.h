#ifndef AST_H
#define AST_H

#include "lexerDef.h"

struct astNode{
    char name[50];
    char type[50];

    int tag;
    value val;
    
    struct astNode* child;
    struct astNode* next;
} astNode;

#endif

