#include<string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>
//#include "stack.c"
#include "lexerDef.h"
#include "lexer.c"
#include "parserFF.c"

// #define NTSIZE 	25
// #define TSIZE	25
// #define BUFF_SIZE 150
// #define MAX_NON_TERMINALS 65
// #define MAX_TERMINALS 65
// #define MAX_FIRST 15
// #define MAX_FOLLOW 15

//#include "parser.h"
// char nonterminals[MAX_NON_TERMINALS][NTSIZE];
// char terminals[MAX_TERMINALS][TSIZE];

// struct ruleToken{
//     int tag; // Non-terminal(0) or terminal(1)
//     char tnt[NTSIZE];
//     struct ruleToken* next;
// };

// struct ntRules{
//     char nt[NTSIZE];
//     int numRules;
//     struct ruleToken heads[10];
// };

// int Table[MAX_NON_TERMINALS][MAX_TERMINALS];




struct ruleToken* createNode()
{
    struct ruleToken* temp;
    temp = (struct ruleToken*)malloc(sizeof(struct ruleToken));
    temp->next = NULL;
    temp->tag = 0;
    strcpy(temp->tnt, "");
    return temp;
}


// struct ntfirstFollow{
//     char nt[NTSIZE];

//     char firsts[MAX_FIRST][TSIZE]; // firsts consists only of terminals
//     int numFirsts;

