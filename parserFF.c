#include "parserFF.h"

void computeFirstAndFollow(){

    // Compute First Sets
    computeFirstAllSets();

    // Compute Follows Sets
    computeFollowAllSets();

}

void computeFirstAllSets(){

    // For each NT
    for(int i=0;i<grammarLength;i++){    
        // For all rules of NT compute Firsts

        tempFirstsSize = 0;
        memset(tempFirsts, '\0', sizeof(char)*MAX_FIRST);
        
        computeFirstForSingleNT(i);
        addFirsts(i);

        // These are temporary global variables to store firsts and their total size
    }
}

void computeFirstForSingleNT(int indexNT){

    currentFirstFollowCompute = indexNT;

    // printf("\nComputingFirst For %s : %d\n",grammar[indexNT].nt,grammar[indexNT].numRules);
    strcpy(FirstFollowSets[indexNT].nt,grammar[indexNT].nt);

    for(int j=0;j<grammar[indexNT].numRules;j++){

        CurrentRuleTokenGlobal = &grammar[indexNT].heads[j];
        // indexNT - Specifies NT,   j - Specifies Rule
        computeFirstRecursive(indexNT,j);     
    }
}

void computeFirstRecursive(int indexNT,int ruleNum){

     struct ruleToken* CurrentRuleHead = &grammar[indexNT].heads[ruleNum];
    // Each Rule is a linked list!
    // CurrentRuleHead starts with first token of that rule

    // printf("computeFirstRecursive : %s  %d\n",grammar[indexNT].nt,ruleNum);
    // printf("CurrentRuleHead : %s %d\n",CurrentRuleHead->tnt,CurrentRuleHead->tag);

    if(CurrentRuleHead->tag==0){
        // If head is NT
        int newIndex = getIndexOfNonTerminal(CurrentRuleHead->tnt);
        computeFirstForSingleNT(newIndex);
    }else{
        // If head is T
        if(isEpsilon(CurrentRuleHead->tnt) && CurrentRuleTokenGlobal->next!=NULL){
            // Fetch First of next NT as well
            
            if(CurrentRuleTokenGlobal->next->tag==1)
            strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);

            CurrentRuleTokenGlobal = CurrentRuleTokenGlobal->next;

            int newIndex = getIndexOfNonTerminal(CurrentRuleTokenGlobal->tnt);
            computeFirstForSingleNT(newIndex);
        }

        if(!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt)){
            // Terminal Added if not a repeat
            // printf("Adding %s to %s\n",CurrentRuleHead->tnt,grammar[currentFirstFollowCompute].nt);
            strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);
        }

    }
}

bool checkIfFirstAlreadyPresent(char* str){
    for(int i=0;i<tempFirstsSize;i++){
        if(strcmp(str,tempFirsts[i])==0)
        return true;
    }
    return 
    false;
}

bool checkIfFollowAlreadyPresent(char* str){
    for(int i=0;i<tempFollowsSize;i++){
        if(strcmp(str,tempFollows[i])==0)
        return true;
    }
    return 
    false;
}

void computeFollowAllSets(){

    // For each NT
    for(int i=0;i<grammarLength;i++){

        currentFirstFollowCompute = i;
        computeFollowForSingleNT(i);
        addFollows(i);

        // These are temporary global variables to store follows and their total size
        tempFollowsSize = 0;
        memset(tempFollows, '\0', sizeof(char)*MAX_FOLLOW);
    }

}

void computeFollowForSingleNT(int indexNT){

    if(indexNT==0){
        // Assuming 0 index has start position
        strcpy(tempFollows[tempFollowsSize++],"$");
    }

    // strcpy(FirstFollowSets[indexNT].nt,grammar[indexNT].nt); // Not needed since this task was done in firsts!

    int* rhsNT = (int*) malloc(sizeof(int)*15);    // contains line position
    int* rhsNTRulePos = (int*) malloc(sizeof(int)*15); // contains rule position
    int rhsNTSize =0;

    rhsNTSize = findRHSPositions(grammar[indexNT].nt,rhsNT,rhsNTRulePos);
    // Output will be stored in rhsNTSize, rhsNT, rhsNTRulePos!

    // printf("\nComputingFollow For %s : %d\n",grammar[indexNT].nt,rhsNTSize);

    for(int i=0;i<rhsNTSize;i++){
        computeFollowRecursive(grammar[indexNT].nt,rhsNT[i],rhsNTRulePos[i]);
        // rhsNT[i] will contains the line no.s(Other NT's L.H.S position) where NT exists on R.H.S
    }

}

void computeFollowRecursive(char* strNT,int lineNo,int ruleNo){

    bool checkLaterHalf =true;

    struct ruleToken* rulePointer = &grammar[lineNo].heads[ruleNo];
    struct ntRules* startToken = &grammar[lineNo];

    while(checkLaterHalf){

        while(rulePointer!=NULL && strcmp(rulePointer->tnt,strNT)!=0 ){
            rulePointer = rulePointer->next;
        }

        if(rulePointer==NULL)
        break;

        computeFollowRecursive2(rulePointer,startToken);
        rulePointer = rulePointer->next;
    } 

}

