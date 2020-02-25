/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
    Rohit Bohra             -   2017A7PS0225P
*/

#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"
#include "lexerDef.h"

// Reading Grammar from grammar.txt and storing it in our array of structures "grammar"

void addRuleToGrammer(char* rule);

// First Functions

void computeFirstAndFollow(); // Main Function for FirstFollow

void computeFirstAllSets();

void computeFirstForSingleNT(int indexNT);

void computeFirstRecursive(int indexNT,int ruleNum);

void computeFirstRecursive2(struct ruleToken* CurrentRuleHead,struct ruleToken* toAddRuleHead);

void addFirsts(int i);

//  Follow Functions

void computeFollowAllSets();

void computeFollowForSingleNT(int indexNT);

void computeFollowRecursive(char* strNT,int lineNo,int ruleNo);

void computeFollowRecursive2(struct ruleToken* rulePointer,struct ntRules* startToken);

void getFirstsForFollows(int indexNT,struct ruleToken* rulePointer,struct ntRules* startToken);

void addFollows(int i);


// Miscellaneous

bool isEpsilon(char* str);

int getIndexOfTerminal(char *bufferToken);

int getIndexOfNonTerminal(char *bufferToken);

int findRHSPositions(char* str,int* rhsNT,int* rhsNTRulePos);

bool checkIfFollowAlreadyPresent(char* str);

bool checkIfFirstAlreadyPresent(char* str);

void computeFirstSets2();

bool checkIfPresentInStack(int indexNT);

void readGrammerTextFile(FILE* fp);

// Print Functions

void printAllGrammar();

void printRuleGrammarStruct(int i);

void printAllTerminals();

void printAllNonTerminals();

void printAllFirstSets();

void printAllFollowSets();

void printParseTable();

// Creating Parse Table from First And Follow
void createParseTable();

void addDollarToParseTable();

// Parsing Input from Lexer to Parser to check for syntax verification
void parseInputSourceCode(token* HEAD, int Table[MAX_NON_TERMINALS][MAX_TERMINALS],struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS],FILE* parseTreeFile);

struct treeNode* findLeftMostWithoutChild(struct treeNode* root);

// Printing InOrder Traversal of Parse Tree
void inOrderParseTree(struct treeNode* root,FILE* parseTreeFile);

token* createToken();

// Stack Functions
void printStack(lex* top);

void push(lex** root, int data, char* str);

int pop(lex** root);

int isEmpty(lex* root);

int peek(lex* root);


#endif