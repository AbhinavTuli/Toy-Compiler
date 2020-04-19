#include "parser.h"
#include "parserDef.h"
#include "lexer.c"

/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/

void computeFirstAndFollow()
{

    // Compute First Sets
    computeFirstAllSets();

    // Compute Follows Sets
    computeFollowAllSets();
}

void computeFirstAllSets()
{
    // For each NT
    for (int i = 0; i < grammarLength; i++)
    {
        // For all rules of NT compute Firsts

        currentFirstFollowCompute = i;
        strcpy(firstFollowSets[i].nt, grammar[i].nt);

        // These are temporary global variables to store firsts and their total size
        tempFirstsSize = 0;
        memset(tempFirsts, '\0', sizeof(char) * MAX_FIRST);
        stackFirstsSize = 0;
        memset(stackFirsts, -1, sizeof(int) * MAX_FIRST);

        for (int j = 0; j < grammar[i].numRules; j++)
        {
            // i - Specifies NT,   j - Specifies Rule
            CurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            prevCurrentRuleTokenGlobal = &grammar[currentFirstFollowCompute].heads[j];
            computeFirstRecursive(i, j);
        }
        // computeFirstForSingleNT(i);
        addFirsts(i);
    }
    computeFirstSets2();
    memset(tempFirsts, '\0', sizeof(char) * MAX_FIRST);
    memset(stackFirsts, -1, sizeof(int) * MAX_FIRST);
}

void computeFirstForSingleNT(int indexNT)
{

    if (checkIfPresentInStack(indexNT))
    {
        return;
    }

    if (indexNT == -1)
    {
        printf("Error!\n");
        exit(0);
    }

    for (int j = 0; j < grammar[indexNT].numRules; j++)
    {
        // indexNT - Specifies NT,   j - Specifies Rule
        computeFirstRecursive(indexNT, j);
    }
}

void computeFirstRecursive(int indexNT, int ruleNum)
{

    struct ruleToken *CurrentRuleHead = &grammar[indexNT].heads[ruleNum];
    // Each Rule is a linked list!
    // CurrentRuleHead starts with first token of that rule

    if (CurrentRuleHead->tag == 0)
    {
        // If head is NT
        int newIndex = getIndexOfNonTerminal(CurrentRuleHead->tnt);

        if (newIndex == currentFirstFollowCompute)
            return;

        // printf("Going to Another NT : %s\n",grammar[newIndex].nt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }
    else
    {
        // If head is T - tag is 1
        if (isEpsilon(CurrentRuleHead->tnt))
        {
            // Fetch First of next NT as well
            if (CurrentRuleTokenGlobal->next != NULL)
            {
                computeFirstRecursive2(CurrentRuleHead, CurrentRuleTokenGlobal);
                if (prevCurrentRuleTokenGlobal->next != NULL)
                {
                    computeFirstRecursive2(CurrentRuleHead, prevCurrentRuleTokenGlobal);
                    return;
                }
            }
            else
            {

                if (prevCurrentRuleTokenGlobal->next != NULL)
                {
                    computeFirstRecursive2(CurrentRuleHead, prevCurrentRuleTokenGlobal);
                    return;
                }

                // Add Epsilon
                if (!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt))
                {
                    strcpy(tempFirsts[tempFirstsSize++], CurrentRuleHead->tnt);
                }
            }
        }
        else
        {
            if (!checkIfFirstAlreadyPresent(CurrentRuleHead->tnt))
            {
                // Terminal Added if not a repeat
                strcpy(tempFirsts[tempFirstsSize++], CurrentRuleHead->tnt);
                return;
            }
        }
    }
}

void computeFirstRecursive2(struct ruleToken *CurrentRuleHead, struct ruleToken *toAddRuleHead)
{

    if (toAddRuleHead->next == NULL)
        return;

    if (toAddRuleHead->next->tag == 1)
    {
        if (!checkIfFirstAlreadyPresent(toAddRuleHead->next->tnt))
        {
            strcpy(tempFirsts[tempFirstsSize++], toAddRuleHead->next->tnt);
        }
    }
    else
    {
        int newIndex = getIndexOfNonTerminal(toAddRuleHead->next->tnt);
        prevCurrentRuleTokenGlobal = CurrentRuleHead;
        computeFirstForSingleNT(newIndex);
    }
}

