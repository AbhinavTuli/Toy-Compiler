#include<string.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define NTSIZE 	10
#define TSIZE	10
#define BUFF_SIZE 150
#define MAX_NON_TERMINALS 2
#define MAX_TERMINALS 5
#define MAX_FIRST 5
#define MAX_FOLLOW 5



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
  
/* Helper function that allocates a new node with the 
   given data and NULL left and right pointers. */

void main()
{   
//     struct treeNode{
//     char tnt[NTSIZE];
//     int tag; //0 for nt, 1 for T
//     struct treeNode* child;
//     struct treeNode* next;
// };


    struct treeNode* root = createTreeNode();
    

    struct treeNode* B,*C,*D,*E,*F,*G,*H,*I;
    B = createTreeNode();
    C = createTreeNode();
    D = createTreeNode();
    E = createTreeNode();
    F = createTreeNode();
    G = createTreeNode();
    H = createTreeNode();
    I = createTreeNode();
    
    strcpy(root->tnt, "A");
    root->tag = 0;
    root->next = NULL;
    root->child = B;

    B->tag = 0;
    B->next = D;
    B->child = F;
    strcpy(B->tnt, "B");

    C->tag = 0;
    C->next = E;
    C->child = NULL;
    strcpy(C->tnt, "C");

    D->tag = 0;
    D->next = C;
    D->child = NULL;
    strcpy(D->tnt, "D");

    E->tag = 0;
    E->next = NULL;
    E->child = G;
    strcpy(E->tnt, "E");

    F->tag = 0;
    F->next = NULL;
    F->child = NULL;
    strcpy(F->tnt, "F");

    G->tag = 0;
    G->next = NULL;
    G->child = H;
    strcpy(G->tnt, "G");

    H->tag = 0;
    H->next = NULL;
    H->child = NULL;
    strcpy(H->tnt, "H");
    
    printLevelOrder(root);
    //printGivenLevel(root,4);
    printf("%d",height(root));





}   


