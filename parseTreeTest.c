#include<string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>
//#include "stack.c"
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
    temp->next = NULL;
    temp->tag = 0;
    strcpy(temp->tnt, "");
    return temp;
}


struct ntfirstFollow{
    char nt[NTSIZE];

    char firsts[MAX_FIRST][TSIZE]; // firsts consists only of terminals
    int numFirsts;

    char follows[MAX_FOLLOW][TSIZE]; // follows consists only of terminals
    int numFollows;
};


struct treeNode{
    char tnt[NTSIZE];
    int tag; //0 for nt, 1 for T
    struct treeNode* child;
    struct treeNode* next;
};

struct treeNode* createTreeNode()
{
    struct treeNode* temp;
    temp = (struct treeNode*)malloc(sizeof(struct treeNode));
    temp->next = NULL;
    strcpy(temp->tnt, "");
    temp->child = NULL;
    temp->tag = 0;
    return temp;
}

typedef struct StackNode { 
	int tag; 
    char tnt[NTSIZE];
	struct StackNode* next; 
}lex; 

lex* newNode(int data, char* str) 
{ 
	lex* stackNode = (lex*)malloc(sizeof(lex)); 
	stackNode->tag = data;
    strcpy(stackNode-> tnt,str);
	stackNode->next = NULL; 
	return stackNode; 
} 

int isEmpty(lex* root) 
{ 
	return !root; 
} 

void push(lex** root, int data, char* str) 
{ 
	lex* stackNode = newNode(data,str); 
	stackNode->next = *root; 
	*root = stackNode; 
	printf("%s pushed to stack\n", str); 
} 

int pop(lex** root) 
{ 
	if (isEmpty(*root)) 
		return INT_MIN; 
	lex* temp = *root; 
	*root = (*root)->next; 
	int popped = temp->tag; 
	free(temp); 

	return popped; 
} 

int peek(lex* root) 
{ 
	if (isEmpty(root)) 
		return INT_MIN; 
	return root->tag; 
} 


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



  
/* Function protoypes */
void printGivenLevel(struct treeNode* root, int level); 
int height(struct treeNode* node); 
  
/* Function to print level order traversal a tree*/
void printLevelOrder(struct treeNode* root) 
{ 
    int h = height(root); 
    int i; 
    for (i=1; i<=h; i++) {
        printGivenLevel(root, i); 
            printf("\n"); 
    }

} 
  
/* Print nodes at a given level */
void printGivenLevel(struct treeNode* root, int level) 
{ 
    if (root == NULL) 
        return; 
    if (level == 1) 
        printf("%s ", root->tnt); 
    else if (level > 1) 
    {   
        struct treeNode* temp = root->child;
        while(temp!=NULL){
            printGivenLevel(temp, level-1);
            temp = temp->next;
        } 
        printf("|");
    }
} 
  
/* Compute the "height" of a tree -- the number of 
    nodes along the longest path from the root node 
    down to the farthest leaf node.*/
int height(struct treeNode* node) 
{   
    if (node==NULL) 
        return 0;
    int maxh=1;
    struct treeNode* temp=node;
    struct treeNode* temp2=node;
    if(temp->child!=NULL){
        maxh=1+height(temp->child);
        temp=temp->child;
    }
    /* compute the height of each subtree */
    while(temp2!=NULL){
        temp2=temp2->next;
        if(height(temp2)>maxh){
                maxh=height(temp2);
        }
    } 

    return maxh;
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

token* createToken()
{
    token* temp;
    temp = (token*)malloc(sizeof(token));
    return temp;
}




struct treeNode* findLeftMostWithoutChild(struct treeNode* root)
{

    if(root->child == NULL && root->next==NULL)
    {
        return root;
    }

    struct treeNode* temp = root;
    //struct treeNode* parent;

    struct treeNode* result=NULL;
    if(temp->child != NULL)
    {
        result = findLeftMostWithoutChild(temp->child);
        if(result->tag==1){
            result=NULL;
        }
    }
    while(result == NULL && temp!=NULL ) 
    {
        result = findLeftMostWithoutChild((temp) -> next);
        if(result->tag==1){
            result=NULL;
        }

        temp = temp->next;
    }

    return result;
}

// void printTree(struct treeNode* root)
// {
//     // print("\n");
//     if(root!=NULL)
//     {
//         if(strlen(root->tnt) < 25)
//         {
//             for(int i=0 ; i<25-strlen(root->tnt); i++)
//             {
//                 printf(" ");
//             }
//         }
//         printf("%s",root->tnt);
//     }

//     while(root->next!=NULL)
//     {
//         printTree(root->next);
//     }
// }

struct treeNode* parseInputSourceCode(token* HEAD, int Table[MAX_NON_TERMINALS][MAX_TERMINALS],struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS])
{