bool checkIfPresentInStack(int indexNT)
{
    for (int i = 0; i < stackFirstsSize; i++)
    {
        if (stackFirsts[i] == indexNT)
            return true; // present in stack
    }

    stackFirsts[stackFirstsSize++] = indexNT;
    return false;
}

bool checkIfFirstAlreadyPresent(char *str)
{
    for (int i = 0; i < tempFirstsSize; i++)
    {
        if (strcmp(str, tempFirsts[i]) == 0)
            return true;
    }
    return false;
}

bool checkIfFollowAlreadyPresent(char *str)
{
    for (int i = 0; i < tempFollowsSize; i++)
    {
        if (strcmp(str, tempFollows[i]) == 0)
            return true;
    }
    return false;
}

void computeFollowAllSets()
{

    // For each NT
    for (int i = 0; i < grammarLength; i++)
    {

        currentFirstFollowCompute = i;
        computeFollowForSingleNT(i);
        addFollows(i);

        // These are temporary global variables to store follows and their total size
        tempFollowsSize = 0;
        memset(tempFollows, '\0', sizeof(char) * MAX_FOLLOW);
    }
}

void computeFollowForSingleNT(int indexNT)
{

    if (indexNT == 0)
    {
        // Assuming 0 index has start position
        strcpy(tempFollows[tempFollowsSize++], "$");
    }

    int *rhsNT = (int *)malloc(sizeof(int) * 15);        // contains line position
    int *rhsNTRulePos = (int *)malloc(sizeof(int) * 15); // contains rule position
    int rhsNTSize = 0;

    rhsNTSize = findRHSPositions(grammar[indexNT].nt, rhsNT, rhsNTRulePos);
    // Output will be stored in rhsNTSize, rhsNT, rhsNTRulePos!

    for (int i = 0; i < rhsNTSize; i++)
    {
        computeFollowRecursive(grammar[indexNT].nt, rhsNT[i], rhsNTRulePos[i]);
        // rhsNT[i] will contains the line no.s(Other NT's L.H.S position) where NT exists on R.H.S
    }
}

void computeFirstSets2()
{
    int index = getIndexOfNonTerminal("program");
    int size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++], "DRIVERDEF");

    index = getIndexOfNonTerminal("statements");
    size = firstFollowSets[index].numFirsts;
    firstFollowSets[index].numFirsts++;
    strcpy(firstFollowSets[index].firsts[size++], "USE");
}

void computeFollowRecursive(char *strNT, int lineNo, int ruleNo)
{

    bool checkLaterHalf = true;

    struct ruleToken *rulePointer = &grammar[lineNo].heads[ruleNo];
    struct ntRules *startToken = &grammar[lineNo];

    while (checkLaterHalf)
    {

        while (rulePointer != NULL && strcmp(rulePointer->tnt, strNT) != 0)
        {
            rulePointer = rulePointer->next;
        }

        if (rulePointer == NULL)
            break;

        computeFollowRecursive2(rulePointer, startToken);
        rulePointer = rulePointer->next;
    }
}

void computeFollowRecursive2(struct ruleToken *rulePointer, struct ntRules *startToken)
{

    if (rulePointer->next == NULL)
    {
        // Follow of First Token of this Rule

        if (strcmp(rulePointer->tnt, startToken->nt) == 0)
            return;

        int newIndex = getIndexOfNonTerminal(startToken->nt);
        computeFollowForSingleNT(newIndex);

        return;
    }

    if (rulePointer->next->tag == 1)
    {
        // If next is a terminal
        if (!checkIfFollowAlreadyPresent(rulePointer->next->tnt))
        {
            strcpy(tempFollows[tempFollowsSize++], rulePointer->next->tnt);
        }
    }
    else
    {
        // If next is a non-terminal
        // Get first of this NT

        int newIndex = getIndexOfNonTerminal(rulePointer->next->tnt);

        getFirstsForFollows(newIndex, rulePointer->next, startToken);
    }
}