//     char follows[MAX_FOLLOW][TSIZE]; // follows consists only of terminals
//     int numFollows;
// };


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
void printStack(lex* top){
    lex* temp=top;
    while(temp!=NULL){
        printf("%s\n", temp->tnt);
        temp=temp->next;
    }
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


// int getIndexOfTerminal(char *t)
// {
//     //searching in the terminal array
//     for(int i=0; i<MAX_TERMINALS; i++)
//     {
//         if(strcmp(t,terminals[i]) == 0)
//         {
//             return i;
//         }
//     }
// }
// int getIndexOfNonTerminal(char *t)
// {
//     //searching in the terminal array
//     for(int i=0; i<MAX_NON_TERMINALS; i++)
//     {
//         if(strcmp(t,nonterminals[i]) == 0)
//         {
//             return i;
//         }
//     }
// }



  
/* Function protoypes */
void printGivenLevel(struct treeNode* root, int level); 
int height(struct treeNode* node); 
  
/* Function to print level order traversal a tree*/

void inOrder(struct treeNode* root){
    if (root == NULL) 
        return; 
    struct treeNode* temp=root->child;
    // temp=root->child;
    // Total children count 
      
    // Print leftmost child
    inOrder(temp);
    if(temp!=NULL)
        temp=temp->next;
  
    // Print the current node's data 
    printf("%s ",root->tnt); 
  
    // Print all other children
    while(temp!=NULL){
            inOrder(temp);
            temp=temp->next;
    } 
}

// void printLevelOrder(struct treeNode* root) 
// { 
//     //printf("Level order Called\n");
//     int h = height(root); 
//     int i; 
//     for (i=1; i<=h; i++) {
//         printGivenLevel(root, i); 
//             printf("\n"); 
//     }

// } 
  
// /* Print nodes at a given level */
// void printGivenLevel(struct treeNode* root, int level) 
// { 
//     if (root == NULL) 
//         return; 
//     if (level == 1) 
//         printf("%s ", root->tnt); 
//     else if (level > 1) 
//     {   
//         struct treeNode* temp = root->child;
//         while(temp!=NULL){
//             printGivenLevel(temp, level-1);
//             temp = temp->next;
//         } 
//         printf("|");
//     }
// } 
  
// /* Compute the "height" of a tree -- the number of 
//     nodes along the longest path from the root node 
//     down to the farthest leaf node.*/
// int height(struct treeNode* node) 
// {   
//     if (node==NULL) 
//         return 0;
//     int maxh=1;
//     struct treeNode* temp=node;
//     struct treeNode* temp2=node;
//     if(temp->child!=NULL){
//         maxh=1+height(temp->child);
//         //temp=temp->child;
//     }
//     /* compute the height of each subtree */
//     while(temp2!=NULL){
//         temp2=temp2->next;
//         int t=height(temp2);
//         if(t>maxh){
//                 maxh=t;
//         }
//     } 

//     return maxh;
// } 

// void createParseTable(struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS], struct ntRules grammar[MAX_TERMINALS]) //, int[][] Table)
// {
//     for(int i=0; i<MAX_NON_TERMINALS; i++)
//     {
//         for(int j=0; j<MAX_TERMINALS; j++)
//         {
//             Table[i][j] = -1; //default
//         }
//     }

//     //populating the table now
    
//     int whichRule = 0;
    
//     for(int i=0; i<MAX_NON_TERMINALS; i++)       //for each production
//     {
//         //rule from NT n
        
        
//         struct ntRules rule = grammar[i];
        
//         char heads[rule.numRules][NTSIZE];     //rule for each NT can have multiple heads too
//         int tag[rule.numRules];                            //whether terminal or non terminal
        
//         for(int k=0; k<rule.numRules; k++)                  //update the names of the heads and their tags 
//         {
//             strcpy(heads[k], rule.heads[k].tnt);
//             tag[k] = rule.heads[k].tag; 
//         }
        
//         char nonT[NTSIZE];
//         strcpy(nonT,rule.nt);

//         //for every rule n -> alpha
//         //head is alpha in every rule 
//         for(int k=0; k<rule.numRules; k++)
//         {
//             int indexNT = getIndexOfNonTerminal(nonT);
//             int headIndex;

//             //DEBUG stmt printf("\n%d\n",whichRule+k);


//             if(tag[k] == 0)              //non terminal
//             {
//                 headIndex = getIndexOfNonTerminal(heads[k]);
                
//                 //for every terminal in first(alpha)
//                 for(int j=0; j<firstFollowSets[headIndex].numFirsts; j++)
//                 {
//                     int indexT = getIndexOfTerminal(firstFollowSets[headIndex].firsts[j]);
//                     Table[indexNT][indexT] = whichRule + k;

                    
//                 }
//             }
//             else                          //terminal
//             {
//                 if(strcmp(heads[k],"ε")==0) //if the terminal is ε
//                 {
//                     //for each b (terminal) in follow(n)
//                     for(int j=0; j<firstFollowSets[headIndex].numFollows; j++)
//                     {
//                         int indexT = getIndexOfTerminal(firstFollowSets[headIndex].follows[j]);
//                         Table[indexNT][indexT] = whichRule + k;
//                     }


//                 }
//                 else
//                 {
//                     headIndex = getIndexOfTerminal(heads[k]);
//                     Table[indexNT][headIndex] = whichRule + k;        //only one terminal in FIRST() because it itself is a terminal
//                 }     
//             } 
            
//         }

//         whichRule += rule.numRules;
//     }
//  }

token* createToken()
{
    token* temp;
    temp = (token*)malloc(sizeof(token));
    return temp;
}




struct treeNode* findLeftMostWithoutChild(struct treeNode* root)
{
    if(root==NULL){
        return NULL;
    }
    if(root->child==NULL && root->tag==0){
        return root;
    }
    // if(root->child == NULL && root->next==NULL)
    // {
    //     return root;
    // }

    struct treeNode* temp = root->child;
    //struct treeNode* parent;

    struct treeNode* result=NULL;
    if(temp != NULL)
    {
        result = findLeftMostWithoutChild(temp);
        if(result!=NULL && result->tag==1){
            result=NULL;
        }
    }

    while(result == NULL && temp!=NULL ) 
    {
        result = findLeftMostWithoutChild(temp -> next);
        if(result!=NULL && result->tag==1){
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

void parseInputSourceCode(token* HEAD, int Table[MAX_NON_TERMINALS][MAX_TERMINALS],struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS])
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
        printf("\nStack is---------\n");
        printStack(top);
        struct treeNode* tempTreeNode;
        struct treeNode* tempTreeNodeParent = findLeftMostWithoutChild(root);
        if(tempTreeNodeParent==NULL){ //parse tree complete
            printf("Popping $\n");
            pop(&top); //popping $
            // printLevelOrder(root);
            // exit(0);
            inOrder(root);
            //return root;
            //break;
            return;
        }
        if(strcmp(tempTreeNodeParent->tnt,"")!=0){
            tempTreeNode = createTreeNode();
            tempTreeNodeParent->child=tempTreeNode; 
        }
        else{
            tempTreeNode=tempTreeNodeParent;
        }
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
        strcpy(currLexeme, getLexeme(tokterm) );
        printf("Currently at %s\n",currLexeme);
        //printf("debug\n");
        //compare currName with name on the stack
        if( strcmp(currLexeme, top->tnt)==0 )
        {
            //need to pop accoring to
            printf("Popping on match %s\n",top->tnt);
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
            if(ruleNo==-1){
                printf("Error! at reading %s, stack is %s %d\n",currLexeme,top->tnt,temp->lineno);
                exit(-1);
            }

            struct ruleToken head;
            //push gramar[ruleNo] on to the stack
            int count = 0, cumulative=0;
            for(int i=0; i<MAX_NON_TERMINALS; i++)
            {
                
                count+=grammar[i].numRules;
                if(ruleNo < count) //found here
                {
                     head = grammar[i].heads[ruleNo - cumulative];
                     //printf("Yo %d\n",ruleNo-cumulative);
                     break;
                }

                cumulative+=grammar[i].numRules;
            }
            printf("Popping on applying rule %d on stack top %s\n",ruleNo ,top->tnt);
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
            for(int i= index; i>=0; i--)
            {   
                if(strcmp(ARRAY[i].tnt,"ε")){
                    push(&top, ARRAY[i].tag, ARRAY[i].tnt );
                }
            }
            
        }
    }
    //return root;

}

int main()
{
    
    programFile=fopen("working1.txt","rb");
    programFile=getStream(programFile);
    
    populateKeywordTable();

    while(true)
    {
        getNextToken();
        //trav = currentToken;

        if(currentToken->tokterm == 60)
        {
            //printf("%d  %s\n",trav->tokterm,trav->val.s);
            break;
        }
        // if(trav->tag == 1)
        //     printf("Term - %s, Lexeme - %d, On line number  %d\n",getLexeme(trav->tokterm),trav->val.i,trav->lineno);
        // if(trav->tag == 2)
        //     printf("Term - %s, Lexeme - %f, On line number  %d\n",getLexeme(trav->tokterm),trav->val.f,trav->lineno);
        // if(trav->tag == 3)
        //     printf("Term - %s, Lexeme - %d, On line number  %d\n",getLexeme(trav->tokterm),trav->val.b,trav->lineno);
        // if(trav->tag == 4)
        //     printf("Term - %s, Lexeme - %s, On line number  %d\n",getLexeme(trav->tokterm),trav->val.s,trav->lineno);
        //trav = trav->next;
    }
    
    runParser();
    printf("Parser Complete\n");
    parseInputSourceCode(head->next, Table, grammar, firstFollowSets);
    //printLevelOrder(ROOT);
    // printf("Height is %d",height(ROOT));

}

