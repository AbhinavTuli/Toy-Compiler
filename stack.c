// C program for linked list implementation of stack 
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 

// A structure to represent a stack 
typedef struct StackNode { 
	int tag; 
    char tnt[NTSIZE];
	struct StackNode* next; 
}lex; 

lex* newNode(int data, char* tnt) 
{ 
	lex* stackNode = (lex*)malloc(sizeof(lex)); 
	stackNode->tag = tag;
    stackNode-> tnt = tnt;
	stackNode->next = NULL; 
	return stackNode; 
} 

int isEmpty(lex* root) 
{ 
	return !root; 
} 

void push(lex** root, int data) 
{ 
	lex* stackNode = newNode(data); 
	stackNode->next = *root; 
	*root = stackNode; 
	printf("%d pushed to stack\n", data); 
} 

int pop(lex** root) 
{ 
	if (isEmpty(*root)) 
		return INT_MIN; 
	struct StackNode* temp = *root; 
	*root = (*root)->next; 
	int popped = temp->data; 
	free(temp); 

	return popped; 
} 

int peek(struct StackNode* root) 
{ 
	if (isEmpty(root)) 
		return INT_MIN; 
	return root->data; 
} 

int main() 
{ 
	struct StackNode* root = NULL; 

	push(&root, 10); 
	push(&root, 20); 
	push(&root, 30); 

	printf("%d popped from stack\n", pop(&root)); 

	printf("Top element is %d\n", peek(root)); 

	return 0; 
} 