void getFirstsForFollows(int indexNT, struct ruleToken *rulePointer, struct ntRules *startToken)
{

    for (int i = 0; i < firstFollowSets[indexNT].numFirsts; i++)
    {

        if (isEpsilon(firstFollowSets[indexNT].firsts[i]))
        {

            if (rulePointer->next != NULL)
            {

                if (rulePointer->next->tag == 1)
                {

                    if (!checkIfFollowAlreadyPresent(rulePointer->next->tnt))
                    {
                        strcpy(tempFollows[tempFollowsSize++], rulePointer->next->tnt);
                    }
                    continue;
                }

                int newIndex = getIndexOfNonTerminal(rulePointer->next->tnt);
                getFirstsForFollows(newIndex, rulePointer->next, startToken);
            }
            else
            {
                // Follow of First of RulePointer

                if (strcmp(rulePointer->tnt, startToken->nt) == 0)
                    continue;

                int newIndex = getIndexOfNonTerminal(startToken->nt);
                computeFollowForSingleNT(newIndex);
            }
        }
        else
        {

            if (!checkIfFollowAlreadyPresent(firstFollowSets[indexNT].firsts[i]))
            {
                strcpy(tempFollows[tempFollowsSize++], firstFollowSets[indexNT].firsts[i]);
            }
        }
    }
}

void addFirsts(int indexNT)
{
    firstFollowSets[indexNT].numFirsts = tempFirstsSize;
    for (int j = 0; j < tempFirstsSize; j++)
    {
        strcpy(firstFollowSets[indexNT].firsts[j], tempFirsts[j]);
    }
}

void addFollows(int indexNT)
{
    firstFollowSets[indexNT].numFollows = tempFollowsSize;
    for (int j = 0; j < tempFollowsSize; j++)
    {
        strcpy(firstFollowSets[indexNT].follows[j], tempFollows[j]);
    }
}

void addRuleToGrammer(char *rule)
{

    int i = 0, j = 0;
    bool got_nt = false;
    char bufferToken[50];
    struct ntRules *ntRulePointer;
    struct ruleToken *prevToken = NULL;
    struct ruleToken *tempRuleToken;

    while (rule[i] != '\n')
    {

        if (rule[i] == ' ')
        {
            i++;
            continue;
        }

        if (!got_nt)
        {

            // If reading L.H.S non-terminal is still left
            got_nt = true;

            if (rule[i] == '<')
            {
                i++;
                j = 0;
                while (rule[i] != '>')
                {
                    bufferToken[j++] = rule[i++];
                }
                bufferToken[j++] = 0;
                // Got the L.H.S of rule(NT)
                i++;
                int index = getIndexOfNonTerminal(bufferToken);

                if (index == -1) // Not Found - First Occurence of this Non-Terminal
                {
                    strcpy(nonterminals[numNT++], bufferToken); // storing all non-terminals in one place
                    ntRulePointer = &grammar[grammarLength++];
                    strcpy(ntRulePointer->nt, bufferToken);
                }
                else
                {
                    ntRulePointer = &grammar[index];
                }

                tempRuleToken = &(ntRulePointer->heads[ntRulePointer->numRules++]); // Got this rule's position!
            }
            memset(bufferToken, '\0', sizeof(char) * 50); // empty the bufferToken
            j = 0;
        }
        else
        {

            if (rule[i] == '-')
            {
                i = i + 4;
                continue;
            }

            while (rule[i] == ' ')
            {
                i++;
            }

            if (rule[i] == '<')
            {
                // Non-terminal encountered
                i++;
                j = 0;
                while (rule[i] != '>' && rule[i] != '\n')
                {
                    bufferToken[j++] = rule[i++];
                }
                bufferToken[j++] = 0;

                if (rule[i] == '>')
                    i++;

                if (prevToken == NULL)
                {
                    // first token at R.H.S
                    prevToken = tempRuleToken;
                    tempRuleToken->tag = 0;
                    tempRuleToken->next = NULL;
                    strcpy(tempRuleToken->tnt, bufferToken);
                }
                else
                {
                    struct ruleToken *newToken = (struct ruleToken *)malloc(sizeof(struct ruleToken));
                    newToken->tag = 0;
                    newToken->next = NULL;
                    strcpy(newToken->tnt, bufferToken);
                    prevToken->next = newToken;
                    prevToken = newToken;
                }

                memset(bufferToken, '\0', sizeof(char) * 50); // empty the bufferToken
                j = 0;
            }
            else
            {

                // Terminal encountered
                j = 0;
                while (rule[i] != ' ' && rule[i] != '\n')
                {
                    bufferToken[j++] = rule[i++];
                }

                bufferToken[j++] = 0;

                if (rule[i] == ' ')
                    i++;

                if (getIndexOfTerminal(bufferToken) == -1)
                {
                    strcpy(terminals[numT++], bufferToken);
                }

                if (prevToken == NULL)
                {
                    // first token at R.H.S
                    prevToken = tempRuleToken;
                    tempRuleToken->tag = 1;
                    tempRuleToken->next = NULL;
                    strcpy(tempRuleToken->tnt, bufferToken);
                }
                else
                {
                    struct ruleToken *newToken = (struct ruleToken *)malloc(sizeof(struct ruleToken));
                    newToken->tag = 1;
                    newToken->next = NULL;
                    strcpy(newToken->tnt, bufferToken);
                    prevToken->next = newToken;
                    prevToken = newToken;
                }

                memset(bufferToken, '\0', sizeof(char) * 50); // empty the bufferToken
                j = 0;
            }
        }
    }
    // printf("Adding Rule : \t %s\n", rule);
    // printRuleGrammarStruct(grammarLength-1);
}

