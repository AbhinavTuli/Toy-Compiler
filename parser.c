#include "parser.h"
#include "parserDef.h"
#include "lexer.c"

/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
    Rohit Bohra             -   2017A7PS0225P
*/


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

        // These are temporary global variables to store firsts and their total size
        tempFirstsSize = 0;
        memset(tempFirsts, '\0', sizeof(char)*MAX_FIRST);
        stackFirstsSize = 0;
        memset(stackFirsts, -1, sizeof(int)*MAX_FIRST);
        
        for(int j=0;j<grammar[i].numRules;j++){
        // i - Specifies NT,   j - Specifies Rule
            CurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            prevCurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            computeFirstRecursive(i,j);     
        }
        // computeFirstForSingleNT(i);
        addFirsts(i);
        
    }
    computeFirstSets2();
}

void computeFirstForSingleNT(int indexNT){

    if(checkIfPresentInStack(indexNT)){
        return;
    }

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

    if(CurrentRuleHead->tag==0){
        // If head is NT
        int newIndex = getIndexOfNonTerminal(CurrentRuleHead->tnt);

        if(newIndex==currentFirstFollowCompute)
        return;

        // printf("Going to Another NT : %s\n",grammar[newIndex].nt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }else{
        // If head is T - tag is 1
        if(isEpsilon(CurrentRuleHead->tnt)){
            // Fetch First of next NT as well
            if(CurrentRuleTokenGlobal->next!=NULL){
                computeFirstRecursive2(CurrentRuleHead,CurrentRuleTokenGlobal);
                if(prevCurrentRuleTokenGlobal->next!=NULL){
                    computeFirstRecursive2(CurrentRuleHead,prevCurrentRuleTokenGlobal);
                    return;
                }
                
            }else{

                if(prevCurrentRuleTokenGlobal->next!=NULL){
                    computeFirstRecursive2(CurrentRuleHead,prevCurrentRuleTokenGlobal);
                    return;
                }

                // Add Epsilon
                if(!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt)){
                    strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);
                }
                
            }
        }
        else{
            if(!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt)){
            // Terminal Added if not a repeat
            strcpy(tempFirsts[tempFirstsSize++],CurrentRuleHead->tnt);
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
            strcpy(tempFirsts[tempFirstsSize++],toAddRuleHead->next->tnt);
        }
    }else{
        int newIndex = getIndexOfNonTerminal(toAddRuleHead->next->tnt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }
}

bool checkIfPresentInStack(int indexNT){
    for(int i=0;i<stackFirstsSize;i++){
        if(stackFirsts[i]==indexNT)
        return true; // present in stack
    }

    stackFirsts[stackFirstsSize++] = indexNT;
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

    int* rhsNT = (int*) malloc(sizeof(int)*15);    // contains line position
    int* rhsNTRulePos = (int*) malloc(sizeof(int)*15); // contains rule position
    int rhsNTSize =0;

    rhsNTSize = findRHSPositions(grammar[indexNT].nt,rhsNT,rhsNTRulePos);
    // Output will be stored in rhsNTSize, rhsNT, rhsNTRulePos!

    for(int i=0;i<rhsNTSize;i++){
        computeFollowRecursive(grammar[indexNT].nt,rhsNT[i],rhsNTRulePos[i]);
        // rhsNT[i] will contains the line no.s(Other NT's L.H.S position) where NT exists on R.H.S
    }

}

void computeFirstSets2(){
    int index = getIndexOfNonTerminal("program");
    int size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++],"DRIVERDEF");

    index = getIndexOfNonTerminal("statements");
    size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++],"USE");
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

        int newIndex = getIndexOfNonTerminal(rulePointer->next->tnt);

        getFirstsForFollows(newIndex,rulePointer->next,startToken);
    }

}

