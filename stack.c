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

lex* newNode(int data, char* str) 
{ 
	lex* stackNode = (lex*)malloc(sizeof(lex)); 
	stackNode->tag = data;
    stackNode-> tnt = str;
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
	printf("%d pushed to stack\n", data); 
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

int main() 
{ 
	lex* root = NULL; 

	push(&root, 2,"FOR"); 
	push(&root, 1,"NUM"); 
	push(&root, 3, "IN"); 

	printf("%d popped from stack\n", pop(&root)); 

	printf("Top element is %d\n", peek(root)); 

	return 0; 
} 
