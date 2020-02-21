#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexerDef.h"
#include <stdbool.h>

#define NTSIZE 	30
#define TSIZE	20
#define BUFF_SIZE 150
#define NON_TERMINALS 50

char buffer[BUFF_SIZE];

struct ruleToken{
    int tag; // Non-terminal(0) or terminal(1)
    char tnt[30];
    struct ruleToken* next;
};

struct ntRules{
    char nt[NTSIZE];
    int numRules;
    struct ruleToken* heads;
};

struct ntfirstFollow{
    char nt[NTSIZE];

    char* firsts[TSIZE]; // firsts consists only of terminals
    int numFirsts;

    char* follows[TSIZE]; // follows consists only of terminals
    int numRules;
};

struct ntRules grammar[NON_TERMINALS];
int grammarLength = 0;

struct ntfirstFollow FirstFollowSets[NON_TERMINALS];

void ComputeFirstAndFollow()
{
    for(int i=0;i<grammarLength;i++){

        struct ntRules currentRules = grammar[i];

        token* currentNt = &currentRules.nt;

        for(int j=0;j<currentRules.numRules;j++){
            
            FirstFollowSets[i] = 
        }

    }
}


int main(){

    FILE* fp = fopen("grammar.txt","r");

    if (fp == NULL) {
        printf("Error Opening Grammar file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }else{

        bool alternateLine = 0;

        while(fgets(buffer, BUFF_SIZE, fp)) {
            buffer[strcspn(buffer, "\n")] = 0;

            if(alternateLine)
            printf("%s\n", buffer);

            alternateLine = !alternateLine;
            // break;
        }
    }

}
