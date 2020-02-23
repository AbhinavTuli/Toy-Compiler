#include<string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stack.c>
#include "lexerDef.h"
#define NTSIZE 	10
#define TSIZE	10
#define BUFF_SIZE 150
#define MAX_NON_TERMINALS 2
#define MAX_TERMINALS 5
#define MAX_FIRST 5
#define MAX_FOLLOW 5

//#include "parser.h"
char nonterminals[MAX_NON_TERMINALS][NTSIZE];
char terminals[MAX_TERMINALS][TSIZE];

struct ruleToken{
    int tag; // Non-terminal(0) or terminal(1)
    char tnt[NTSIZE];
    struct ruleToken* next;
};

struct ntRules{
    char nt[NTSIZE];
    int numRules;
    struct ruleToken heads[10];
};

int Table[MAX_NON_TERMINALS][MAX_TERMINALS];



struct ruleToken* createNode()
{
    struct ruleToken* temp;
    temp = (struct ruleToken*)malloc(sizeof(struct ruleToken));
    return temp;
}


struct ntfirstFollow{
    char nt[NTSIZE];

    char firsts[MAX_FIRST][TSIZE]; // firsts consists only of terminals
    int numFirsts;

    char follows[MAX_FOLLOW][TSIZE]; // follows consists only of terminals
    int numFollows;
};


int getIndexOfTerminal(char *t)
{
    //searching in the terminal array
    for(int i=0; i<MAX_TERMINALS; i++)
    {
        if(strcmp(t,terminals[i]) == 0)
        {
            return i;
        }
    }
}
int getIndexOfNonTerminal(char *t)
{
    //searching in the terminal array
    for(int i=0; i<MAX_NON_TERMINALS; i++)
    {
        if(strcmp(t,nonterminals[i]) == 0)
        {
            return i;
        }
    }
}

void createParseTable(struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS], struct ntRules grammar[MAX_TERMINALS]) //, int[][] Table)
{
    for(int i=0; i<MAX_NON_TERMINALS; i++)
    {
        for(int j=0; j<MAX_TERMINALS; j++)
        {
            Table[i][j] = -1; //default
        }
    }

    //populating the table now
    
    int whichRule = 0;
    
    for(int i=0; i<MAX_NON_TERMINALS; i++)       //for each production
    {
        //rule from NT n
        
        
        struct ntRules rule = grammar[i];
        
        char heads[rule.numRules][NTSIZE];     //rule for each NT can have multiple heads too
        int tag[rule.numRules];                            //whether terminal or non terminal
        
        for(int k=0; k<rule.numRules; k++)                  //update the names of the heads and their tags 
        {
            strcpy(heads[k], rule.heads[k].tnt);
            tag[k] = rule.heads[k].tag; 
        }
        
        char nonT[NTSIZE];
        strcpy(nonT,rule.nt);

        //for every rule n -> alpha
        //head is alpha in every rule 
        for(int k=0; k<rule.numRules; k++)
        {
            int indexNT = getIndexOfNonTerminal(nonT);
            int headIndex;

            //DEBUG stmt printf("\n%d\n",whichRule+k);


            if(tag[k] == 0)              //non terminal
            {
                headIndex = getIndexOfNonTerminal(heads[k]);
                
                //for every terminal in first(alpha)
                for(int j=0; j<firstFollowSets[headIndex].numFirsts; j++)
                {
                    int indexT = getIndexOfTerminal(firstFollowSets[headIndex].firsts[j]);
                    Table[indexNT][indexT] = whichRule + k;

                    
                }
            }
            else                          //terminal
            {
                if(strcmp(heads[k],"ε")==0) //if the terminal is ε
                {
                    //for each b (terminal) in follow(n)
                    for(int j=0; j<firstFollowSets[headIndex].numFollows; j++)
                    {
                        int indexT = getIndexOfTerminal(firstFollowSets[headIndex].follows[j]);
                        Table[indexNT][indexT] = whichRule + k;
                    }


                }
                else
                {
                    headIndex = getIndexOfTerminal(heads[k]);
                    Table[indexNT][headIndex] = whichRule + k;        //only one terminal in FIRST() because it itself is a terminal
                }     
            } 
            
        }

        whichRule += rule.numRules;
    }
 }


