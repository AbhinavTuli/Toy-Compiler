/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/

#include "ast.h"
#include "symbolTableDef.h"
#include "symbolTable.h"
#include "constructST.h"

functionTable *globalFuncTable;
variableTable *currentVarTable;
variableTable *driverVarTable;
int globalNestingLevel = 0;

struct expNode *expRoot; // Expression Node

// For generateAB_Expression
char tempStr[50];
arr_index tempArrIndex;

int otherModPos;

int pass = 1;

int global_offset = 0;

int getType(struct astNode *root)
{ // initially root is expression!
    // 0,1,2,3,4 all good
    //type mismatch
    //array operation illegal
    //undeclared variable being used
    //means non array can't be accessed by index
    //array index is non integer
    //bounds incorrect
    if (root == NULL)
        return 0;

    if (root->left == NULL && root->right == NULL)
    {
        if (strcmp(root->name, "NUM") == 0)
        {
            return 1;
        }
        else if (strcmp(root->name, "RNUM") == 0)
        {
            return 2;
        }
        else if (strcmp(root->name, "boolConstt") == 0)
        {
            return 3;
        }
        else if (strcmp(root->name, "ID") == 0)
        {
            if (searchNested(currentVarTable, root->val.s))
            {
                variableTableEntry *idEntry = searchNestedRetrieve(currentVarTable, root->val.s);

                if (idEntry->isArray)
                {
                    return 3 + idEntry->tag;
                    // 4 -Integer Array
                    // 5 - Float Array
                    // 6 - Boolean Array
                }

                return (idEntry->tag);
            }
            else
            {
                printf("ERROR at line no. %d: Variable %s not declared before use\n", root->lineno, root->val.s);
                return -11;
            }
        }
        else
        {
            printf("Unexpected %s\n", root->name);
        }
    }

    // TODO : ARRAY index
    if (strcmp(root->name, "ARRAY_ELE") == 0)
    {
        if (searchNested(currentVarTable, root->left->val.s))
        {
            variableTableEntry *arrIdEntry = searchNestedRetrieve(currentVarTable, root->left->val.s);
            if (arrIdEntry->isArray)
            {
                // Check Bounds for the given index
                if (root->right->tag == 1)
                { // index is NUM
                    int index = root->right->val.i;
                    if (arrIdEntry->isArrayStatic)
                    {
                        if (index > (arrIdEntry->lowerBound) && index < (arrIdEntry->upperBound))
                        { // Within Bounds
                            //printf("ARRAY Element %s[%d] within bounds\n",root->left->val.s,index);
                            return arrIdEntry->tag;
                        }
                        else
                        {
                            printf("ERROR at line no. %d: Element of array %s[%d] is Out of bounds\n", root->lineno, root->left->val.s,index);
                            return -5;
                        }
                    }
                    else
                    {
                        // if dynamic created
                        return arrIdEntry->tag;
                    }
                }
                else
                { //index is not NUM
                    // A[b] - index is ID
                    char *indexId = root->val.s;
                    if (searchNested(currentVarTable, indexId))
                    {
                        variableTableEntry *indexIdEntry = searchNestedRetrieve(currentVarTable, indexId);

                        if (indexIdEntry->isArray)
                        {
                            // A[B] - both A and B are array
                            printf("ERROR at line no. %d: Index %s cannot be an Array\n", root->lineno, indexId);
                            return -6;
                        }

                        if (indexIdEntry->tag == 1)
                        {                           // Index is Valid - Integer
                            return arrIdEntry->tag; // return array type
                        }
                        else
                        { // A[b] - b is not an integer type
                            printf("ERROR at line no. %d: Index %s is not an Integer Type\n", root->lineno, indexId);
                            return -7;
                        }
                    }
                    else
                    { // A[b] - b not declared!
                        printf("ERROR at line no. %d: Index %s is not declared\n", root->lineno, indexId);
                        return -8;
                    }
                }
            }
            else
            {
                // Non array accessed by index, A[b], A is not array
                printf("ERROR at line no. %d: Non-Array %s cannot be accessed by an index\n", root->lineno, root->left->val.s);
                return -9;
            }
        }
        else
        {
            // if searchNested didn't found the Array ID
            printf("ERROR at line no. %d: ID %s is not declared\n", root->lineno, root->left->val.s);
            return -10;
        }
    }

    int l = getType(root->left);
    int r = getType(root->right);

    if(l<-1 || r<-1)
    return -12;

    if (l == -1 || r == -1)
    {
        return -1;
    }
    if (r != 0 && l != r)
    {
        printf("ERROR at line no. %d: Type Mismatch \n", root->lineno);
        return -2;
    }
    if (l >= 4 && r != 0)
    {
        printf("ERROR at lineno. %d: These Array operations aren't permitted\n", root->lineno);
        return -3;
    }
    if (r >= 4)
    {
        printf("ERROR at lineno. %d: These Array operations aren't permitted\n", root->lineno);
        return -4;
    }
    if (l == r)
    {
        if (strcmp(root->name, "relationalOp") == 0)
        {
            return 3;
        }
    }
    return l;
}

bool checkIfArrayType(struct astNode *node)
{

    if (node->child == NULL)
        return false;
    else
    {
        return true;
    }
}

struct expNode *makeExpNode(int tag, char *name, bool isDynamic, arr_index index)
{
    struct expNode *node = (struct expNode *)malloc(sizeof(struct expNode));
    node->tag = tag;

    if (tag == 4 || tag == 0) // id or op
        strcpy(node->name, name);

    if (tag == 5)
    {
        node->isDynamic = isDynamic;

        if (isDynamic == false)
            node->index.i = index.i;
        else
            strcpy(node->index.s, index.s);
    }

    node->next = NULL;

    return node;
}

