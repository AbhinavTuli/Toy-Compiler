#include "ast.h"

void printGivenLevelExp(struct astNode* root, int level); 
int heightExp(struct astNode* node);

void printLevelOrderExp(struct astNode* root) 
{ 
    int h = heightExp(root); 
    int i; 
    for (i=1; i<=h; i++) {
        printf("Level %d: ", i);
        printGivenLevelExp(root, i); 
        printf("\n");
    }
} 
  
/* Print nodes at a given level */
void printGivenLevelExp(struct astNode* root, int level) 
{ 
    if (root == NULL) 
        return; 
    if (level == 1){
         if(root->tag==1){
            printf("%d ",root->val.i);
         }
         else if(root->tag==4){
            printf("%s ", root->val.s); 
         }
         else{
            printf("%s ", root->name); 
         }
    }
    else if (level > 1) 
    { 
        printGivenLevelExp(root->left, level-1); 
        printGivenLevelExp(root->right, level-1); 
    } 
} 
  
/* Compute the "height" of a tree -- the number of 
    nodes along the longest path from the root node 
    down to the farthest leaf node.*/
int heightExp(struct astNode* node) 
{ 
    if (node==NULL) 
        return 0; 
    else
    { 
        /* compute the height of each subtree */
        int lheight = heightExp(node->left); 
        int rheight = heightExp(node->right); 
  
        /* use the larger one */
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
} 