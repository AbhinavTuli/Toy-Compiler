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

        currentFirstFollowCompute = i;
        strcpy(firstFollowSets[i].nt,grammar[i].nt);

        // printf("\nComputingFirst For %s : %d\n\n",grammar[currentFirstFollowCompute].nt,grammar[i].numRules);

        tempFirstsSize = 0;
        memset(tempFirsts, '\0', sizeof(char)*MAX_FIRST);
        stackSize = 0;
        memset(stack, -1, sizeof(int)*MAX_FIRST);
        
        for(int j=0;j<grammar[i].numRules;j++){
        // i - Specifies NT,   j - Specifies Rule
            CurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            prevCurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            computeFirstRecursive(i,j);     
        }
        // computeFirstForSingleNT(i);
        addFirsts(i);
        // These are temporary global variables to store firsts and their total size
        // break;
    }

    int index = getIndexOfNonTerminal("program");
    int size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++],"DRIVERDEF");

    index = getIndexOfNonTerminal("statements");
    size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++],"USE");

}

void computeFirstForSingleNT(int indexNT){

    if(checkIfPresentInStack(indexNT)){
        // printf("%d %d\n",currentFirstFollowCompute,indexNT);
        return;
    }
    //  printf("Check10\n");

    if(indexNT==-1)
    {
        printf("Error!\n");
        exit(0);
    }

    for(int j=0;j<grammar[indexNT].numRules;j++){
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
        // printf("Check1\n");
        int newIndex = getIndexOfNonTerminal(CurrentRuleHead->tnt);

        if(newIndex==currentFirstFollowCompute)
        return;

        // printf("Going to Another NT : %s\n",grammar[newIndex].nt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }else{
        // If head is T - tag is 1
        // printf("Check2\n");
        if(isEpsilon(CurrentRuleHead->tnt)){
            // printf("Check3 : %s\n",CurrentRuleTokenGlobal->tnt);
            // Fetch First of next NT as well
            if(CurrentRuleTokenGlobal->next!=NULL){
                // printf("Check5 : %s\n",CurrentRuleTokenGlobal->tnt);
                computeFirstRecursive2(CurrentRuleHead,CurrentRuleTokenGlobal);
                // computeFirstRecursive2(CurrentRuleHead,CurrentRuleTokenGlobal->next);
                if(prevCurrentRuleTokenGlobal->next!=NULL){
                    // printf("Check6 : %s\n",prevCurrentRuleTokenGlobal->tnt);
                    computeFirstRecursive2(CurrentRuleHead,prevCurrentRuleTokenGlobal);
                    return;
                }
                
            }else{

                if(prevCurrentRuleTokenGlobal->next!=NULL){
                    // printf("Check6 : %s\n",prevCurrentRuleTokenGlobal->tnt);
                    computeFirstRecursive2(CurrentRuleHead,prevCurrentRuleTokenGlobal);
                    return;
                }

                // Add Epsilon
                if(!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt)){
                    // printf("Add 2 : %s\n",CurrentRuleHead->tnt);
                    strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);
                }
                
            }
        }
        else{
            // printf("Check4\n");
            if(!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt)){
            // Terminal Added if not a repeat
            // printf("Adding %s to %s\n",CurrentRuleHead->tnt,grammar[currentFirstFollowCompute].nt);
            strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);
            // printf("Add 3 : %s\n",CurrentRuleHead->tnt);
            return;
            }
        }
    }
}

