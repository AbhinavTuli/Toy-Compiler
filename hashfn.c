#define MAX_TABLE 200
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "lexerDef.h"
int hash(char * str) {
	int hash = 401;
	while (*str != '\0') {
		hash = ((hash << 4) + (int)(*str)) % MAX_TABLE;
		str++;
	}

	return hash % MAX_TABLE;
}

struct node{
    char value[30];
    struct node* next;
    struct token* t; //token
};

struct node* keywords[MAX_TABLE];
token* createToken(char* keyword, term tt) //token
{
    token* t = (token*) malloc(sizeof(token));

    t->tag = 4;
    strcpy(t->val.s,keyword);
    t->prev = NULL;
    t->next = NULL;

    t->tokterm = tt;
    t->lineno = -1;
}
void insertEntry(char* c, term tt){//term tt
    if(keywords[hash(c)]==NULL){
        keywords[hash(c)]=(struct node*)malloc(sizeof(struct node));
        strcpy(keywords[hash(c)]->value,c);
        keywords[hash(c)]->next=NULL;
        keywords[hash(c)]->t=createToken(c,tt); //token
    }
    else{
        struct node* t;
        t=keywords[hash(c)];
        while(t->next!=NULL){
            t=t->next;
        }
        struct node* curr=(struct node*)malloc(sizeof(struct node));
        strcpy(curr->value,c);
        curr->next=NULL;
        t->next=curr;
        keywords[hash(c)]->t=createToken(c,tt); //token
    }
}
int findEntry(char* c){
    struct node*t=keywords[hash(c)];
    while(t!=NULL){
        if(strcmp(c,t->value)==0){
            return 1;
        }
        t=t->next;
    }
    return 0;
}
int printHashtable(){
    for(int i=0;i<MAX_TABLE;i++){
        if(keywords[i]!=NULL){
            printf("Hash entry %d\n",i);
            struct node* t=keywords[i];
            while(t!=NULL){
                printf("%s\n",t->value);
                t=t->next;
            }
        }
    }
}
void initializeHashtable(){
    for(int i=0;i<MAX_TABLE;i++){
        keywords[i]=NULL;
    }
}
int main(){
    initializeHashtable();
    char *c="nfsjsdnfjdnf";
    insertEntry(c);
    insertEntry(c);
    printHashtable();
    int a=findEntry(c);
    printf("%d\n",a);
}