void getFirstsForFollows(int indexNT,struct ruleToken* rulePointer,struct ntRules* startToken){

    for(int i=0;i<firstFollowSets[indexNT].numFirsts;i++){

        if(isEpsilon(firstFollowSets[indexNT].firsts[i])){

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

                if(strcmp(rulePointer->tnt,startToken->nt)==0)
                continue;

                int newIndex = getIndexOfNonTerminal(startToken->nt);
                computeFollowForSingleNT(newIndex);
            }
        }else{

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
                int index = getIndexOfNonTerminal(bufferToken);

                if(index==-1)   // Not Found - First Occurence of this Non-Terminal
                {
                    strcpy(nonterminals[numNT++],bufferToken); // storing all non-terminals in one place
                    ntRulePointer = &grammar[grammarLength++];
                    strcpy(ntRulePointer->nt,bufferToken);
                }
                else{
                    ntRulePointer = &grammar[index];
                }
                
                tempRuleToken = &(ntRulePointer->heads[ntRulePointer->numRules++]); // Got this rule's position!
            }
            memset( bufferToken, '\0', sizeof(char)*50); // empty the bufferToken
            j=0;
        }
        else{

            if(rule[i]=='-'){
                i=i+4;
                continue;
            }

            while(rule[i]==' '){
                i++;
            }

            if(rule[i]=='<'){
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

                if(getIndexOfTerminal(bufferToken)==-1){
                    strcpy(terminals[numT++],bufferToken);
                }

                if(prevToken==NULL){
                    // first token at R.H.S
                    prevToken = tempRuleToken;
                    tempRuleToken->tag = 1;
                    tempRuleToken->next = NULL;
                    strcpy(tempRuleToken->tnt,bufferToken);
                }else{
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

    // printf("Check : %d\n",indexPos);

    return indexFirsts;
}


void createParseTable() 
{
    strcpy(terminals[numT++],"$"); // adding dollar to terminals

    for(int i=0; i<numNT; i++)
    {
        for(int j=0; j<numT; j++)
        {
            Table[i][j] = -1; //    default
        }
    }

    //  populating the table now
    
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

            if(tag[k] == 0)              // non terminal
            {
                int* indexList = firstofRHS(&grammar[i].heads[k]);
                int t = 0;

                while(indexList[t]!=-1){
                    Table[indexNT][indexList[t]] = whichRule;
                    // printf("AddingFirst %s  %s   :  %d\n",nonT,terminals[indexList[t]],whichRule);
                    t++;
                }
            }
            else                          //    terminal
            {
                if(strcmp(heads[k],"ε")==0) //if the terminal is ε
                {
                    //for each b (terminal) in follow(n)
                    for(int j=0; j<firstFollowSets[i].numFollows; j++)
                    {
                        int indexT = getIndexOfTerminal(firstFollowSets[i].follows[j]);
                        Table[indexNT][indexT] = whichRule;
                        // printf("AddingFollow_ε %s  %s   :  %d\n",nonT,firstFollowSets[i].follows[j],whichRule);
                    }

                }
                else
                {
                    headIndex = getIndexOfTerminal(heads[k]);
                    Table[indexNT][headIndex] = whichRule;       //only one terminal in FIRST() because it itself is a terminal
                    // printf("AddingT  %s  %s   :  %d\n",nonT,heads[k],whichRule);
                }     
            } 
            whichRule++;
        }

    }
}

