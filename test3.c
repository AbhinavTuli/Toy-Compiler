#include <stdlib.h>
#include <stdio.h>
#include "lexerDef.h"

#define NTSIZE 	30
#define TSIZE	20
#define BUFF_SIZE 150
#define NON_TERMINALS 50

char buffer[BUFF_SIZE];

struct rule{
    token nt;
    token** heads;
};

struct firstFollow{
    token nt;
    token** firsts;
    token** follows;
};


struct rule grammar[NON_TERMINALS];
struct firstFollow FirstFollowSets[NON_TERMINALS];

// void ComputeFirstAndFollow(grammar G,FirstandFollow F)
// {
    
// }


int main(){

    FILE* fp = fopen("grammar.txt","r");

    if (fp == NULL) {
        printf("Error Opening Grammer file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }else{
        while(fgets(buffer, BUFF_SIZE, fp)) {
            printf("%s\n", buffer);
        }
    }

}