void main()
{
    //setting the nonterminals and terminals
    strcpy(nonterminals[0],"S");
    strcpy(nonterminals[1], "F");


    strcpy(terminals[0],"(");
    strcpy(terminals[1],"+");
    strcpy(terminals[2],")");
    strcpy(terminals[3],"a");
    strcpy(terminals[4],"ε");

    //setting up grammar rules 
    struct ntRules grammar[MAX_NON_TERMINALS];
    // int grammarLength = 0;

    strcpy(grammar[0].nt, "S");
    grammar[0].numRules = 2;
    grammar[0].heads[0].tag = 0;
    strcpy(grammar[0].heads[0].tnt, "F");

    struct ruleToken* temp,*node;
    
    grammar[0].heads[0].next = NULL;

    strcpy(grammar[0].heads[1].tnt, "(");
    grammar[0].heads[1].tag = 1;
    temp = createNode();
    node = temp;
    grammar[0].heads[1].next = node;
    node->tag = 0;
    strcpy(node->tnt, "S");

    temp = createNode();
    node->next = temp;
    node = temp;
    node->tag = 1; //t
    strcpy(node->tnt, "+");

    temp = createNode();
    node->next = temp;
    node = temp;
    node->tag = 0;
    strcpy(node->tnt, "F");
    
    temp = createNode();
    node->next = temp;
    node = temp;
    node->tag = 1;
    strcpy(node->tnt, ")");

    node->next = NULL;

    strcpy(grammar[1].nt, "F");
    grammar[1].numRules = 1;
    grammar[1].heads[0].tag = 1;
    strcpy(grammar[1].heads[0].tnt, "a");
    grammar[1].heads[0].next = NULL;

    //setting up First Follow Sets
    struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS];
    strcpy(firstFollowSets[0].nt, "S");

    strcpy(firstFollowSets[0].firsts[0], "a");
    strcpy(firstFollowSets[0].firsts[1], "(");
    firstFollowSets[0].numFirsts = 2;

    strcpy(firstFollowSets[0].follows[0], "$");
    strcpy(firstFollowSets[0].follows[1], "+");
    firstFollowSets[0].numFollows = 2;

    strcpy(firstFollowSets[1].nt, "F");

    strcpy(firstFollowSets[1].firsts[0], "a");
    firstFollowSets[1].numFirsts = 1;

    strcpy(firstFollowSets[1].follows[0], "$");
    strcpy(firstFollowSets[1].follows[1], "+");
    strcpy(firstFollowSets[1].follows[2], ")");
    firstFollowSets[1].numFollows = 3;




    createParseTable(firstFollowSets, grammar);

    //printing the parse table
    for(int i=0; i<MAX_NON_TERMINALS; i++)
    {
        printf("\n");
        for(int j=0; j<MAX_TERMINALS; j++)
        {
            printf("   %d", Table[i][j]); 

        }
    }


}

struct treeNode{
    char tnt[NTSIZE];
    int tag;
    struct treeNode* child;
    struct treeNode* next;
}

struct treeNode* createTreeNode()
{
    struct treeNode* temp;
    temp = (struct treeNode*)malloc(sizeof(struct treeNode));
    return temp;
}






struct treeNode* parseInputSourceCode(lex* root, char *testcaseFile, int Table[MAX_NON_TERMINALS][MAX_TERMINALS],struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS])
{

    //lexer on testcaseFile
    //use head

    struct treeNode* top = createTreeNode();
    struct treeNode* tempTree = top;

    token* temp;
    temp = head;
    while(temp!= NULL)
    {

        //push the start symbol on the stack 
        push(root, 0, nonterminals[0]);
        int tag = temp->tag;
        value val = temp->val;
        term tokterm = temp->tokterm;
        int lineno = temp->lineno;

        

        switch(tag)
        {
            case 4: //functionNames/ identifiers
                    char currName[NTSIZE];
                    strcpy(currName, val.s);

                    
                    //compare currName with name on the stack
                    if( strcmp(currName, lex->tnt)==0 )
                    {
                        //need to pop accoring to
                    }
                    else
                    {
                        int i;
                        int j;
                        i = getIndexOfNonTerminal(lex->tnt);
                        j = getIndexOfTerminal(currName);
                        int ruleNo = Table[i][j];

                        
                    }
                    
                
        }
        





    }


}