void printParseTableToFile(){

    FILE *fp = fopen("ParseTable.txt","w");

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

void printRuleNumbersToFile(){

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


void parseInputSourceCode(token* HEAD, int Table[MAX_NON_TERMINALS][MAX_TERMINALS],struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS],FILE* parseTreeFile)
{
    struct treeNode* root = createTreeNode();
    root->next = NULL;

    token* temp;
    temp = HEAD;

    lex* top;
    top = (lex*)malloc(sizeof(lex));
    //push the $ , then start symbol on the stack 
    push(&top, 1, terminals[getIndexOfTerminal("$")]);
    push(&top, 0, nonterminals[0]);
    strcpy(root->tnt, nonterminals[0]);
    root->tag = 0;

    
    while(temp!= NULL)
    {
        //printf("\nStack is---------\n");
        //printf("Th is %d\n",isEmpty(top));
        //printStack(top);
        struct treeNode* tempTreeNode;
        struct treeNode* tempTreeNodeParent = findLeftMostWithoutChild(root);
        if(tempTreeNodeParent==NULL){ //parse tree complete
            //printf("Popping $\n");
            pop(&top); //popping $
            //printf("This is %d\n",isEmpty(top));
            printStack(top);
            if(isEmpty(top)==0){
                //printf("Yo\n");
                break;
            }
            printf("Parse tree is:- \n");
            inOrderParseTree(root,parseTreeFile);
            printf("\n\nParseTreeFile Generated\n");
            break;
        }
        if(strcmp(tempTreeNodeParent->tnt,"")!=0){
            tempTreeNode = createTreeNode();
            tempTreeNodeParent->child=tempTreeNode; 
        }
        else{
            tempTreeNode=tempTreeNodeParent;
        }
        term tokterm = temp->tokterm;

        //functionNames/ identifiers
        char currLexeme[20];
        strcpy(currLexeme, getLexeme(tokterm) );
        //printf("Currently at %s\n",currLexeme);

        //compare currName with name on the stack
        if( strcmp(currLexeme, top->tnt)==0 )
        {
            //need to pop accoring to
            //printf("Popping on match %s\n",top->tnt);
            pop(&top);
            temp=temp->next;
        }
        else
        {
            int i;
            int j;
            if(top->tag == 0)
                i = getIndexOfNonTerminal(top->tnt);
            else
                i = getIndexOfTerminal(top->tnt);
                
            j = getIndexOfTerminal(currLexeme);
            int ruleNo = Table[i][j];
            if(ruleNo==-1){
                printf("Error at %d. Currently reading %s, stack top is %s\n",temp->lineno,currLexeme,top->tnt);
                return;
            }

            struct ruleToken head;
            //push gramar[ruleNo] on to the stack
            int count = 0, cumulative=0;
            for(int i=0; i<MAX_NON_TERMINALS; i++)
            {
                
                count+=grammar[i].numRules;
                if(ruleNo < count) //found here
                {
                     head = grammar[i].heads[ruleNo - cumulative];
                     //printf("Yo %d\n",ruleNo-cumulative);
                     break;
                }

                cumulative+=grammar[i].numRules;
            }
            //printf("Popping on applying rule %d on stack top %s\n",ruleNo ,top->tnt);
            pop(&top); 
            //push(&top, head.tag, head.tnt);

            strcpy(tempTreeNode->tnt, head.tnt);
            tempTreeNode->tag = head.tag;
            tempTreeNode->next = NULL;
            tempTreeNode->child = NULL;

            
            struct ruleToken* headNext = head.next;

            struct ruleToken ARRAY[20]; 
            struct ruleToken ZEROS;
            ZEROS.next = NULL;
            ZEROS.tag = 0;
            strcpy(ZEROS.tnt, "");

            for(int i=0; i<20; i++) //setting every ARRAY[i] to 0
            {
                ARRAY[i] = ZEROS;
            }
            ARRAY[0] = head;
            int index = 0;

            while(headNext!=NULL)
            {
                ARRAY[++index] = *headNext;
                tempTreeNode->next = createTreeNode();
                tempTreeNode = tempTreeNode->next;

                strcpy(tempTreeNode->tnt, headNext->tnt);
                tempTreeNode->tag = headNext->tag;
                tempTreeNode->next = NULL;
                tempTreeNode->child = NULL;   

                
                //push(&top, headNext->tag, headNext->tnt );
                headNext = headNext->next;
            }

            //push ulta
            for(int i= index; i>=0; i--)
            {   
                if(strcmp(ARRAY[i].tnt,"ε")){
                    push(&top, ARRAY[i].tag, ARRAY[i].tnt );
                }
            }
            
        }
    }
    //if stack isn't empty
    if(isEmpty(top)==0){
        printf("Error in parsing! Stack isn't empty at the end! Parse Tree Not generated! \n");
        return;
    }

}

// Stack Functions
void printStack(lex* top){
    lex* temp=top;
    while(temp!=NULL){
        printf("%s\n", temp->tnt);
        temp=temp->next;
    }
}
int isEmpty(lex* root) 
{ 
	if(root->next==NULL){
        return 1;
    } 
    return 0;
} 

void push(lex** root, int data, char* str) 
{ 
	lex* stackNode = newNode(data,str); 
	stackNode->next = *root; 
	*root = stackNode; 
	//printf("%s pushed to stack\n", str); 
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

void inOrderParseTree(struct treeNode* root,FILE* parseTreeFile){
    if (root == NULL) 
        return; 
    struct treeNode* temp=root->child;
    // Total children count 
      
    // Print leftmost child
    inOrderParseTree(temp,parseTreeFile);
    if(temp!=NULL)
        temp=temp->next;
  
    // Print the current node's data 
    printf("%s ",root->tnt); 
    fflush(parseTreeFile);
    fprintf(parseTreeFile,"%s ",root->tnt);
  
    // Print all other children
    while(temp!=NULL){
            inOrderParseTree(temp,parseTreeFile);
            temp=temp->next;
    } 
}

token* createToken()
{
    token* temp;
    temp = (token*)malloc(sizeof(token));
    return temp;
}


struct treeNode* findLeftMostWithoutChild(struct treeNode* root)
{
    if(root==NULL){
        return NULL;
    }
    if(root->child==NULL && root->tag==0){
        return root;
    }
    // if(root->child == NULL && root->next==NULL)
    // {
    //     return root;
    // }

    struct treeNode* temp = root->child;
    //struct treeNode* parent;

    struct treeNode* result=NULL;
    if(temp != NULL)
    {
        result = findLeftMostWithoutChild(temp);
        if(result!=NULL && result->tag==1){
            result=NULL;
        }
    }

    while(result == NULL && temp!=NULL ) 
    {
        result = findLeftMostWithoutChild(temp -> next);
        if(result!=NULL && result->tag==1){
            result=NULL;
        }

        temp = temp->next;
    }

    return result;
}


void runParser(FILE* testFile, FILE* parseTreeFile){

    FILE* fp = fopen("grammar.txt","r");

    readGrammerTextFile(fp);

    printf("\nComputing Firsts And Follows Started\n");
    computeFirstAndFollow();
    printf("\nComputing Firsts And Follows Ended\n\n");

    printf("\nComputing Parse Table Started\n");
    createParseTable();
    printf("\nComputing Parse Table Ended\n\n");

    populateKeywordTable();

    programFile = testFile;
    programFile = getStream(programFile);

    if(first){
        populateKeywordTable();
        first = false;
    }


    while(true)
    {
        getNextToken();
        if(currentToken->tokterm == 60)
        break;
    }
    

    printf("Parser Complete\n");

    parseInputSourceCode(head->next, Table, grammar, firstFollowSets,parseTreeFile);
    
    

    /*
        To generate Files : ParseTable.txt
        Uncomment above lines
    */

}

