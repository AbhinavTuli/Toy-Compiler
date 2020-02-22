#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexerDef.h"
#include "parser.h"
#include <stdbool.h>

void ComputeFirstAndFollow(){

    for(int i=0;i<grammarLength;i++){ // which non-terminal

        currentFirstFollowCompute = i;
        tempSize=0;
        struct ntRules* currentNtRules = &grammar[i];
        // Checking a non-terminal's rules
        for(int j=0;j<grammar[i].numRules;j++){ // ruleNo
            CurrentRuleTokenFF = &grammar[i].heads[j];
            // printf("Check10 : %s\n",currentNtRules->nt);
            computeRecursiveFirst(i,j);
            // computeRecursiveFollow(i,currentNtRules->nt,&(currentNtRules->heads[j]));
            
        }
        // printf("Check1_4 : %s\n",grammar[i].nt);
        addFirsts(currentFirstFollowCompute);   // add all from temp array 
        // addFollows(i);
    }
}

void computeRecursiveFirst(int index,int j){

        struct ruleToken* CurrentRuleToken = &grammar[index].heads[j];
        printf("Current Computing FF of : %s \n",grammar[currentFirstFollowCompute].nt);
        printf("In recursive : %s \n",grammar[index].nt);

        // printf("Check : %s\n",grammar[index].nt);

        if(strcmp(CurrentRuleToken->tnt,"ε")==0){
            // see the diff b/w CurrentRuleToken and CurrentRuleTokenFF
            if(CurrentRuleTokenFF->next!=NULL){
                CurrentRuleTokenFF = CurrentRuleTokenFF->next;

                if(CurrentRuleTokenFF->tag==1){
                    // add this terminal
                    if(checkIfTokenAlreadyPresent(CurrentRuleTokenFF))
                    return;

                    strcpy(temp[tempSize++],CurrentRuleTokenFF->tnt);
                    printf("Addding %s\n",temp[tempSize-1]);
                    return;
                }

                int newIndex = getIndexOfNonTerminal(CurrentRuleTokenFF->tnt);
                for(int k=0;k<grammar[newIndex].numRules;k++)
                computeRecursiveFirst(newIndex,k);
                return;
            }
        }

        struct ntfirstFollow* firstFollowRuleOfCurrentRule = &FirstFollowSets[index];

        strcpy(firstFollowRuleOfCurrentRule->nt,grammar[index].nt);

        
        if(CurrentRuleToken->tag==0)    // Non-Terminal
        {
            int indexOfFirstNtToken  = getIndexOfNonTerminal(CurrentRuleToken->tnt);
            struct ntRules* currentNtRules = &grammar[indexOfFirstNtToken];

            for(int j=0;j<currentNtRules->numRules;j++){
                computeRecursiveFirst(indexOfFirstNtToken,j);
            }

        }else{
            // add terminal to firsts of this non-terminal
            // Check if it is already there or not!   
            // printf("Check12\n"); 

            if(checkIfTokenAlreadyPresent(CurrentRuleToken))
            return;

            strcpy(temp[tempSize++],CurrentRuleToken->tnt);
            printf("Addding %s\n",temp[tempSize-1]);
        }

}

bool checkIfTokenAlreadyPresent(struct ruleToken* CurrentRuleToken){
    
    for(int i=0;i<tempSize;i++){
        // printf("Check13 : %s\n",CurrentRuleToken->tnt); 
        if(strcmp(CurrentRuleToken->tnt,temp[i])==0)
        return true;
    }

    return false;
}

void computeRecursiveFollow(int index,int j){
    findRuleNumbersForRHS_NonTerminals(grammar[index].nt);
    // for(int i=0; i<rhsNTSize;i++){
    //     while(strcmp(grammar[rhsNT[i]]->grammar[index].nt)!=0){

    //     }
    // }
}

void findRuleNumbersForRHS_NonTerminals(char* nt){
    for(int i=0;i<grammarLength;i++){
        struct ntRules* currentNtRules = &grammar[i];
        for(int j=0;j<currentNtRules->numRules;j++){
            struct ruleToken* head = &currentNtRules->heads[j];
            // printf("Check1\n");
            while(head!=NULL){
                // printf("Check4 : %s\n",head->tnt);
                if(strcmp(head->tnt,nt)==0){
                    // printf("Check3\n");
                    rhsNT[rhsNTSize++] = i;
                    break;
                }
                head = head->next;
            }
            // printf("Check2\n");
        }
    }
}

int getIndexOfNonTerminal(char* nt){
    for(int i=0;i<numNT;i++){
        if(strcmp(nonterminals[i],nt)==0){
            // Non-Terminal Found
            return i;
        }
    }
    return -1;
}

int getIndexOfTerminal(char *bufferToken){
    for(int i=0;i<numT;i++){
        if(strcmp(terminals[i],bufferToken)==0){
            // Terminal Found
            return i;
        }
    }
    return -1;
}


void addFirsts(int i){
    FirstFollowSets[i].numFirsts = tempSize;
    // printf("Check14\n"); 
    for(int j=0;j<tempSize;j++){
        // printf("Check15 : %s\n",temp[j]); 
        strcpy(FirstFollowSets[i].firsts[j],temp[j]);
    }

    memset(temp, '\0', sizeof(char)*10);
}

void addFollows(int i){
    FirstFollowSets[i].numFollows = tempSize2;
    for(int j=0;j<tempSize2;j++){
        strcpy(FirstFollowSets[i].follows[j],temp2[j]);
    }
}

