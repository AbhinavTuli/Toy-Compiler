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
#include <limits.h>
#include "lexerDef.h"

#ifndef PARSERDEF_H
#define PARSERDEF_H

#define NTSIZE 35
#define TSIZE 30
#define BUFF_SIZE 150
#define MAX_NON_TERMINALS 200
#define MAX_TERMINALS 200
#define MAX_FIRST 15
#define MAX_FOLLOW 15

int numParseNodes=0;

char buffer[BUFF_SIZE];

char tempFirsts[MAX_FIRST][TSIZE]; // Contains Firsts
int tempFirstsSize = 0;

char tempFollows[MAX_FOLLOW][TSIZE]; // Contains Follows
int tempFollowsSize = 0;

int stackFirsts[MAX_FIRST];
int stackFirstsSize = 0;

char nonterminals[MAX_NON_TERMINALS][NTSIZE];
char terminals[MAX_TERMINALS][TSIZE];

int numNT = 0;
int numT = 0;

int currentFirstFollowCompute = 0;

struct ruleToken *CurrentRuleTokenGlobal;
struct ruleToken *prevCurrentRuleTokenGlobal;

struct ruleToken
{
    int tag; // Non-terminal(0) or terminal(1)
    char tnt[NTSIZE];
    struct treeNode *pTreePointer;
    value val;
    struct ruleToken *next;
};

struct ntRules
{
    char nt[NTSIZE];
    int numRules;
    struct ruleToken heads[10];
};

struct ntfirstFollow
{
    char nt[NTSIZE];

    char firsts[MAX_FIRST][TSIZE]; // firsts consists only of terminals
    int numFirsts;

    char follows[MAX_FOLLOW][TSIZE]; // follows consists only of terminals
    int numFollows;
};

struct ntRules grammar[MAX_NON_TERMINALS];
int grammarLength = 0;

struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS];

int Table[MAX_NON_TERMINALS][MAX_TERMINALS]; //stores the rule number

// For Parse Tree and Syntax Verification

struct treeNode
{
    char tnt[NTSIZE];
    int tag; //0 for nt, 1 for T
    struct treeNode *child;
    struct treeNode *next;
    value val;

    int tagUnion;

    int lineno;
};

typedef struct StackNode
{
    int tag;
    char tnt[NTSIZE];
    struct StackNode *next;
    struct treeNode *pTreePointer;
    value val;
    int lineno;
} lex;

lex *newNode(int data, char *str)
{
    lex *stackNode = (lex *)malloc(sizeof(lex));
    stackNode->tag = data;
    strcpy(stackNode->tnt, str);
    stackNode->next = NULL;

    struct treeNode *pTreePointer;
    return stackNode;
}

#endif