void printAllNonTerminals()
{
    for (int i = 0; i < numNT; i++)
    {
        printf("%s\n", nonterminals[i]);
    }
}

void printAllTerminals()
{
    for (int i = 0; i < numT; i++)
    {
        printf("%s\n", terminals[i]);
    }
}

void printAllFirstSets()
{
    for (int i = 0; i < grammarLength; i++)
    {
        printf("%s \t : ", firstFollowSets[i].nt);

        for (int j = 0; j < firstFollowSets[i].numFirsts; j++)
        {
            printf("%s  ", firstFollowSets[i].firsts[j]);
        }
        printf("\n");
    }
}

void printAllFollowSets()
{
    for (int i = 0; i < grammarLength; i++)
    {
        printf("%s \t : ", firstFollowSets[i].nt);

        for (int j = 0; j < firstFollowSets[i].numFollows; j++)
        {
            printf("%s  ", firstFollowSets[i].follows[j]);
        }
        printf("\n");
    }
}

void printRuleGrammarStruct(int i)
{
    printf("NT : %s\n", grammar[i].nt);
    for (int j = 0; j < grammar[i].numRules; j++)
    {
        printf("Rule %d : \n", j + 1);
        struct ruleToken *head = &grammar[i].heads[j];
        while (head != NULL)
        {
            printf("Token  : %s - %d\n", head->tnt, head->tag);
            head = head->next;
        }
        printf("\n");
    }
}

void printAllGrammar()
{
    for (int i = 0; i < grammarLength; i++)
    {
        // printf("Check\n");
        printRuleGrammarStruct(i);
    }
}

int getIndexOfNonTerminal(char *nt)
{
    for (int i = 0; i < numNT; i++)
    {
        if (strcmp(nonterminals[i], nt) == 0)
        {
            // Non-Terminal Found
            return i;
        }
    }
    return -1;
}

int getIndexOfTerminal(char *bufferToken)
{

    for (int i = 0; i < numT; i++)
    {
        if (strcmp(terminals[i], bufferToken) == 0)
        {
            // Terminal Found
            return i;
        }
    }
    return -1;
}