void computeFirstRecursive2(struct ruleToken* CurrentRuleHead,struct ruleToken* toAddRuleHead){

    if(toAddRuleHead->next==NULL)
    return;

    if(toAddRuleHead->next->tag==1){
        if(!checkIfFirstAlreadyPresent(toAddRuleHead->next->tnt)){
            // printf("Add 1 : %s\n",toAddRuleHead->next->tnt);
            strcpy(tempFirsts[tempFirstsSize++],toAddRuleHead->next->tnt);
        }
    }else{

        // CurrentRuleTokenGlobal = CurrentRuleTokenGlobal->next;
        int newIndex = getIndexOfNonTerminal(toAddRuleHead->next->tnt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }
}

bool checkIfPresentInStack(int indexNT){
    for(int i=0;i<stackSize;i++){
        if(stack[i]==indexNT)
        return true; // present in stack
    }

    stack[stackSize++] = indexNT;
    return false;
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

    // strcpy(firstFollowSets[indexNT].nt,grammar[indexNT].nt); // Not needed since this task was done in firsts!

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

    for(int i=0;i<firstFollowSets[indexNT].numFirsts;i++){

        // if(strcmp(grammar[currentFirstFollowCompute].nt,"moduleDeclarations")==0)
        // printf("Check Rule %d\n",i);


        if(isEpsilon(firstFollowSets[indexNT].firsts[i])){
            
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

            if(!checkIfFollowAlreadyPresent(firstFollowSets[indexNT].firsts[i])){
                strcpy(tempFollows[tempFollowsSize++],firstFollowSets[indexNT].firsts[i]);
            }
        }
    }
}

void addFirsts(int indexNT){
    firstFollowSets[indexNT].numFirsts = tempFirstsSize;
    for(int j=0;j<tempFirstsSize;j++){ 
        strcpy(firstFollowSets[indexNT].firsts[j],tempFirsts[j]);
    }
}

void addFollows(int indexNT){
    // printf("Adding %d follows to %s\n",tempFollowsSize,grammar[indexNT].nt);
    firstFollowSets[indexNT].numFollows = tempFollowsSize;
    for(int j=0;j<tempFollowsSize;j++){ 
        strcpy(firstFollowSets[indexNT].follows[j],tempFollows[j]);
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
        printf("%s \t : ",firstFollowSets[i].nt);

        for(int j=0;j<firstFollowSets[i].numFirsts;j++){
            printf("%s  ",firstFollowSets[i].firsts[j]);
        }
        printf("\n");
    }
}

void printAllFollowSets(){
    for(int i=0;i<grammarLength;i++){
        printf("%s \t : ",firstFollowSets[i].nt);

        for(int j=0;j<firstFollowSets[i].numFollows;j++){
            printf("%s  ",firstFollowSets[i].follows[j]);
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
            printf("Token  : %s - %d\n",head->tnt,head->tag);
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

void readGrammerTextFile(FILE* fp){
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
}

int* firstofRHS(struct ruleToken* head){
    
    int* indexFirsts = (int*)malloc(sizeof(int)*MAX_FIRST);
    int indexPos = 0;
    bool checkEp = false;

    struct ruleToken* temp = head;

    while(temp!=NULL && temp->tag==0){

        int indexNT = getIndexOfNonTerminal(temp->tnt);

        for(int i=0;i<firstFollowSets[indexNT].numFirsts;i++){
            if(isEpsilon(firstFollowSets[indexNT].firsts[i])){
                checkEp = true;
                continue;
            }

            indexFirsts[indexPos++] = getIndexOfTerminal(firstFollowSets[indexNT].firsts[i]);
        }

        if(checkEp){
            checkEp = false;
            temp = temp->next;
        }else
        break;
    }

    if(temp!=NULL && temp->tag==1){
        indexFirsts[indexPos++] = getIndexOfTerminal(temp->tnt);
    }

    indexFirsts[indexPos++] = -1;

    printf("Check : %d\n",indexPos);

    return indexFirsts;
}


void createParseTable() //, int[][] Table)
{
    strcpy(terminals[numT++],"$"); // adding dollar to terminals

    for(int i=0; i<numNT; i++)
    {
        for(int j=0; j<numT; j++)
        {
            Table[i][j] = -1; //default
        }
    }

    //populating the table now
    
    int whichRule = 0;
    
    for(int i=0; i<numNT; i++)       //for each production
    {
        //rule from NT n
        struct ntRules rule = grammar[i];

        char heads[rule.numRules][NTSIZE];     //rule for each NT can have multiple heads too
        int tag[rule.numRules];                            //whether terminal or non terminal
        
        for(int k=0; k<rule.numRules; k++)                  //update the names of the heads and their tags 
        {
            strcpy(heads[k], rule.heads[k].tnt);
            tag[k] = rule.heads[k].tag; 
        }
        
        char nonT[NTSIZE];
        strcpy(nonT,rule.nt);

        //for every rule n -> alpha
        //head is alpha in every rule 
        for(int k=0; k<rule.numRules; k++)
        {
            int indexNT = getIndexOfNonTerminal(nonT);
            int headIndex;

            //DEBUG stmt printf("\n%d\n",whichRule+k);


            if(tag[k] == 0)              //non terminal
            {
                int* indexList = firstofRHS(&grammar[i].heads[k]);
                int t = 0;

                while(indexList[t]!=-1){
                    Table[indexNT][indexList[t]] = whichRule;
                    printf("AddingFirst %s  %s   :  %d\n",nonT,terminals[indexList[t]],whichRule);
                    t++;
                }

                // headIndex = getIndexOfNonTerminal(heads[k]);
                
                // //for every terminal in first(alpha)
            

                // for(int j=0; j<firstFollowSets[indexNT].numFirsts; j++)
                // {
                    
                //     int indexT = getIndexOfTerminal(firstFollowSets[indexNT].firsts[j]);
                //     // if(indexNT==16 && indexT==1){

                //     // }
                //     Table[indexNT][indexT] = whichRule;
                //     printf("AddingFirst %s  %s   :  %d\n",nonT,firstFollowSets[indexNT].firsts[j],whichRule);
                // }
            }
            else                          //terminal
            {
                if(strcmp(heads[k],"ε")==0) //if the terminal is ε
                {
                    //for each b (terminal) in follow(n)
                    for(int j=0; j<firstFollowSets[i].numFollows; j++)
                    {
                        int indexT = getIndexOfTerminal(firstFollowSets[i].follows[j]);
                        Table[indexNT][indexT] = whichRule;
                        printf("AddingFollow_ε %s  %s   :  %d\n",nonT,firstFollowSets[i].follows[j],whichRule);
                    }

                }
                else
                {
                    headIndex = getIndexOfTerminal(heads[k]);
                    Table[indexNT][headIndex] = whichRule;       //only one terminal in FIRST() because it itself is a terminal
                    printf("AddingT  %s  %s   :  %d\n",nonT,heads[k],whichRule);
                }     
            } 
            whichRule++;
        }

    }

    // addDollarToParseTable();
}

void addDollarToParseTable(){
    for(int i=0;i<numNT;i++){
        int checkEp = checkIfEpTerminalRuleExists(i);
        if(checkEp==-1){
            // check follow of this
            // checkFollowForDollar(i){
                
            // }
            Table[i][numT] = checkEp;
        }else{
            Table[i][numT] = checkEp;
        }
    }
}

int checkIfEpTerminalRuleExists(int index){
    for(int i=0;i<grammar[index].numRules;i++){
        if(isEpsilon(grammar[index].heads[i].tnt))
        {
            return getRuleNumber(index,i);
        }
    }
    return -1;
}

int getRuleNumber(int index,int rule){
    int count = 0;
    for(int i=0;i<grammarLength;i++){
        int nR = grammar[i].numRules;
        for(int j=0;j<nR;j++){

            if(i==index && j==rule)
            return count;

            count++;
        }
    }

    return -1;
}

void printParseTable(){
    for(int i=0; i<=numNT; i++)
    {
        printf("\n");
        for(int j=0; j<=numT; j++)
        {
            printf(",%d", Table[i][j]); 

        }
    }
}

void printParseTableToFile(){

    FILE *fp = fopen("OutputTable.txt","w");

    fprintf(fp,"%30s"," ");

    for(int i=0;i<numT;i++){
        fprintf(fp,"%10s",terminals[i]);
    }

    for(int i=0; i<numNT; i++)
    {
        fprintf(fp,"\n%30s",nonterminals[i]); // rows
    
        for(int j=0; j<numT; j++)
        {
            fprintf(fp,"%10d", Table[i][j]); 

        }
    }

}

void printRuleNumbers(){

    FILE* fp = fopen("RuleNumbers.txt","w");
    int count = 0;

    for(int i=0;i<grammarLength;i++){
        int nR = grammar[i].numRules;
        for(int j=0;j<nR;j++){
            fprintf(fp,"\nRule %d  %s\t: ",count,grammar[i].nt);
            count++;
            struct ruleToken* head = &grammar[i].heads[j];
            while(head!=NULL){
                fprintf(fp,"%s ",head->tnt);
                head = head->next;
            }
        }
    }
}

void printFirstFollowsToFile(){
    
    FILE *fp = fopen("FirstFollowsTable.txt","w");
    fprintf(fp,"\t\t\t\t\t%s","Firsts\t\t");
    fprintf(fp,"%20s\n","Follows");

    for(int i=0;i<numNT;i++){
        fprintf(fp,"\n%s\n",grammar[i].nt);
        for(int j=0;j<10;j++){
            if(j>firstFollowSets[i].numFirsts && j>firstFollowSets[i].numFollows)
            break;

            fprintf(fp,"\t\t\t\t\t%s\t\t%20s\n",firstFollowSets[i].firsts[j],firstFollowSets[i].follows[j]);
        }
    }

}


// void createParseTableTemp(){

//     initializeParseTable();

//     strcpy(terminals[getIndexOfTerminal("ε")],"$"); // Parse Table have column of "$" and No "ε"

//     int ruleNo = 0;

//     for(int i=0;i<numNT;i++){
//         for(int j=0;j<grammar[i].numRules;j++){
//             // Iterating rule by rule

//             for(int k=0;k<firstFollowSets[i].firsts;k++){
                
//                 if(isEpsilon(firstFollowSets[i].firsts[k])){
//                     // check follows of this NT

//                 }else{
//                     struct ruleToken* head = grammar[i].heads[j];
//                     while()
//                 }
//             }

//             ruleNo++;
//         }
//     }

// }

int findRuleForTerminal(int i,int j){
    for(int k=0;k<grammar[i].numRules;k++){
        struct ruleToken* head = &grammar[i].heads[k];

        if(head->tag==1){ // If Terminal
            if(strcmp(head->tnt,terminals[j])==0)
            return getRuleNumber(i,k);
        }
        else{
            int indexNT = getIndexOfNonTerminal(head->tnt);
            // check if first of indexNT has the terminal j
            for(int a=0;a<firstFollowSets[indexNT].numFirsts;a++){
                if(strcmp(firstFollowSets[indexNT].firsts[a],terminals[j]))
                return getRuleNumber(i,k);
            }
        }
    }
    return -1;
}

void initializeParseTable(){
    parsingTable = (int**)malloc(sizeof(int**)*numNT);
    for(int i=0;i<numNT;i++){
        parsingTable[i] = (int*)malloc(sizeof(int)*numT);
        for(int j=0;j<numT;j++){
            parsingTable[i][j] = -1;
        }
    }
}

void runParser(){

    FILE* fp = fopen("grammar.txt","r");

    readGrammerTextFile(fp);

    printf("\nComputing Firsts And Follows Started\n");
    computeFirstAndFollow();
    printf("\nComputing Firsts And Follows Ended\n\n");

    printFirstFollowsToFile();

    // printAllFirstSets();

    printf("\nComputing Parse Table Started\n");
    createParseTable();
    printf("\nComputing Parse Table Ended\n\n");
    printParseTableToFile();


    // printParseTable();

    // printRuleNumbers();

     // printf("Total Non-Terminals : %d\n",numNT);
    // printf("Total Terminals : %d\n\n",numT);

    // printAllGrammar();
    // printAllTerminals();
    // printAllNonTerminals();

    // printf("\nFirst Sets : \n\n");
    // printAllFirstSets();

    // printf("\n\nFollow Sets : \n\n");
    // printAllFollowSets();
}

// int main(){
//     runParser();
// }