void addRuleToGrammer(char* rule){

    int i=0,j=0;
    bool got_nt = false;
    char bufferToken[50];
    struct ntRules* ntRulePointer;
    struct ruleToken* prevToken = NULL;
    struct ruleToken* tempRuleToken;

    while(rule[i]!='\n'){

        if(rule[i]==' '){
            i++;
            continue;
        }
       

        if(!got_nt){
            // printf("Check1\n");

            // If reading L.H.S non-terminal is still left
            got_nt =true;

            if(rule[i]=='<'){
                i++;
                j=0;
                while(rule[i]!='>'){
                    bufferToken[j++] = rule[i++];
                }
                bufferToken[j++] = 0;
                // Got the L.H.S of rule(NT)
                i++;
                // printf("Check2 : %s\n",bufferToken);
                int index = getIndexOfNonTerminal(bufferToken);

                if(index==-1)   // Not Found - First Occurence of this Non-Terminal
                {
                    // printf("Check3\n");
                    strcpy(nonterminals[numNT++],bufferToken); // storing all non-terminals in one place
                    ntRulePointer = &grammar[grammarLength++];
                    strcpy(ntRulePointer->nt,bufferToken);
                }
                else{
                    // printf("Check4\n");
                    ntRulePointer = &grammar[index];
                }
                
                tempRuleToken = &(ntRulePointer->heads[ntRulePointer->numRules++]); // Got this rule's position!
                // printf("Addding Rule Number : %d\n",ntRulePointer->numRules);
            }
            memset( bufferToken, '\0', sizeof(char)*50); // empty the bufferToken
            j=0;
        }
        else{
            // printf("Check5\n");

            if(rule[i]=='-'){
                // printf("Check5_5\n");
                i=i+4;
                continue;
            }

            while(rule[i]==' '){
                i++;
            }

            if(rule[i]=='<'){
                // printf("Check6\n");
                // Non-terminal encountered
                i++;
                j=0;
                while(rule[i]!='>' && rule[i]!='\n'){
                    bufferToken[j++] = rule[i++];
                }
                bufferToken[j++] = 0;

                if(rule[i]=='>')
                i++;

                if(prevToken==NULL){
                    // first token at R.H.S
                    prevToken = tempRuleToken;
                    tempRuleToken->tag = 0;
                    tempRuleToken->next = NULL;
                    strcpy(tempRuleToken->tnt,bufferToken);
                }else{
                    struct ruleToken* newToken = (struct ruleToken*)malloc(sizeof(struct ruleToken));
                    newToken->tag=0;
                    newToken->next=NULL;
                    strcpy(newToken->tnt,bufferToken);
                    prevToken->next = newToken;
                    prevToken = newToken;
                }

                memset( bufferToken, '\0', sizeof(char)*50); // empty the bufferToken
                j=0;
            }
            else{
                
                // Terminal encountered
                j=0;
                while(rule[i]!=' ' && rule[i]!='\n'){
                    bufferToken[j++] = rule[i++];
                }

                bufferToken[j++] = 0;

                if(rule[i]==' ')
                i++;

                // printf("Check7 : %s\n",bufferToken);

                if(getIndexOfTerminal(bufferToken)==-1){
                    strcpy(terminals[numT++],bufferToken);
                }

                if(prevToken==NULL){
                    // printf("Check8\n");
                    // first token at R.H.S
                    prevToken = tempRuleToken;
                    tempRuleToken->tag = 1;
                    tempRuleToken->next = NULL;
                    strcpy(tempRuleToken->tnt,bufferToken);
                }else{
                    // printf("Check9\n");
                    struct ruleToken* newToken = (struct ruleToken*)malloc(sizeof(struct ruleToken));
                    newToken->tag=1;
                    newToken->next=NULL;
                    strcpy(newToken->tnt,bufferToken);
                    prevToken->next = newToken;
                    prevToken = newToken;
                }

                memset( bufferToken, '\0', sizeof(char)*50); // empty the bufferToken
                j=0;
            }
        }

    }
    // printf("Adding Rule : \t %s\n", rule);
    // printRuleGrammarStruct(grammarLength-1);
}

void printAllNonTerminals(){
    for(int i=0;i<numNT;i++){
        printf("%s\n",nonterminals[i]);
    }
}

void printAllTerminals(){
     for(int i=0;i<numT;i++){
        printf("%s\n",terminals[i]);
    }
}

void printAllFirstSets(){
    for(int i=0;i<grammarLength;i++){
        printf("%s \t : ",FirstFollowSets[i].nt);

        for(int j=0;j<FirstFollowSets[i].numFirsts;j++){
            printf("%s  ",FirstFollowSets[i].firsts[j]);
        }
        printf("\n");
    }
}

void printRuleGrammarStruct(int i){
    printf("NT : %s\n",grammar[i].nt);
    for(int j=0;j<grammar[i].numRules;j++){
        printf("Rule %d : \n",j+1);
        struct ruleToken* head = &grammar[i].heads[j];
        while(head!=NULL){
            printf("Token  : %s\n",head->tnt);
            head = head->next;
        }
        printf("\n");
    }
}


int main(){

    FILE* fp = fopen("grammar.txt","r");

    if (fp == NULL) {
        printf("Error Opening Grammar file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }else{

        bool alternateLine = 1;

        while(fgets(buffer, BUFF_SIZE, fp)) {
            // buffer[strcspn(buffer, "\n")] = 0;

            if(alternateLine){
                // printf("Adding\n");
                addRuleToGrammer(buffer);
                // printf("Adding Rule : \t %s\n", buffer);
            }

            alternateLine = !alternateLine;
            // break;
        }
    }

    // ComputeFirstAndFollow();
    // printAllFirstSets();
    // printAllNonTerminals();
    // printAllTerminals();
    
    findRuleNumbersForRHS_NonTerminals("otherModules");

    for(int i=0;i<rhsNTSize;i++){
        printf("%d\n",rhsNT[i]);
    }

    // printRuleGrammarStruct(0);
}