int findRHSPositions(char *str, int *rhsNT, int *rhsNTRulePos)
{

    int rhsNTSize = 0;

    for (int i = 0; i < grammarLength; i++)
    {
        struct ntRules *currentNtRules = &grammar[i];
        for (int j = 0; j < currentNtRules->numRules; j++)
        {
            struct ruleToken *head = &currentNtRules->heads[j];
            while (head != NULL)
            {
                if (strcmp(head->tnt, str) == 0)
                {
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

bool isEpsilon(char *str)
{
    if (strcmp(str, "ε") == 0)
        return true;

    return false;
}

void readGrammerTextFile(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error Opening Grammar file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    else
    {

        bool alternateLine = 1;

        // if (printFlag)
        //     printf("Reading Grammer...\n\n");

        while (fgets(buffer, BUFF_SIZE, fp))
        {

            if (alternateLine)
            {
                addRuleToGrammer(buffer);
                // printf("Adding Rule : \t %s\n", buffer);
            }

            alternateLine = !alternateLine;
            // break;
        }
    }
}

int *firstofRHS(struct ruleToken *head)
{

    int *indexFirsts = (int *)malloc(sizeof(int) * MAX_FIRST);
    int indexPos = 0;
    bool checkEp = false;

    struct ruleToken *temp = head;

    while (temp != NULL && temp->tag == 0)
    {

        int indexNT = getIndexOfNonTerminal(temp->tnt);

        for (int i = 0; i < firstFollowSets[indexNT].numFirsts; i++)
        {
            if (isEpsilon(firstFollowSets[indexNT].firsts[i]))
            {
                checkEp = true;
                continue;
            }

            indexFirsts[indexPos++] = getIndexOfTerminal(firstFollowSets[indexNT].firsts[i]);
        }

        if (checkEp)
        {
            checkEp = false;
            temp = temp->next;
        }
        else
            break;
    }

    if (temp != NULL && temp->tag == 1)
    {
        indexFirsts[indexPos++] = getIndexOfTerminal(temp->tnt);
    }

    indexFirsts[indexPos++] = -1;

    // printf("Check : %d\n",indexPos);

    return indexFirsts;
}

void createParseTable()
{
    strcpy(terminals[numT++], "$"); // adding dollar to terminals

    for (int i = 0; i < numNT; i++)
    {
        for (int j = 0; j < numT; j++)
        {
            Table[i][j] = -1; //    default
        }
    }

    //  populating the table now

    int whichRule = 0;

    for (int i = 0; i < numNT; i++) //for each production
    {

        for (int k = 0; k < grammar[i].numRules; k++)
        {
            int indexNT = getIndexOfNonTerminal(grammar[i].nt);
            int headIndex;

            if (grammar[i].heads[k].tag == 0) // non terminal
            {
                int *indexList = firstofRHS(&grammar[i].heads[k]);
                int t = 0;

                while (indexList[t] != -1)
                {
                    Table[indexNT][indexList[t]] = whichRule;
                    // printf("AddingFirst %s  %s   :  %d\n",nonT,terminals[indexList[t]],whichRule);
                    t++;
                }
            }
            else //    terminal
            {
                if (strcmp(grammar[i].heads[k].tnt, "ε") == 0) //if the terminal is ε
                {
                    //for each b (terminal) in follow(n)
                    for (int j = 0; j < firstFollowSets[i].numFollows; j++)
                    {
                        int indexT = getIndexOfTerminal(firstFollowSets[i].follows[j]);
                        Table[indexNT][indexT] = whichRule;
                        // printf("AddingFollow_ε %s  %s   :  %d\n",nonT,firstFollowSets[i].follows[j],whichRule);
                    }
                }
                else
                {
                    headIndex = getIndexOfTerminal(grammar[i].heads[k].tnt);
                    Table[indexNT][headIndex] = whichRule; //only one terminal in FIRST() because it itself is a terminal
                    // printf("AddingT  %s  %s   :  %d\n",nonT,heads[k],whichRule);
                }
            }
            whichRule++;
        }
    }
}

void printParseTableToFile()
{

    FILE *fp = fopen("ParseTable.txt", "w");

    fprintf(fp, "%30s", " ");

    for (int i = 0; i < numT; i++)
    {
        fprintf(fp, "%10s", terminals[i]);
    }

    for (int i = 0; i < numNT; i++)
    {
        fprintf(fp, "\n%30s", nonterminals[i]); // rows

        for (int j = 0; j < numT; j++)
        {
            fprintf(fp, "%10d", Table[i][j]);
        }
    }
}

void printRuleNumbersToFile()
{

    FILE *fp = fopen("RuleNumbers.txt", "w");
    int count = 0;

    for (int i = 0; i < grammarLength; i++)
    {
        int nR = grammar[i].numRules;
        for (int j = 0; j < nR; j++)
        {
            fprintf(fp, "\nRule %d  %s\t: ", count, grammar[i].nt);
            count++;
            struct ruleToken *head = &grammar[i].heads[j];
            while (head != NULL)
            {
                fprintf(fp, "%s ", head->tnt);
                head = head->next;
            }
        }
    }
}

void printFirstFollowsToFile()
{

    FILE *fp = fopen("FirstFollowsTable.txt", "w");
    fprintf(fp, "\t\t\t\t\t%s", "Firsts\t\t");
    fprintf(fp, "%20s\n", "Follows");

    for (int i = 0; i < numNT; i++)
    {
        fprintf(fp, "\n%s\n", grammar[i].nt);
        for (int j = 0; j < 10; j++)
        {
            if (j > firstFollowSets[i].numFirsts && j > firstFollowSets[i].numFollows)
                break;

            fprintf(fp, "\t\t\t\t\t%s\t\t%20s\n", firstFollowSets[i].firsts[j], firstFollowSets[i].follows[j]);
        }
    }
}

struct treeNode *createTreeNode()
{
    numParseNodes++;

    struct treeNode *temp;
    temp = (struct treeNode *)malloc(sizeof(struct treeNode));
    temp->next = NULL;
    temp->lineno = -5;
    strcpy(temp->tnt, "");
    temp->child = NULL;
    temp->tag = 0;
    return temp;
}

struct ruleToken *createNode()
{
    struct ruleToken *temp;
    temp = (struct ruleToken *)malloc(sizeof(struct ruleToken));
    temp->next = NULL;
    temp->tag = 0;
    strcpy(temp->tnt, "");
    return temp;
}

bool checkIfInFirst(char *currLexeme, lex *top)
{

    int index = getIndexOfNonTerminal(top->tnt);

    for (int i = 0; i < firstFollowSets[index].numFirsts; i++)
    {
        if (strcmp(firstFollowSets[index].firsts[i], currLexeme) == 0)
        {
            return true;
        }
    }

    return false;
}

bool checkIfInFollows(char *currLexeme, lex *top)
{

    int index = getIndexOfNonTerminal(top->tnt);

    for (int i = 0; i < firstFollowSets[index].numFollows; i++)
    {
        if (strcmp(firstFollowSets[index].follows[i], currLexeme) == 0)
        {
            return true;
        }
    }

    return false;
}

bool checkIfLexicalErrorLine(int line)
{
    for (int i = 0; i < numLexicalErrors; i++)
    {
        if (lexicalErrors[i] == line)
            return true;
    }
    return false;
}

struct treeNode *parseInputSourceCode(token *HEAD, int Table[MAX_NON_TERMINALS][MAX_TERMINALS], struct ntRules grammar[MAX_NON_TERMINALS], struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS])
{
    struct treeNode *root = createTreeNode();
    root->next = NULL;

    token *temp;
    temp = HEAD;

    lex *top;
    top = (lex *)malloc(sizeof(lex));
    //push the $ , then start symbol on the stack
    push(&top, 1, terminals[getIndexOfTerminal("$")], NULL);
    push(&top, 0, nonterminals[0], NULL);
    strcpy(root->tnt, nonterminals[0]);
    root->tag = 0;

    while (temp != NULL)
    {

        if (checkIfLexicalErrorLine(temp->lineno))
        {
            temp = temp->next;
            continue;
        }

        // printf("\nStack is---------\n");
        // printStack(top);
        struct treeNode *tempTreeNode;
        struct treeNode *tempTreeNodeParent = findLeftMostWithoutChild(root);

        if (tempTreeNodeParent == NULL)
        { //parse tree complete
            //printf("Popping $\n");
            pop(&top); //popping $
            //printf("This is %d\n",isEmpty(top));
            // printStack(top);
            if (isEmpty(top) == 0)
            {
                // printf("Yo\n");
                break;
            }

            // inOrderParseTree(root,parseTreeFile);

            return root;
        }

        if (strcmp(tempTreeNodeParent->tnt, "") != 0)
        {
            tempTreeNode = createTreeNode();
            tempTreeNodeParent->child = tempTreeNode;
        }
        else
        {
            tempTreeNode = tempTreeNodeParent;
        }

        term tokterm = temp->tokterm;

        //functionNames/identifiers
        char currLexeme[20];
        strcpy(currLexeme, getLexeme(tokterm));
        //printf("Currently at %s\n",currLexeme);
        //printf("debug\n");
        //compare currName with name on the stack

        if (strcmp(currLexeme, top->tnt) == 0)
        {
            //POP ON MATCH
            //need to pop accoring to
            // printf("Popping on match %s , %s\n",top->tnt,tempTreeNode->tnt);
            if (checkForValueToken(currLexeme))
            {
                // Add Value to the TreeNode

                if (top->pTreePointer != NULL)
                {
                    top->pTreePointer->val = temp->val;
                    top->pTreePointer->tagUnion = temp->tag;
                    top->lineno = temp->lineno; // Input LineNo
                    // printf("Add Value : %s\n",top->tnt);
                }
            }
            if (top->pTreePointer != NULL)
                top->pTreePointer->lineno = temp->lineno;

            pop(&top);
            temp = temp->next;
        }
        else
        {
            int i;
            int j;
            if (top->tag == 0)
                i = getIndexOfNonTerminal(top->tnt);
            else
                i = getIndexOfTerminal(top->tnt);

            j = getIndexOfTerminal(currLexeme);
            int ruleNo = Table[i][j];
            if (ruleNo == -1)
            {

                if (isEmpty(top) == 0 && strcmp(currLexeme, "$") == 0)
                {
                    printf("Error in parsing! Stack isn't empty at the end! Parse Tree Not generated!\n");
                    return NULL;
                }

                if (printFlag)
                {
                    if (strcmp(top->tnt, "SEMICOL") == 0)
                        printf("Line %d: Syntax Error. Current token is %s, stack top is %s\n", temp->lineno - 1, currLexeme, top->tnt);
                    else
                        printf("Line %d: Syntax Error. Current token is %s, stack top is %s\n", temp->lineno, currLexeme, top->tnt);
                }

                while (temp->next != NULL)
                {

                    if (checkIfLexicalErrorLine(temp->lineno))
                    {
                        temp = temp->next;
                        continue;
                    }

                    // printf("Tag : %d\n",top->tag);

                    if (top->tag == 1)
                    {

                        if (strcmp(currLexeme, top->tnt) == 0)
                            temp = temp->next;

                        pop(&top);
                    }

                    if (checkIfInFirst(currLexeme, top))
                    {
                        //temp=temp->next;
                        break;
                    }
                    if (checkIfInFollows(currLexeme, top))
                    {
                        // printf("Entered here\n");
                        pop(&top);
                        break;
                    }

                    temp = temp->next;
                    // printf("Skipping\n");
                    strcpy(currLexeme, getLexeme(temp->tokterm));
                }

                if (temp == NULL)
                    return NULL;

                continue;
            }

            struct ruleToken head;
            //push gramar[ruleNo] on to the stack
            int count = 0, cumulative = 0;
            for (int i = 0; i < MAX_NON_TERMINALS; i++)
            {

                count += grammar[i].numRules;
                if (ruleNo < count) //found here
                {
                    head = grammar[i].heads[ruleNo - cumulative];
                    //printf("Yo %d\n",ruleNo-cumulative);
                    break;
                }

                cumulative += grammar[i].numRules;
            }
            // printf("Popping on applying rule %d on stack top %s\n",ruleNo ,top->tnt);
            pop(&top);
            //push(&top, head.tag, head.tnt);

            strcpy(tempTreeNode->tnt, head.tnt);
            tempTreeNode->tag = head.tag;
            tempTreeNode->next = NULL;
            tempTreeNode->child = NULL;
            //if (head.tag == 1) //only if Terminal, then adding the lineno makes sense
            tempTreeNode->lineno = temp->lineno;
            // printf("Add %s\n",tempTreeNode->tnt);

            if (checkForValueToken(tempTreeNode->tnt))
            {

                // printf("Check!! : %s\n",tempTreeNode->tnt);

                switch (temp->tag)
                {
                case 1:
                    tempTreeNode->val.i = temp->val.i;
                    tempTreeNode->tagUnion = 1;
                    // printf("Add Int %d\n",temp->val.i);
                    break;

                case 2:
                    tempTreeNode->val.f = temp->val.f;
                    tempTreeNode->tagUnion = 2;
                    // printf("Add Float %f\n",temp->val.f);
                    break;

                case 3:
                    tempTreeNode->val.b = temp->val.b;
                    tempTreeNode->tagUnion = 3;
                    // printf(temp->val.b ? "true\n" : "false\n");
                    break;

                case 4:
                    strcpy(tempTreeNode->val.s, temp->val.s);
                    tempTreeNode->tagUnion = 4;
                    // printf("Add Str %s\n",temp->val.s);
                    break;

                default:
                    break;
                }
            }

            struct ruleToken *headNext = head.next;

            struct ruleToken ARRAY[25];
            struct ruleToken ZEROS;
            ZEROS.next = NULL;
            ZEROS.tag = 0;
            strcpy(ZEROS.tnt, "");

            for (int i = 0; i < 10; i++) //setting every ARRAY[i] to 0
            {
                ARRAY[i] = ZEROS;
            }
            ARRAY[0] = head;
            int index = 0;

            while (headNext != NULL)
            {
                ARRAY[++index] = *headNext;
                tempTreeNode->next = createTreeNode();
                tempTreeNode = tempTreeNode->next;

                strcpy(tempTreeNode->tnt, headNext->tnt);
                tempTreeNode->tag = headNext->tag;
                tempTreeNode->next = NULL;
                tempTreeNode->child = NULL;
                //if (headNext->tag == 1) //only if nonterminal, lineno makes sense
                tempTreeNode->lineno = temp->lineno;
                // printf("Add2 %s\n",tempTreeNode->tnt);
                ARRAY[index].pTreePointer = tempTreeNode;
                //push(&top, headNext->tag, headNext->tnt );
                headNext = headNext->next;
            }

            //push ulta
            for (int i = index; i >= 0; i--)
            {
                if (strcmp(ARRAY[i].tnt, "ε"))
                {
                    push(&top, ARRAY[i].tag, ARRAY[i].tnt, ARRAY[i].pTreePointer);
                }
            }
        }
    }
    //if stack isn't empty
    if (isEmpty(top) == 0)
    {
        printf("Error in parsing! Stack isn't empty at the end! Parse Tree Not generated!\n");
        return NULL;
    }

}

// Stack Functions
void printStack(lex *top)
{
    lex *temp = top;
    while (temp != NULL)
    {
        printf("%s\n", temp->tnt);
        temp = temp->next;
    }
}
int isEmpty(lex *root)
{
    if (root->next == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void push(lex **root, int data, char *str, struct treeNode *pTreePointer)
{
    lex *stackNode = newNode(data, str);
    stackNode->next = *root;
    // stackNode->lineno = pTreePointer->lineno;
    stackNode->pTreePointer = pTreePointer;
    *root = stackNode;
    //printf("%s pushed to stack\n", str);
}

int pop(lex **root)
{
    if (isEmpty(*root))
        return INT_MIN;
    lex *temp = *root;
    *root = (*root)->next;
    int popped = temp->tag;
    free(temp);

    return popped;
}

int peek(lex *root)
{
    if (isEmpty(root))
        return INT_MIN;
    return root->tag;
}

void inOrderParseTree(struct treeNode *root)
{
    if (root == NULL)
        return;
    struct treeNode *temp = root->child;
    // Total children count

    // Print leftmost child
    inOrderParseTree(temp);
    if (temp != NULL)
        temp = temp->next;

    // Print the current node's data
    // if (root->tag == 1)
    //     printf("%s#%d ", root->tnt, root->lineno);
    // else
        printf("%s ", root->tnt);
    
    while (temp != NULL)
    {
        inOrderParseTree(temp);
        temp = temp->next;
    }
}

token *createToken()
{
    token *temp;
    temp = (token *)malloc(sizeof(token));
    return temp;
}

struct treeNode *findLeftMostWithoutChild(struct treeNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (root->child == NULL && root->tag == 0)
    {
        return root;
    }
    // if(root->child == NULL && root->next==NULL)
    // {
    //     return root;
    // }

    struct treeNode *temp = root->child;
    //struct treeNode* parent;

    struct treeNode *result = NULL;
    if (temp != NULL)
    {
        result = findLeftMostWithoutChild(temp);
        if (result != NULL && result->tag == 1)
        {
            result = NULL;
        }
    }

    while (result == NULL && temp != NULL)
    {
        result = findLeftMostWithoutChild(temp->next);
        if (result != NULL && result->tag == 1)
        {
            result = NULL;
        }

        temp = temp->next;
    }

    return result;
}

void parserFree()
{

    tempFirstsSize = 0;
    tempFollowsSize = 0;
    stackFirstsSize = 0;
    numNT = 0;
    numT = 0;
    currentFirstFollowCompute = 0;
    grammarLength = 0;
    CurrentRuleTokenGlobal = NULL;
    prevCurrentRuleTokenGlobal = NULL;
    memset(tempFirsts, '\0', sizeof(char) * MAX_FIRST);
    memset(stackFirsts, -1, sizeof(int) * MAX_FIRST);
    memset(buffer, '\0', sizeof(char) * BUFF_SIZE);

    memset(grammar, 0, MAX_NON_TERMINALS * sizeof(struct ntRules));
}

bool checkForValueToken(char *str)
{

    if (strcmp(str, "ID") == 0 || strcmp(str, "NUM") == 0 || strcmp(str, "RNUM") == 0)
        return true;

    return false;
}

void runParser(FILE *testFile)
{

    parserFree();

    FILE *fp = fopen("grammar.txt", "r");

    readGrammerTextFile(fp);

    computeFirstAndFollow();

    createParseTable();

    populateKeywordTable();

    programFile = testFile;
    programFile = getStream(programFile);

    if (first)
    {
        populateKeywordTable();
        first = false;
    }

    while (true)
    {
        getNextToken();
        if (currentToken->tokterm == 60)
            break;
    }

    struct treeNode *rootParseTree = parseInputSourceCode(head->next, Table, grammar, firstFollowSets);

    inOrderParseTree(rootParseTree);

    /*
        To generate Files : ParseTable.txt
        Uncomment above lines
    */
}