    //lexer on testcaseFile
    //use head

    struct treeNode* root = createTreeNode();
    root->next = NULL;

    token* temp;
    temp = HEAD;

    lex* top;
    top = (lex*)malloc(sizeof(lex));
    //push the $ , then start symbol on the stack 
    push(&top, 1, terminals[getIndexOfTerminal("$")]);
    push(&top, 0, nonterminals[0]);
    strcpy(root->tnt, nonterminals[0]);
    root->tag = 0;


    // struct treeNode* tempTreeNode = createTreeNode();
    // root->child = tempTreeNode;
    
    while(temp!= NULL)
    {
        struct treeNode* tempTreeNodeParent = findLeftMostWithoutChild(root);
        struct treeNode* tempTreeNode = createTreeNode();
        tempTreeNodeParent->child=tempTreeNode; 
        // int tag = temp->tag;
        // value val = temp->val;
        term tokterm = temp->tokterm;
        // int lineno = temp->lineno;

        // char lexeme[20];
        //  strcpy(lexeme, GetLexeme(temp->tokterm)) ;
        // int lenflag = strlen(lexeme) + strlen(top->tnt);
        // char flag[lenflag];
        // strcat(lenflag, lexeme);
        // strcat(lenflag, "_");
        // strcat(lenflag, top->tnt);


        //functionNames/ identifiers
        char currLexeme[20];
        strcpy(currLexeme, GetLexeme(tokterm) );

        
        //compare currName with name on the stack
        if( strcmp(currLexeme, top->tnt)==0 )
        {
            //need to pop accoring to
            pop(&top);
            temp=temp->next;
        }
        else
        {
            int i;
            int j;
            if(top->tag == 0)
                i = getIndexOfNonTerminal(top->tnt);
            else
                i = getIndexOfTerminal(top->tnt);
                
            j = getIndexOfTerminal(currLexeme);
            int ruleNo = Table[i][j];

            struct ruleToken head;
            //push gramar[ruleNo] on to the stack
            int count = 0, cumulative=0;
            for(int i=0; i<MAX_NON_TERMINALS; i++)
            {
                
                count+=grammar[i].numRules;
                if(ruleNo < count) //found here
                {
                    
                     head = grammar[i].heads[ruleNo - cumulative];
                }

                cumulative+=grammar[i].numRules;
            }

            pop(&top); 
            //push(&top, head.tag, head.tnt);

            strcpy(tempTreeNode->tnt, head.tnt);
            tempTreeNode->tag = head.tag;
            tempTreeNode->next = NULL;
            tempTreeNode->child = NULL;

            
            struct ruleToken* headNext = head.next;

            struct ruleToken ARRAY[10]; //jisko ulta stack me daalna hai :)
            struct ruleToken ZEROS;
            ZEROS.next = NULL;
            ZEROS.tag = 0;
            strcpy(ZEROS.tnt, "");

            for(int i=0; i<10; i++) //setting every ARRAY[i] to 0
            {
                ARRAY[i] = ZEROS;
            }
            ARRAY[0] = head;
            int index = 0;

            while(headNext!=NULL)
            {
                ARRAY[++index] = *headNext;
                tempTreeNode->next = createTreeNode();
                tempTreeNode = tempTreeNode->next;

                strcpy(tempTreeNode->tnt, headNext->tnt);
                tempTreeNode->tag = headNext->tag;
                tempTreeNode->next = NULL;
                tempTreeNode->child = NULL;   

                
                //push(&top, headNext->tag, headNext->tnt );
                headNext = headNext->next;
            }

            //push ulta
            for(int i= index-1; i>=0; i--)
            {
                push(&top, ARRAY[i].tag, ARRAY[i].tnt );
            }
            




        }
                    
                
    
        





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

    token* headT = createToken();
    headT->tokterm = BO;

    token* temp1 = createToken();
    headT->next = temp1;
    temp1->tokterm = ID;
    
    temp1->next = createToken();
    temp1 = temp1->next;
    temp1->tokterm = PLUS;

    temp1->next = createToken();
    temp1 = temp1->next;
    temp1->tokterm = ID;

    temp1->next = createToken();
    temp1 = temp1->next;
    temp1->tokterm = BC;

    temp1->next = createToken();
    temp1 = temp1->next;
    temp1->tokterm = $;

    temp1->next = NULL;

    


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

    printf("\n\n\n");

    struct treeNode* ROOT = parseInputSourceCode(headT, Table, grammar, firstFollowSets);
    printLevelOrder(ROOT);

    


}