void computeFollowRecursive2(struct ruleToken* rulePointer,struct ntRules* startToken){

    if(rulePointer->next==NULL){
        // Follow of First Token of this Rule
        // if(strcmp(grammar[currentFirstFollowCompute].nt,"otherModules")==0)
        // printf("Check2 %s\n",startToken->nt);

        if(strcmp(rulePointer->tnt,startToken->nt)==0)
        return;

        int newIndex = getIndexOfNonTerminal(startToken->nt);
        computeFollowForSingleNT(newIndex);

        return;
    }
    
    if(rulePointer->next->tag==1){
        // If next is a terminal
        if(!checkIfFollowAlreadyPresent(rulePointer->next->tnt)){
            strcpy(tempFollows[tempFollowsSize++],rulePointer->next->tnt);
        } 
    }
    else{
        // If next is a non-terminal
        // Get first of this NT
        // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
        // printf("Check2 %s\n",rulePointer->next->tnt);

        int newIndex = getIndexOfNonTerminal(rulePointer->next->tnt);

        getFirstsForFollows(newIndex,rulePointer->next,startToken);
    }

}

void getFirstsForFollows(int indexNT,struct ruleToken* rulePointer,struct ntRules* startToken){

    // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
    // printf("Check %s\n",rulePointer->tnt);

    for(int i=0;i<FirstFollowSets[indexNT].numFirsts;i++){

        // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
        // printf("Check Rule %d\n",i);


        if(isEpsilon(FirstFollowSets[indexNT].firsts[i])){
            
            // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
            // printf("Check3 %s\n",rulePointer->next->tnt);

            // Find First of Next - TODO
            if(rulePointer->next!=NULL){

                if(rulePointer->next->tag==1){
                    
                    if(!checkIfFollowAlreadyPresent(rulePointer->next->tnt)){
                        strcpy(tempFollows[tempFollowsSize++],rulePointer->next->tnt);
                    }
                    continue;
                }

                int newIndex = getIndexOfNonTerminal(rulePointer->next->tnt);
                getFirstsForFollows(newIndex,rulePointer->next,startToken);
            }else{
                // Follow of First of RulePointer
                // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
                // printf("Check4 %s\n",rulePointer->next->tnt);

                if(strcmp(rulePointer->tnt,startToken->nt)==0)
                continue;

                int newIndex = getIndexOfNonTerminal(startToken->nt);
                computeFollowForSingleNT(newIndex);
            }
        }else{

            // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
            // printf("Check5 %s\n",rulePointer->next->tnt);

            if(!checkIfFollowAlreadyPresent(FirstFollowSets[indexNT].firsts[i])){
                strcpy(tempFollows[tempFollowsSize++],FirstFollowSets[indexNT].firsts[i]);
            }
        }
    }
}

void addFirsts(int indexNT){
    FirstFollowSets[indexNT].numFirsts = tempFirstsSize;
    for(int j=0;j<tempFirstsSize;j++){ 
        strcpy(FirstFollowSets[indexNT].firsts[j],tempFirsts[j]);
    }
    FirstFollowSets[indexNT].status = 1; // First for this NT computed
}

void addFollows(int indexNT){
    // printf("Adding %d follows to %s\n",tempFollowsSize,grammar[indexNT].nt);
    FirstFollowSets[indexNT].numFollows = tempFollowsSize;
    for(int j=0;j<tempFollowsSize;j++){ 
        strcpy(FirstFollowSets[indexNT].follows[j],tempFollows[j]);
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

void printAllFollowSets(){
    for(int i=0;i<grammarLength;i++){
        printf("%s \t : ",FirstFollowSets[i].nt);

        for(int j=0;j<FirstFollowSets[i].numFollows;j++){
            printf("%s  ",FirstFollowSets[i].follows[j]);
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

void printAllGrammar(){
    for(int i=0;i<grammarLength;i++){
        printRuleGrammarStruct(i);
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

int findRHSPositions(char* str,int* rhsNT,int* rhsNTRulePos){

    int rhsNTSize = 0;

    for(int i=0;i<grammarLength;i++){
        struct ntRules* currentNtRules = &grammar[i];
        for(int j=0;j<currentNtRules->numRules;j++){
            struct ruleToken* head = &currentNtRules->heads[j];
            while(head!=NULL){
                if(strcmp(head->tnt,str)==0){
                    rhsNT[rhsNTSize] = i;
                    rhsNTRulePos[rhsNTSize++] = j;
                    break;
                }
                head = head->next;
            }
        }
    }

    return rhsNTSize;
}

bool isEpsilon(char* str){
    if(strcmp(str,"ε")==0)
    return true;
    
    return false;
}


int main(){

    FILE* fp = fopen("grammar.txt","r");

    if (fp == NULL) {
        printf("Error Opening Grammar file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }else{

        bool alternateLine = 1;

        printf("Reading Grammer...\n\n");

        while(fgets(buffer, BUFF_SIZE, fp)) {

            if(alternateLine){
                addRuleToGrammer(buffer);
                // printf("Adding Rule : \t %s\n", buffer);
            }

            alternateLine = !alternateLine;
            // break;
        }
    }

    // printf("Total Non-Terminals : %d\n",numNT);
    // printf("Total Terminals : %d\n\n",numT);

    // printAllGrammar();

    printf("\nComputing Firsts And Follows : \n\n");

    computeFirstAndFollow();

    printf("\nFirst Sets : \n\n");
    printAllFirstSets();
    printf("\n\nFollow Sets : \n\n");
    printAllFollowSets();
}