void constructST2(struct astNode *root)
{

    // printf("constructST2 : %s line%d\n", root->name,root->lineno);

    if (root == NULL) 
    {
        return;
        // Report ERROR!
        //doubt error line
        printf("ConstructST2 : Root is NULL");
        exit(0);
    }

    struct astNode *temp = root; // Using this for traversal

    if (strcmp(root->name, "program") == 0)
    {
        // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
        temp = temp->child; // <moduleDeclarations>
        constructST2(temp);
        // otherModPos = 1;
        temp = temp->next; // <otherModules1>
        constructST2(temp);
        temp = temp->next; // <driverModule>
        constructST2(temp);
        // otherModPos = 2;
        temp = temp->next;  // <otherModules2>
        constructST2(temp); // Treat this in a different manner!
        return;             // Not necessary!!
    }
    else if (strcmp(root->name, "moduleDeclarations") == 0)
    {
        return;
    }
    else if (strcmp(root->name, "driverModule") == 0)
    {
        temp = temp->child; // <moduleDef>
        currentVarTable = driverVarTable;
        constructST2(temp);
    }
    else if (strcmp(root->name, "otherModules") == 0)
    {
        while (temp->child != NULL)
        {
            constructST2(temp->child);
            temp = temp->child->next;
        }
    }
    else if (strcmp(root->name, "module") == 0)
    {
        // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
        temp = temp->child; // ID

        // printf("MODULE CHECK : %s\n",temp->val.s);

        currentVarTable = (retrieveFunTable(globalFuncTable, temp->val.s))->localVarTable;
        temp = temp->next; // <input_plist>
        temp = temp->next; // <ret>
        temp = temp->next; // <moduleDef>
        constructST2(temp);
    }
    else if (strcmp(root->name, "moduleDef") == 0)
    {
        // <moduleDef>   -->  START <statements> END
        temp = temp->child; // <statement>
        while (temp != NULL)
        {
            // TODO : Start Different Scope!
            constructST2(temp);
            temp = temp->next;
        }
    }
    else if (strcmp(root->name, "statements") == 0)
    {
        // <statements>  -->  <statement> <statements>
        temp = temp->child;
        while (temp != NULL)
        {
            // printf("Statements Check %s\n",temp->name);
            constructST2(temp);
            temp = temp->next;
        }
    }
    else if (strcmp(root->name, "ioStmt1") == 0)
    {
        return;
    }
    else if (strcmp(root->name, "ioStmt2") == 0)
    {
        return;
    }
    else if (strcmp(root->name, "simpleStmt") == 0)
    {
        root = root->child;
        temp = root;
        if (strcmp(root->name, "assignmentStmt") == 0)
        {
            return;
        }
        else if (strcmp(root->name, "moduleReuseStmt") == 0)
        {
            // Second Pass Checks
            // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
            temp = temp->child; // <optional>
            //  TODO : Check if func parameters size and type are correct!

            temp = temp->next; // Function ID
            functionTableEntry *ftemp;
            if (searchInFunTable(globalFuncTable, temp->val.s))
            {
                ftemp = retrieveFunTable(globalFuncTable, temp->val.s);
            }
            else
            {
                // Function Not Found
                printf("Second Pass Unexpected Function %s not Declared\n", temp->val.s);
                return;
            }

            // temp is at Function ID
            temp = temp->next;  //  <idList>
            temp = temp->child; // ID->ID->ID....

            // Check Parameter type of idList matches or not

            if (ftemp->isDefined)
            {
                parameter *inputList = ftemp->inputList;
                while (temp != NULL && inputList != NULL)
                {
                    variableTableEntry *idEntry;
                    if (searchNested(currentVarTable, temp->val.s))
                    {
                        idEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
                    }
                    else
                    {
                        printf("ERROR at line no %d : Identifier %s is not declared in Function Call %s\n", temp->lineno, temp->val.s, ftemp->key);
                        return;
                    }

                    if (inputList->tag == idEntry->tag)
                    {
                        if (inputList->isArray == idEntry->isArray)
                        {
                            // Bound Check if array
                            if (inputList->isArray)
                            {
                                if (inputList->isArrayStatic && idEntry->isArrayStatic)
                                {
                                    if (inputList->lowerBound != idEntry->lowerBound || inputList->upperBound != idEntry->upperBound)
                                    {
                                        // Bounds of parameter array doesn't match
                                        printf("ERROR at line no. %d: Bounds of parameter array %s of function %s are not correct\n", temp->lineno, idEntry->key, ftemp->key);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("ERROR at line no. %d: Call Function %s with the correct parameters\n", temp->lineno, ftemp->key);
                        return;
                    }
                    temp = temp->next;
                    inputList = inputList->next;
                }

                if(temp!=NULL || inputList!=NULL ){
                    printf("ERROR at line no. %d: Call Function %s with the correct number of Input parameters\n", temp->lineno, ftemp->key);
                    return;
                }

                parameter *outputList = ftemp->outputList;
                temp = root->child->child;
                temp = temp->child;
                
                // printf("Check1 line%d %s\n",temp->lineno,temp->name);

                if(outputList==NULL)
                return;

                int tempLineNo = temp->lineno;

                while(temp != NULL && outputList != NULL){
                    // printf("Check2 : line%d %s\n",temp->lineno,temp->val.s);
                    // printf("Check3 : %s %s\n",outputList->key,temp->val.s);
                    variableTableEntry *idEntry;
                    if (searchNested(currentVarTable, temp->val.s))
                    {
                        idEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
                    }
                    else
                    {
                        printf("ERROR at line no %d : Identifier %s is not declared in Function Call %s\n", temp->lineno, temp->val.s, ftemp->key);
                        return;
                    }

                    if (outputList->tag == idEntry->tag)
                    {
                        if (outputList->isArray == idEntry->isArray)
                        {
                            // Bound Check if array
                            if (outputList->isArray)
                            {
                                if (outputList->isArrayStatic && idEntry->isArrayStatic)
                                {
                                    if (outputList->lowerBound != idEntry->lowerBound || outputList->upperBound != idEntry->upperBound)
                                    {
                                        // Bounds of parameter array doesn't match
                                        printf("ERROR at line no. %d: Bounds of parameter array %s of function %s are not correct\n", temp->lineno, idEntry->key, ftemp->key);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("ERROR at line no. %d: Call Function %s with the correct Output parameters\n", temp->lineno, ftemp->key);
                        return;
                    }
                    temp = temp->next;
                    outputList = outputList->next;
                }
                // printf("Check4 \n");

                if(temp!=NULL || outputList!=NULL ){
                    printf("ERROR at line no. %d: Call Function %s with the correct number of Output parameters\n", tempLineNo, ftemp->key);
                    return;
                }

            }
            else
            {
                // Function is not defined!
                printf("ERROR at line no. %d: Function %s was never defined\n", temp->lineno, ftemp->key);
            }

        }
        else
        {
            printf("Unexpected simpleStmt\n");
        }
    }
    else if(strcmp(root->name, "iterativeStmt") == 0)
    {
        // temp = root
        variableTable* tempTable = currentVarTable;
        currentVarTable = currentVarTable->child;
        while(currentVarTable->done==true){

            if(currentVarTable==NULL){
                printf("Unexpected Error ConstructST2\n");
            }

            currentVarTable = currentVarTable->next;
        }

        if (strcmp(root->val.s, "FOR") == 0){
            // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
            temp = temp->child; // ID
            temp = temp->next;  // range
            temp = temp->next; // <statements>
            constructST2(temp);
        }
        else if (strcmp(root->val.s, "WHILE") == 0){
            // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END
            temp = temp->child; // arithmeticOrBooleanExpr
            temp = temp->next; // <statements>
            constructST2(temp);
        }else{
            printf("Unexpected Error ConstructST2\n");
        }
        currentVarTable->done = true;
        currentVarTable = tempTable;

    }
    else if(strcmp(root->name, "conditionalStmt") == 0)
    {

        variableTable* tempTable = currentVarTable;
        currentVarTable = currentVarTable->child;
        while(currentVarTable->done==true){
            currentVarTable = currentVarTable->next;
        }

        temp = temp->child;
        temp = temp->next; // <caseStmts>
        currentVarTable->scopeStart = temp->lineno;
        currentVarTable->scopeEnd = temp->lineno;
        while (temp != NULL)
        {
            temp = temp->child; // <value>
            temp = temp->next; // <statements>
            constructST2(temp);

            temp = temp->next; // N9
        }

        temp = root->child->next->next; // <default> = <statements>

        if(temp!=NULL)
        constructST2(temp);
        currentVarTable->done = true;
        currentVarTable = tempTable;
    }
    else if (strcmp(root->name, "expression") == 0)
    {
        constructST2(root->child);
    }
    else
    {
        return;
    }
}

void constructST(struct astNode *root)
{

    // printf("constructST : %s\n", root->name);

    if (root == NULL)
    {
        return;
        // Report ERROR!
        printf("Root is NULL");
        exit(0);
    }

    struct astNode *temp = root; // Using this for traversal

    if (strcmp(root->name, "program") == 0)
    {
        // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
        temp = temp->child; // <moduleDeclarations>
        constructST(temp);
        otherModPos = 1;
        temp = temp->next; // <otherModules1>
        constructST(temp);
        temp = temp->next; // <driverModule>
        constructST(temp);
        otherModPos = 2;
        temp = temp->next; // <otherModules2>
        constructST(temp); // Treat this in a different manner!

        return; // Not necessary!!
    }
    else if (strcmp(root->name, "moduleDeclarations") == 0)
    {
        // <moduleDeclarations>  -->  <moduleDeclaration><moduleDeclarations>
        // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL
        // <moduleDeclarations>  -->  ε

        temp = temp->child; // ID or NULL

        while (temp != NULL)
        {
            // DONE - Wil get Module ID from here!
            char funcName[40];
            strcpy(funcName, temp->val.s);
            //printf("%s\n",funcName);

            if (strcmp("ε", funcName) != 0)
            {
                if (searchInFunTable(globalFuncTable, funcName))
                    printf("ERROR at line no. %d: Redeclaration of function %s\n", temp->lineno, funcName);
                else
                {
                    insertInFunTable(globalFuncTable, funcName, NULL, NULL);
                    updateDefineBool(globalFuncTable, funcName, false);
                }
            }
            //printFunTable(globalFuncTable); // TODO - take care of input and output list
            //TODO - redeclaration error
            temp = temp->next; // ID or NULL;
        }
        //printFunTable(globalFuncTable);
        return;
    }
    else if (strcmp(root->name, "driverModule") == 0)
    {
        temp = temp->child; // <moduleDef>
        // <driverModule> -->  DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
        // <moduleDef>   -->  START <statements> END
        // <statements>  -->  <statement> <statements>
        // ...

        //DONE
        //global_offset = 0;
        currentVarTable = driverVarTable;
        constructST(temp); // if for moduleDef written below!
    }

    else if (strcmp(root->name, "otherModules") == 0)
    {
        // <otherModules>   -->  ε
        // <otherModules>   -->  <module> <otherModules>
        while (temp->child != NULL)
        {
            constructST(temp->child); // module
            temp = temp->child->next; // otherModules
        }
    }
    else if (strcmp(root->name, "module") == 0)
    {
        // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>

        temp = temp->child; // ID
        char funcName[40];
        strcpy(funcName, temp->val.s);

        if (otherModPos == 1)
        {
            //TODO - Redeclaration
            if (searchInFunTable(globalFuncTable, funcName))
            {
                // f1 func defin
                // f1 func defin  ERROR
                functionTableEntry *ftemp = retrieveFunTable(globalFuncTable, funcName);
                if (ftemp->isDefined)
                {
                    printf("ERROR at line no. %d: Redefinition of function %s\n", temp->lineno, funcName);
                    return; // Re-defined Function Skipped
                }
                else
                {
                    updateDefineBool(globalFuncTable, funcName, true);
                }

                // TODO1 : Tell Symbol Table that this function was first came by definition
                // f1 declare
                // f1 func defin NOT ERROR
            }
            //DONE - Simply Add to Global Func Table
            //printf("%s\n",funcName);
            else
            {
                insertInFunTable(globalFuncTable, funcName, NULL, NULL); // TODO - check for errors, take care of input and output lists
                updateDefineBool(globalFuncTable, funcName, true);
            }
            //printFunTable(globalFuncTable);
        }
        else if (otherModPos == 2)
        {
            // DONE - Check if definition was prev there or not! If not, then error will be printed!
            if (!searchInFunTable(globalFuncTable, funcName))
            {
                printf("ERROR at line no. %d: Function %s Not Declared Before Driver.\n", temp->lineno, funcName);
                insertInFunTable(globalFuncTable, funcName, NULL, NULL);
                updateDefineBool(globalFuncTable, funcName, true);
            }
            else
            {
                functionTableEntry *ftemp = retrieveFunTable(globalFuncTable, funcName);
                if (ftemp->isDefined)
                {
                    printf("ERROR: On line number %d , Redefinition of function %s\n", temp->lineno, funcName);
                    return; // Re-defined Function Skipped
                }
                else
                {
                    updateDefineBool(globalFuncTable, funcName, true);
                }
            }
            //printf("%s\n",funcName);
            //printFunTable(globalFuncTable);
        }

        // DONE: currentVarTable = current var table of curr Function!
        currentVarTable = (retrieveFunTable(globalFuncTable, funcName))->localVarTable;
        //printVarTable(currentVarTable);
        global_offset = 0;
        temp = temp->next; // <input_plist>
        // Code for input_plist
        // <input_plist>  -->  ID COLON <dataType><N1>

        temp = temp->child; // First ID
        char idName[40];
        strcpy(idName, temp->val.s);
        //printf("INPUTINPUTINPUT - %s\n",idName);
        // TODO:  Add this ID = temp->child to function table (input)  with type at temp->next
        temp = temp->next;            // <dataType>
        struct astNode *temp2 = temp; // <N1>
        // Datatype

        if (!checkIfArrayType(temp))
        { //NON ARRAY
            // temp->tag;
            insertInVarTable(currentVarTable, idName, false, temp->tag, 0); // doing for function, nesting level will always be zero. // TODO - check for errors
            updateIsInput(currentVarTable, idName, true);
            // DONE : Insert Offset
            updateOffset(currentVarTable, idName, global_offset);
            global_offset += retrieveWidth(currentVarTable, idName);

            functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
            ft->inputList = initializeParameter(idName, false, temp->tag);
            //printParameterList(ft->inputList);
            //printVarTable(currentVarTable);
        }
        else
        { //ARRAY
            // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
            // <range_arrays>  -->  <index> RANGEOP <index>
            temp2 = temp2->child; // <range_arrays>
            int type = temp2->next->tag;
            int low, high;
            char lowId[40], highId[40];

            // printf("Check51 : %d!\n",temp->tag);

            temp2 = temp2->child;

            if (temp2->tag == 1)
            { //static
                low = temp2->val.i;
                high = temp2->next->val.i;
                //printf("low = %d , high = %d \n",low,high);
                insertInVarTable(currentVarTable, idName, true, type, 0);
                updateArrayVarStatic(currentVarTable, idName, low, high);
                updateIsInput(currentVarTable, idName, true);

                functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                ft->inputList = initializeParameter(idName, true, type);
                updateParameterArrayStatic(ft->inputList, low, high);

                updateOffset(currentVarTable, idName, global_offset);
                global_offset += retrieveWidth(currentVarTable, idName);
            }
            else
            { //dynamic
                strcpy(lowId, temp2->val.s);
                strcpy(highId, temp2->next->val.s);

                insertInVarTable(currentVarTable, idName, true, type, 0);
                updateArrayVarDynamic(currentVarTable, idName, lowId, highId);
                updateIsInput(currentVarTable, idName, true);

                functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                ft->inputList = initializeParameter(idName, true, type);
                updateParameterArrayDynamic(ft->inputList, lowId, highId);

                updateOffset(currentVarTable, idName, global_offset);
                global_offset += retrieveWidth(currentVarTable, idName);
            }
        }

        //printVarTable(currentVarTable);

        temp = temp->next; // <N1> = ID <dataType> ID <dataType> ...
        // <N1>  -->  COMMA ID COLON <dataType> <N1>
        while (temp != NULL)
        {
            // temp = ID
            //printf("VALUE N1 - %s \n",temp->val.s);
            strcpy(idName, temp->val.s);

            if (searchInVarTable(currentVarTable, idName))
            {
                printf("ERROR: On line number-%d, Variable Name %s has already been used in input list.", temp->lineno, idName);
                temp = temp->next->next;
                continue;
            }
            // Redeclaration ERROR - Variable already defined!

            // DONE:  Add this ID = temp->child to function table (input)  with type at temp->next
            temp = temp->next; // <dataType>
            temp2 = temp;

            if (!checkIfArrayType(temp))
            {
                // temp->tag;
                insertInVarTable(currentVarTable, idName, false, temp->tag, 0); // doing for function, nesting level will always be zero. // TODO - check for errors
                updateIsInput(currentVarTable, idName, true);
                // TODO : Insert Offset
                functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                parameter *ptemp = initializeParameter(idName, false, temp->tag);
                addParametertoList(ft->inputList, ptemp);

                updateOffset(currentVarTable, idName, global_offset);
                global_offset += retrieveWidth(currentVarTable, idName);
                //printVarTable(currentVarTable);
            }
            else
            {
                temp2 = temp2->child; // <range_arrays>
                int type = temp2->next->tag;
                int low, high;
                char lowId[40], highId[40];

                // printf("Check51 : %d!\n",temp->tag);

                temp2 = temp2->child;

                if (temp2->tag == 1)
                {
                    low = temp2->val.i;
                    high = temp2->next->val.i;
                    //printf("low = %d , high = %d \n",low,high);
                    insertInVarTable(currentVarTable, idName, true, type, 0);
                    // TODO : Insert Offset
                    updateArrayVarStatic(currentVarTable, idName, low, high);
                    updateIsInput(currentVarTable, idName, true);

                    functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                    parameter *ptemp = initializeParameter(idName, true, type);
                    updateParameterArrayStatic(ptemp, low, high);
                    addParametertoList(ft->inputList, ptemp);

                    updateOffset(currentVarTable, idName, global_offset);
                    global_offset += retrieveWidth(currentVarTable, idName);
                }
                else
                {
                    strcpy(lowId, temp2->val.s);
                    strcpy(highId, temp2->next->val.s);

                    insertInVarTable(currentVarTable, idName, true, type, 0);
                    // TODO : Insert Offset
                    updateArrayVarDynamic(currentVarTable, idName, lowId, highId);
                    updateIsInput(currentVarTable, idName, true);

                    functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                    parameter *ptemp = initializeParameter(idName, true, type);
                    updateParameterArrayDynamic(ptemp, lowId, highId);
                    addParametertoList(ft->inputList, ptemp);

                    updateOffset(currentVarTable, idName, global_offset);
                    global_offset += retrieveWidth(currentVarTable, idName);
                }
            }
            temp = temp->next; // N1 or NULL
        }

        //printVarTable(currentVarTable);
        // Code for input_plist ends

        // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
        temp = root->child;      // ID
        temp = temp->next->next; // <ret>

        if (temp->child != NULL)
        {

            temp = temp->child; // ID
            char idName[40];
            strcpy(idName, temp->val.s);

            if (searchInVarTable(currentVarTable, idName))
            {
                printf("ERROR at line no. %d: Variable Name %s has already been used in input list.", temp->lineno, idName);
            }

            //printf("OUTPUT VAR - %s\n",temp->val.s);

            // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
            // printf("Check9 : %s\n",temp->name);
            temp = temp->next; // <type>
            temp2 = temp;

            if (!checkIfArrayType(temp))
            {
                insertInVarTable(currentVarTable, idName, false, temp->tag, 0);

                functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                ft->outputList = initializeParameter(idName, false, temp->tag);

                updateOffset(currentVarTable, idName, global_offset);
                global_offset += retrieveWidth(currentVarTable, idName);
                // TODO : Insert Offset
                // doing for function, nesting level will always be zero. // TODO - check for errors
            }
            else
            {
                temp2 = temp2->child; // <range_arrays>
                int type = temp2->next->tag;
                int low, high;
                char lowId[40], highId[40];

                // printf("Check51 : %d!\n",temp->tag);

                temp2 = temp2->child;

                if (temp2->tag == 1)
                {
                    low = temp2->val.i;
                    high = temp2->next->val.i;
                    //printf("low = %d , high = %d \n",low,high);
                    insertInVarTable(currentVarTable, idName, true, type, 0);
                    // TODO : Insert Offset
                    updateArrayVarStatic(currentVarTable, idName, low, high);

                    functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                    ft->outputList = initializeParameter(idName, true, type);
                    updateParameterArrayStatic(ft->outputList, low, high);
                }

                else
                {
                    strcpy(lowId, temp2->val.s);
                    strcpy(highId, temp2->next->val.s);

                    insertInVarTable(currentVarTable, idName, true, type, 0);
                    // TODO : Insert Offset
                    updateArrayVarDynamic(currentVarTable, idName, lowId, highId);

                    functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                    ft->outputList = initializeParameter(idName, true, type);
                    updateParameterArrayDynamic(ft->outputList, lowId, highId);
                }
            }

            temp = temp->next; // <N2> = ID <type> ID <type> ...
            while (temp != NULL)
            {
                // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
                // temp is ID
                strcpy(idName, temp->val.s);
                temp = temp->next; // <type>
                temp2 = temp;

                if (searchInVarTable(currentVarTable, idName))
                {
                    printf("ERROR at line no. %d: Variable Name %s has already been used in input list.", temp->lineno, idName);
                    temp = temp->next->next;
                    continue;
                }

                if (!checkIfArrayType(temp))
                {
                    insertInVarTable(currentVarTable, idName, false, temp->tag, 0);

                    functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                    parameter *ptemp = initializeParameter(idName, false, temp->tag);
                    addParametertoList(ft->outputList, ptemp);

                    updateOffset(currentVarTable, idName, global_offset);
                    global_offset += retrieveWidth(currentVarTable, idName);
                    // TODO : Insert Offset
                    // doing for function, nesting level will always be zero. // TODO - check for errors
                }

                else
                {
                    temp2 = temp2->child; // <range_arrays>
                    int type = temp2->next->tag;
                    int low, high;
                    char lowId[40], highId[40];

                    // printf("Check51 : %d!\n",temp->tag);

                    temp2 = temp2->child;

                    if (temp2->tag == 1)
                    {
                        low = temp2->val.i;
                        high = temp2->next->val.i;
                        //printf("low = %d , high = %d \n",low,high);
                        insertInVarTable(currentVarTable, idName, true, type, 0);
                        // TODO : Insert Offset
                        updateArrayVarStatic(currentVarTable, idName, low, high);

                        functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                        parameter *ptemp = initializeParameter(idName, true, type);
                        updateParameterArrayStatic(ptemp, low, high);
                        addParametertoList(ft->outputList, ptemp);
                    }

                    else
                    {
                        strcpy(lowId, temp2->val.s);
                        strcpy(highId, temp2->next->val.s);

                        insertInVarTable(currentVarTable, idName, true, type, 0);
                        // TODO : Insert Offset
                        updateArrayVarDynamic(currentVarTable, idName, lowId, highId);

                        functionTableEntry *ft = retrieveFunTable(globalFuncTable, funcName);
                        parameter *ptemp = initializeParameter(idName, true, type);
                        updateParameterArrayDynamic(ptemp, lowId, highId);
                        addParametertoList(ft->outputList, ptemp);
                    }
                }

                temp = temp->next; // ID or NULL
            }
            // printVarTable(currentVarTable);
            // Code for output_plist ends
        }

        temp = root->child;            // ID
        temp = temp->next->next->next; // <moduleDef>
        constructST(temp);
    }
    else if (strcmp(root->name, "moduleDef") == 0)
    {
        // <moduleDef>   -->  START <statements> END
        currentVarTable->scopeStart = temp->lineno;
        currentVarTable->scopeEnd = temp->lineno;
        temp = temp->child; // <statement>
        global_offset = 0;
        while (temp != NULL)
        {
            constructST(temp);
            temp = temp->next;
        }
    }
    else if (strcmp(root->name, "expression") == 0)
    {
        constructST(root->child);
    }
    else if (strcmp(root->name, "statements") == 0)
    {
        // <statements>  -->  <statement> <statements>
        temp = temp->child;
        while (temp != NULL)
        {
            // printf("Statements Check %s\n",temp->name);
            currentVarTable->scopeEnd = temp->lineno;
            constructST(temp);
            temp = temp->next;
        }
    }
    else if (strcmp(root->name, "ioStmt1") == 0)
    {
        // Input Stmt
        // Nothing much with Symbol Table

        temp = temp->child; // ID
        if (strcmp(temp->name, "ID") == 0)
        {
            if(!searchNested(currentVarTable,temp->val.s))
            printf("ERROR at line no. %d : Identifier %s not declared\n",temp->lineno, temp->val.s);

            return;
        }
            // TODO:  Check if ID exists in var table! If not, give error!
            // <ioStmt>  -->  GET_VALUE BO ID BC SEMICOL
            // ID will already be there in symbol table!
            return;
    }
    else if (strcmp(root->name, "ioStmt2") == 0)
    {
        // Output Stmt
        // Nothing much with Symbol Table

        return;
    }
    else if (strcmp(root->name, "simpleStmt") == 0)
    {

        root = root->child; // assignmentStmt or moduleReuseStmt
        temp = root;
        if (strcmp(root->name, "assignmentStmt") == 0)
        {
            // <assignmentStmt>   -->   ID <whichStmt>

            temp = temp->child; // ID - type aur exp match !
            

            variableTableEntry *idEntry;

            // Not-Declared ERROR Check
            if (searchNested(currentVarTable, temp->val.s))
            {
                idEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
            }
            else
            {
                printf("ERROR at line no. %d: Identifier %s undeclared\n", temp->lineno, temp->val.s);
                return;
            }

            // printf("Check Assign %s %d - Type %d\n",temp->val.s,temp->lineno,idEntry->tag);
            // <whichStmt>  -->  <lvalueIDStmt>
            // <whichStmt>  -->  <lvalueARRStmt>
            temp = temp->next; // either of these two

            if (strcmp(temp->name, "lvalueIDStmt") == 0)
            {
                temp = temp->child; // ASSIGNOP
                temp = temp->next;  // expression
                temp = temp->child;
                int expType = getType(temp);

                // printf("Check1 : line%d  - %d , %d\n",temp->lineno,idEntry->tag,expType);
                
                if (expType <= -1)
                {
                    if(expType==-1)
                    printf("ERROR at line no. %d: Type Mismatch\n", temp->lineno);

                    return;
                }

                if (idEntry->isArray)
                {
                    if (expType >= 4)
                    {
                        // RHS ID is an Array of type = (expType-3)
                        // A = B - both are arrays
                        // printf("Check2 : line%d  - %d,%d\n",temp->lineno,expType,idEntry->tag);
                        if (idEntry->tag == (expType - 3))
                        {
                            // Type of Array A and B are same
                            // Bound checking
                            // Here A = B
                            if (idEntry->isArrayStatic)
                            {
                                // printf("Check3 : line%d  - %d\n",temp->lineno,expType);
                                if (searchNested(currentVarTable, idEntry->key))
                                {
                                    // printf("Check4 : line%d  - %d\n",temp->lineno,expType);
                                    variableTableEntry *idRHSEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
                                    if (idRHSEntry->isArrayStatic)
                                    {
                                        // printf("Check5 : line%d %d %d\n",temp->lineno,idEntry->lowerBound,idEntry->upperBound);
                                        // printf("Check5 : line%d %d %d\n",temp->lineno,idRHSEntry->lowerBound,idRHSEntry->upperBound);

                                        if (idEntry->lowerBound != idRHSEntry->lowerBound || idEntry->upperBound != idRHSEntry->upperBound)
                                        {
                                            // Bounds of Static Arrays doesn't match
                                            printf("ERROR at line no. %d: Bounds of Array %s doesn't match with Array %s\n", temp->lineno, idEntry->key, idRHSEntry->key);
                                            return;
                                        }
                                    }
                                }
                                return;
                            }
                        }
                        else
                        {
                            // Type of Array A and B are not same
                            printf("ERROR at line no. %d: ARRAY %s cannot be assigned to a different type\n", temp->lineno, idEntry->key);
                            return;
                        }
                    }
                    else
                    {
                        // RHS ID is not an array type
                        if(expType>=-1)
                        printf("ERROR at line no. %d: ARRAY %s can only perform assignment operation with an ARRAY of same type\n", temp->lineno, idEntry->key);
                        return;
                    }
                }
                else
                {
                    // Normal Type Checking between Ids!
                    if (expType != idEntry->tag)
                    {
                        // LHS RHS Type Mismatch
                        if(expType>=-1)
                        printf("ERROR at line no. %d: Type Mismatch\n", temp->lineno);
                    }
                    return;
                }
            }
            else if (strcmp(temp->name, "lvalueARRStmt") == 0)
            {
                // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
                temp = temp->child; // index

                if (temp->tag == 1)
                {
                    // Check Bounds if array static
                    if (idEntry->isArrayStatic)
                    {
                        if (temp->val.i < idEntry->lowerBound || temp->val.i > idEntry->upperBound)
                        {
                            // Out of Bounds
                            printf("ERROR at line no. %d: Array Index Out of Bounds for %s\n",root->child->lineno,idEntry->key);
                            return;
                        }
                    }
                }
                else if (temp->tag == 4)
                {
                    // Dynamic index!
                    if (searchNested(currentVarTable, temp->val.s))
                    {
                        variableTableEntry *indexEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
                        if (indexEntry->tag != 1)
                        {
                            printf("ERROR at line no. %d : Array index %s is not an Integer type\n",temp->lineno,temp->val.s);
                            return;
                        }
                    }
                    else
                    {
                        printf("ERROR at line no. %d : Index %s not declared\n",temp->lineno, temp->val.s);
                        return;
                    }
                }
                else
                {
                    printf("Unexpected index type\n");
                }

                temp = temp->next; // ASSIGNOP
                temp = temp->next; // expression
                temp = temp->child;
                int expType = getType(temp);

                // A[i] = expression!
                if (idEntry->tag != expType)
                {
                    // LHS RHS Type Mismatch
                    if(expType>=-1)
                    printf("ERROR at line no. %d: Type Mismatch\n", temp->lineno);
                }
                return;
            }
            else
            {
                printf("Unexpected ERROR at line no. %d (at whichStmt) : %s\n", temp->lineno, temp->name);
                return;
            }
        }

        if (strcmp(root->name, "moduleReuseStmt") == 0)
        {
            // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL

            temp = temp->child; // optional
            //  TODO : Check if func parameters size and type are correct!

            temp = temp->next; // Function ID

            // To handle Recursion

            if(strcmp(temp->val.s,currentVarTable->funcName)==0){
                printf("ERROR at line no. %d: Function %s cannot call itself(Recursion Not Allowed)\n",temp->lineno,temp->val.s);
                return;
            }


            functionTableEntry *ftemp;
            // TODO : This is the function ID
            // Function Calling statement
            if (searchInFunTable(globalFuncTable, temp->val.s))
            {
                ftemp = retrieveFunTable(globalFuncTable, temp->val.s);
            }
            else
            {
                // Function Not Found
                printf("ERROR at line no. %d: Function %s is not declared\n", temp->lineno, temp->val.s);
                return;
            }

            // printf("Check1 : %d %s\n",temp->lineno,temp->val.s);

            temp = temp->next;  // idlist
            temp = temp->child;

            // printf("Check2 : %d %s\n",temp->lineno,temp->val.s);

            while (temp != NULL)
            {
                // printf("Check3 : %d %s\n",temp->lineno,temp->val.s);
                if (!searchNested(currentVarTable, temp->val.s)){
                    printf("ERROR at line no %d : Identifier %s is not declared in Function Call %s\n", temp->lineno, temp->val.s, ftemp->key);
                    return;
                }
                temp = temp->next;
            }

            temp = root->child; // optional
            temp = temp->child; // idlist
            temp = temp->child;

            // printf("Check3 : %d %s\n",temp->lineno,temp->val.s);

            while (temp != NULL)
            {
                // printf("Check3 : %d %s\n",temp->lineno,temp->val.s);
                variableTableEntry *idEntry;
                if (!searchNested(currentVarTable, temp->val.s)){
                    printf("ERROR at line no %d : Identifier %s is not declared in Function Call %s\n", temp->lineno, temp->val.s, ftemp->key);
                    return;
                }
                temp = temp->next;
            }

            // We are not sure at this point that function was ever defined!
        }
    }

    else if (strcmp(root->name, "declareStmt") == 0)
    {
        // return;
        // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
        // TODO : Get dataType from right <dataType> and set Ids type to that! ADD to Symbol Table(Nothing to add here)

        temp = temp->child; // <idList>
        temp = temp->next;  // <dataType>
        int datatype;

        if (temp->child == NULL)
        {
            // Not an Array
            datatype = temp->tag;
            //printf(" DECLARE - %d\n",datatype);
            /*
                1 - INTEGER
                2 - REAL/FLOAT
                3 - BOOLEAN
            */
            temp = root->child; // <idList>
            //<idList>  -->  ID <N3>
            temp = temp->child; // ID
            while (temp != NULL)
            {

                // DONE: Now temp is a linked list of IDs - Add them to Symbol Table
                //printf("DECLARE - IDS - %s\n",temp->val.s);
                char varName[40];
                strcpy(varName, temp->val.s);

                if(searchInVarTable(currentVarTable,varName))
                {
                    variableTableEntry* vptr = retrieveVarTable(currentVarTable,varName);
                    if(vptr->isInput)
                    {
                        vptr->isEmpty = true;
                        functionTableEntry* fptr = retrieveFunTable(globalFuncTable, currentVarTable->funcName);
                        parameter* traverse1 = fptr->inputList;
                        while(traverse1!=NULL)
                        {
                            if(strcmp(traverse1->key,varName)==0)
                            {
                                traverse1->isRedifined = true;
                                traverse1->offset = vptr->offset;
                                traverse1->width = vptr->width;
                                break;
                            }

                            traverse1 = traverse1->next;
                        }
                    }
                    else
                    {
                        printf("ERROR at line no. %d: Redeclaration of variable %s\n", temp->lineno, varName);
                        temp = temp->next;
                        continue;
                    }
                }
                
                insertInVarTable(currentVarTable, varName, false, datatype, globalNestingLevel); // TODO - Check for errors
                
                updateOffset(currentVarTable, varName, global_offset);
                global_offset += retrieveWidth(currentVarTable, varName);

                temp = temp->next;
            }
        }
        else
        {
            // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
            // <range_arrays>  -->  <index> RANGEOP <index>
            temp = temp->child; // <range_arrays>  -->  <index> RANGEOP <index>

            datatype = temp->next->tag; // <type> -> Array datatype

            temp = temp->child; // <index>
            int low_tag, high_tag;
            int low, high;
            char lowId[40], highId[40];

            low_tag = temp->tag; // 1 or 4
            high_tag = temp->next->tag;

            // If 4, it's an ID
            // If 1, it's an Integer
            if (low_tag == 1)
            {
                low = temp->val.i;
            }
            else
            {
                strcpy(lowId, temp->val.s);
            }

            if (high_tag == 1)
            {
                high = temp->next->val.i;
            }
            else
            {
                strcpy(highId, temp->next->val.s);
            }

            // TODO : How to handle dynamic and static arrays?

            temp = root->child; // <idList>
            //<idList>  -->  ID <N3>
            temp = temp->child; // ID

            while (temp != NULL)
            {

                // DONE: Now temp is a linked list of IDs - Add them to Symbol Table
                char varName[40];
                strcpy(varName, temp->val.s);

                if(searchInVarTable(currentVarTable,varName))
                {
                    variableTableEntry* vptr = retrieveVarTable(currentVarTable,varName);
                    if(vptr->isInput)
                    {
                        vptr->isEmpty = true;

                        functionTableEntry* fptr = retrieveFunTable(globalFuncTable, currentVarTable->funcName);
                        parameter* traverse1 = fptr->inputList;
                        while(traverse1!=NULL)
                        {
                            if(strcmp(traverse1->key,varName)==0)
                            {
                                traverse1->isRedifined = true;
                                traverse1->offset = vptr->offset;
                                traverse1->width = vptr->width;
                                break;
                            }

                            traverse1 = traverse1->next;
                        }
                    }
                    else
                    {
                        printf("ERROR at line no. %d: Redeclaration of variable %s\n", temp->lineno, varName);
                        temp = temp->next;
                        continue;
                    }
                }

                if (low_tag == 1) // STATIC ARRAY
                {
                    insertInVarTable(currentVarTable, varName, true, datatype, globalNestingLevel);
                    updateArrayVarStatic(currentVarTable, varName, low, high);

                    updateOffset(currentVarTable, varName, global_offset);
                    global_offset += retrieveWidth(currentVarTable, varName);
                }

                if (low_tag == 4)
                {
                    insertInVarTable(currentVarTable, varName, true, datatype, globalNestingLevel);
                    updateArrayVarDynamic(currentVarTable, varName, lowId, highId);

                    updateOffset(currentVarTable, varName, global_offset);
                    global_offset += retrieveWidth(currentVarTable, varName);
                }
                temp = temp->next;
            }

            // <type>  -->  INTEGER
            // <type>  -->  REAL
            // <type>  -->  BOOLEAN
        }

        //printVarTable(currentVarTable);

    } // declareStmt Ends

    else if (strcmp(root->name, "conditionalStmt") == 0)
    {
        // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
        // DONE : Create another var table for conditionalStmt

        variableTable *tempTable = currentVarTable;
        globalNestingLevel++;
        if (tempTable->child == NULL)
        {

            variableTable *newTable = initializeVarTable();
            newTable->parent = tempTable;
            tempTable->child = newTable;
            strcpy(newTable->funcName,tempTable->funcName);
            currentVarTable = newTable;
        }
        else
        {

            variableTable *traverse = tempTable->child;

            while (traverse->next != NULL)
                traverse = traverse->next;

            variableTable *newTable = initializeVarTable();
            traverse->next = newTable;
            newTable->parent = tempTable;
            strcpy(newTable->funcName,tempTable->funcName);
            currentVarTable = newTable;
        }
        // <caseStmts>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>

        temp = temp->child;
        temp = temp->next; // <caseStmts>
        currentVarTable->scopeStart = temp->lineno;
        currentVarTable->scopeEnd = temp->lineno;
        while (temp != NULL)
        {
            // printf("Case Check %s\n",temp->name);
            temp = temp->child; // <value>
            // <value>  -->  NUM
            // <value>  -->  TRUE
            // <value>  -->  FALSE
            // TODO : Check on Value Type Consistent - NUM/Boolean
            // TODO : Check if Boolean Value Type then no Default Stmt?
            temp = temp->next; // <statements>
            currentVarTable->scopeEnd = temp->lineno;
            constructST(temp);

            temp = temp->next; // N9
            // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
            // <N9>  -->  ε
        }

        temp = root->child->next->next; // <default> = <statements>

        // printf("Check default : %d %s\n",temp->lineno,temp->name);

        if(temp!=NULL)
        {
        constructST(temp);
        currentVarTable->scopeEnd = temp->lineno;
        }
        // <default>  -->  DEFAULT COLON <statements> BREAK SEMICOL
        // <default>  -->  ε

        // DONE : Change Scope!
        currentVarTable = tempTable;
        globalNestingLevel--;
    } // conditionalStmt Ends

    else if (strcmp(root->name, "iterativeStmt") == 0)
    {

        // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
        // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END

        variableTable *tempTable = currentVarTable;
        globalNestingLevel++;
        if (tempTable->child == NULL)
        {
            variableTable *newTable = initializeVarTable();
            newTable->parent = tempTable;
            tempTable->child = newTable;
            strcpy(newTable->funcName,tempTable->funcName);
            currentVarTable = newTable;
        }
        else
        {
            variableTable *traverse = tempTable->child;

            while (traverse->next != NULL)
                traverse = traverse->next;

            variableTable *newTable = initializeVarTable();
            traverse->next = newTable;
            newTable->parent = tempTable;
            strcpy(newTable->funcName,tempTable->funcName);
            currentVarTable = newTable;
        }

        if (strcmp(root->val.s, "FOR") == 0)
        {

            // printf("FOR STARTS\n");
            temp = temp->child; // <ID>

            if (searchNested(currentVarTable, temp->val.s))
            {
                variableTableEntry *idEntry = searchNestedRetrieve(currentVarTable, temp->val.s);
                if (idEntry->tag != 1 || idEntry->isArray)
                {
                    printf("ERROR at line %d : Identifier %s is not an Integer(For Loop Variable) \n", temp->lineno, temp->val.s);
                }
            }
            else
            {
                printf("ERROR at line %d : Identifier %s not declared\n", temp->lineno, temp->val.s);
            }

            // TODO : ID for iterativeStmt
            currentVarTable->scopeStart = temp->lineno;
            currentVarTable->scopeEnd = temp->lineno;
            temp = temp->next; // <range>
            // <range>  -->  NUM1 RANGEOP NUM2
            int low, high;
            temp = temp->child;

            low = temp->val.i;        // NUM1
            high = temp->next->val.i; // NUM2

            temp = root->child; // <ID>
            temp = temp->next;  // <range>
            temp = temp->next;  // <statements>

            constructST(temp);
        }

        else if (strcmp(root->val.s, "WHILE") == 0)
        {
            temp = temp->child; // <arithmeticOrBooleanExpr>
            // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END

            int condType = getType(temp);

            if (condType != 3)
            {
                printf("Check Type : %d\n",condType);
                printf("ERROR at line %d : WHILE Loop Condition should consist of boolean expression\n", temp->lineno);
            }

            currentVarTable->scopeStart = temp->lineno;
            currentVarTable->scopeEnd = temp->lineno;

            temp = temp->next; // <statements>

            constructST(temp);
        }
        else
        {
            printf("Unexpected iterativeStmt ERROR at line no. %d\n", root->lineno);
        }

        // DONE : Change Scope!
        //printVarTable(currentVarTable);
        currentVarTable = tempTable;
        globalNestingLevel--;
    } // iterativeStmt Ends
}

void runConstructST(FILE *testFile, FILE *parseTreeFile)
{
    driverVarTable = initializeVarTable();
    char ttemp[30] = "driver";
    strcpy(driverVarTable->funcName,ttemp);
    globalFuncTable = initializeFunTable();
    currentVarTable = driverVarTable;
    //printFunTable(globalFuncTable);

    // Same as runAst
    parserFree();

    FILE *fp = fopen("grammar.txt", "r");

    readGrammerTextFile(fp);

    // printAllGrammar();

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

    // inOrderParseTree(rootParseTree,parseTreeFile);

    struct astNode *rootAstNode = generateAST(rootParseTree);

    // runAst()

    printf("\nGenerating Symbol Table\n");

    constructST(rootAstNode);
    constructST2(rootAstNode);

    printWidth(globalFuncTable);
}

void freeSymbolTable()
{
    free(globalFuncTable);
    free(driverVarTable);
    
    global_offset = 0;
    globalNestingLevel = 0;
}