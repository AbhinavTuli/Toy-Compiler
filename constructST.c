#include "ast.h"
#include "symbolTableDef.h"
#include "symbolTable.h"
#include "constructST.h"

functionTable* globalFuncTable;
variableTable* currentVarTable;
variableTable* driverVarTable;
int globalNestingLevel = 0;

struct expNode* expRoot;    // Expression Node

// For generateAB_Expression
char tempStr[40];
arr_index tempArrIndex;

int otherModPos;

bool checkIfArrayType(struct astNode* node){

    if(node->child==NULL)
    return false;
    else{
        return true;
    }
}

struct expNode* makeExpNode(int tag,char* name,bool isDynamic,arr_index index){
    struct expNode* node = (struct expNode*) malloc(sizeof(struct expNode));
    node->tag = tag;

    if(tag==4 || tag==0) // id or op
    strcpy(node->name,name);

    if(tag==5){
        node->isDynamic = isDynamic;

        if(isDynamic==false)
        node->index.i = index.i;
        else
        strcpy(node->index.s,index.s);
    }

    node->next = NULL;

    return node;
}

struct expNode* generateExpression(struct astNode* temp){

    struct astNode* root = temp;
    printf("GeneratingExpression : %s\n",root->name);

    if(temp==NULL){
         // Report Error!
        printf("generateExpression : temp is NULL\n");
        exit(0);
    }

    struct expNode* exp;

    if(strcmp(root->name,"assignmentStmt")==0){
        // <assignmentStmt>   -->   ID <whichStmt>
        // <whichStmt>  -->  <lvalueIDStmt> 
        // <whichStmt>  -->  <lvalueARRStmt>
        temp = temp->child;
        strcpy(tempStr,temp->val.s);    // ID 


        if(strcmp(temp->next->name,"lvalueIDStmt")==0){
            // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
            exp = makeExpNode(4,tempStr,false,tempArrIndex);
            temp = temp->next;  // <whichStmt> = <lvalueIDStmt>
            exp->next = generateExpression(temp);
        } 
        else{
            // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
            temp = temp->child; // <lvalueARRStmt>
            temp = temp->child; //  <index>

            if(temp->next->tag==1){
                    tempArrIndex.i = temp->val.i;    // Static Array
                    exp = makeExpNode(5,tempStr,false,tempArrIndex);
            }
            else{
                // Dynamic Array
                strcpy(tempArrIndex.s,temp->val.s); // index ID
                exp = makeExpNode(5,tempStr,true,tempArrIndex);
            }   

            temp = temp->next;  // ASSIGNOP
            strcpy(tempStr,temp->val.s);
            exp = makeExpNode(0,tempStr,false,tempArrIndex);

            temp = temp->next; // <expression>
            exp->next = generateExpression(temp);
        }
    }

    if(strcmp(root->name,"lvalueIDStmt")==0){
        // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
        temp = temp->child; // ASSIGNOP
        strcpy(tempStr,temp->val.s);
        exp = makeExpNode(0,tempStr,false,tempArrIndex);

        temp = temp->next; // <expression>
        exp->next = generateExpression(temp);
    }

    if(strcmp(root->name,"U")==0){
        temp = temp->child;  // unary_op
        strcpy(tempStr,temp->val.s);
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp=temp->next;  //  new_NT
        exp->next = generateExpression(temp);
    }

    if(strcmp(root->name,"arithmeticOrBooleanExpr")==0){
        temp = temp->child;
        exp = generateExpression(temp); // Anyterm
        temp = temp->next;  
        if(temp!=NULL)
        exp->next = generateExpression(temp);   // N7
    }

    if(strcmp(root->name,"N7")==0){
        temp = temp->child; 
        strcpy(tempStr,temp->val.s); // logicalOp
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;  
        exp->next = generateExpression(temp); // AnyTerm
        temp=temp->next;
        if(temp!=NULL)
        exp->next->next = generateExpression(temp); // N7
    }

    if(strcmp(root->name,"boolConstt")==0){
        exp = makeExpNode(3,NULL,false,tempArrIndex);
    }

    if(strcmp(root->name,"AnyTerm")==0){
        temp = temp->child; // arithmeticExpr
        exp = generateExpression(temp);
        temp = temp->next;  
        if(temp!=NULL)
        exp->next = generateExpression(temp); // N8
    }

    if(strcmp(root->name,"arithmeticExpr")==0){
        temp = temp->child; // term
        exp = generateExpression(temp);
        temp = temp->next;  
        if(temp!=NULL)
        exp->next = generateExpression(temp); // N4
    }

    if(strcmp(root->name,"N4")==0){
        temp = temp->child; // op1
        strcpy(tempStr,temp->val.s); // op1
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;  
        exp->next = generateExpression(temp); // term
        temp=temp->next;
        if(temp!=NULL)
        exp->next->next = generateExpression(temp); // N4
    }

    if(strcmp(root->name,"N8")==0){
        temp = temp->child; // relationalOp
        exp = generateExpression(temp);
        temp = temp->next;  
        exp->next = generateExpression(temp); // arithmeticExpr
    }

    if(strcmp(root->name,"relationalOp")==0){
        strcpy(tempStr,temp->val.s);
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
    }

    if(strcmp(root->name,"term")==0){
        temp = temp->child; // factor = arithmeticOrBooleanExpr/ var_id_num
        exp = generateExpression(temp);
        temp = temp->next;  
        if(temp!=NULL)
        exp->next = generateExpression(temp); // N5

    }

    if(strcmp(root->name,"N5")==0){
        temp = temp->child; // op2
        strcpy(tempStr,temp->val.s); // op2
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;  
        exp->next = generateExpression(temp); // factor
        temp=temp->next;
        if(temp!=NULL)
        exp->next->next = generateExpression(temp); // N5
    }

    if(strcmp(root->name,"var_id_num")==0){
        if(temp->child==NULL){
            if(temp->tag==1)
            exp = makeExpNode(1,NULL,false,tempArrIndex);
            if(temp->tag==2)
            exp = makeExpNode(2,NULL,false,tempArrIndex);
        }
        else{
            temp = temp->child; // ID
            if(temp->next==NULL){
                strcpy(tempStr,temp->val.s);
                exp = makeExpNode(4,tempStr,false,tempArrIndex); // Not array element
            }     
            else{
                if(temp->next->tag==1){
                    tempArrIndex.i = temp->next->val.i;    // Static Array
                    strcpy(tempStr,temp->val.s); // ID
                    exp = makeExpNode(5,tempStr,false,tempArrIndex);
                }
                else{
                    // Dynamic Array
                    strcpy(tempArrIndex.s,temp->next->val.s); // index ID
                    strcpy(tempStr,temp->val.s); // ID
                    exp = makeExpNode(5,tempStr,true,tempArrIndex);
                }    
            }
        }
    }
    printf("GeneratingExpression Ends: %s\n",root->name);

    return exp;
}


void constructST(struct astNode* root){

        printf("constructST : %s\n",root->name);

        if(root==NULL)
        {
            // Report Error!
            printf("Root is NULL");
            exit(0);
        }

        struct astNode* temp = root; // Using this for traversal

        if(strcmp(root->name,"program")==0){
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

        if(strcmp(root->name,"moduleDeclarations")==0){
           // <moduleDeclarations>  -->  <moduleDeclaration><moduleDeclarations> 
           // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL 
           // <moduleDeclarations>  -->  ε

            temp = temp->child; // ID or NULL

            while(temp!=NULL){
                // DONE - Wil get Module ID from here!
                char funcName[40];
                strcpy(funcName,temp->val.s);
                //printf("%s\n",funcName);
                
                if(strcmp("ε",funcName)!=0)
                    insertInFunTable(globalFuncTable,funcName,NULL,NULL);

                //printFunTable(globalFuncTable); // TODO - take care of input and output list
                //TODO - redeclaration error
                temp = temp->next; // ID or NULL;
            }

            return;
        }

        if(strcmp(root->name,"driverModule")==0){
            temp = temp->child; // <moduleDef>
            // <driverModule> -->  DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
            // <moduleDef>   -->  START <statements> END
            // <statements>  -->  <statement> <statements>  
            // ...

            //DONE
            currentVarTable = driverVarTable;
            constructST(temp); // if for moduleDef written below!
        }


        if(strcmp(root->name,"otherModules")==0){
            // <otherModules>   -->  ε
            // <otherModules>   -->  <module> <otherModules>
            while(temp->child!=NULL){ 
                constructST(temp->child); // module
                temp = temp->child->next; // otherModules
            }
        }

        if(strcmp(root->name,"module")==0){
            // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
        
            temp = temp->child; // ID
            char funcName[40];
            strcpy(funcName,temp->val.s);

            if(otherModPos==1)
            {
                //TODO - Redeclaration
                //DONE - Simply Add to Global Func Table
                //printf("%s\n",funcName);
                insertInFunTable(globalFuncTable,funcName,NULL,NULL); // TODO - check for errors, take care of input and output lists
                //printFunTable(globalFuncTable);

            }else if(otherModPos==2){
                // DONE - Check if definition was prev there or not! If not, then error will be printed!
                
                if(!searchInFunTable(globalFuncTable,funcName))
                    printf("Error\n"); // TODO - Proper naming of error
                //printf("%s\n",funcName);
                //printFunTable(globalFuncTable);
            }

            // DONE: currentVarTable = current var table of curr Function!
            currentVarTable = (retrieveFunTable(globalFuncTable,funcName)).localVarTable;
            //printVarTable(currentVarTable);

            temp = temp->next; // <input_plist>
            // Code for input_plist
            // <input_plist>  -->  ID COLON <dataType><N1>

            temp = temp->child; // ID
            char idName[40];
            strcpy(idName,temp->val.s);
            //printf("INPUTINPUTINPUT - %s\n",idName);
            // TODO:  Add this ID = temp->child to function table (input)  with type at temp->next
            temp = temp->next; // <dataType>
            struct astNode* temp2 = temp;    // <N1>
            // Datatype
            
            if(!checkIfArrayType(temp)){
                // temp->tag;
                insertInVarTable(currentVarTable,idName,false,temp->tag,0); // doing for function, nesting level will always be zero. // TODO - check for errors
                //printVarTable(currentVarTable);
            }else{
                // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
                // <range_arrays>  -->  <index> RANGEOP <index>
                temp2 = temp2->child; // <range_arrays>
                int type = temp2->next->tag;    
                int low,high;
                char lowId[40], highId[40];

                // printf("Check51 : %d!\n",temp->tag);

                temp2 = temp2->child;


                if(temp2->tag==1){
                    low = temp2->val.i;
                    high = temp2->next->val.i;
                    //printf("low = %d , high = %d \n",low,high);
                    insertInVarTable(currentVarTable,idName,true,type,0);
                    updateArrayVarStatic(currentVarTable,idName,low,high);
                }else{
                    strcpy(lowId,temp2->val.s);
                    strcpy(highId,temp2->next->val.s);

                    insertInVarTable(currentVarTable,idName,true,type,0);
                    updateArrayVarDynamic(currentVarTable,idName,lowId,highId);
                }
            }

            //printVarTable(currentVarTable);

            temp = temp->next;  // <N1> = ID <dataType> ID <dataType> ...
            // <N1>  -->  COMMA ID COLON <dataType> <N1>
            while(temp!=NULL){
                // temp = ID
                //printf("VALUE N1 - %s \n",temp->val.s);
                strcpy(idName,temp->val.s);
                // DONE:  Add this ID = temp->child to function table (input)  with type at temp->next
                temp = temp->next; // <dataType>
                temp2 = temp;

                if(!checkIfArrayType(temp)){
                    // temp->tag;
                    insertInVarTable(currentVarTable,idName,false,temp->tag,0); // doing for function, nesting level will always be zero. // TODO - check for errors
                    //printVarTable(currentVarTable);
                }
                else{
                    temp2 = temp2->child; // <range_arrays>
                    int type = temp2->next->tag;    
                    int low,high;
                    char lowId[40], highId[40];

                    // printf("Check51 : %d!\n",temp->tag);

                    temp2 = temp2->child;


                    if(temp2->tag==1)
                    {
                        low = temp2->val.i;
                        high = temp2->next->val.i;
                        //printf("low = %d , high = %d \n",low,high);
                        insertInVarTable(currentVarTable,idName,true,type,0);
                        updateArrayVarStatic(currentVarTable,idName,low,high);
                    }

                    else
                    {
                        strcpy(lowId,temp2->val.s);
                        strcpy(highId,temp2->next->val.s);

                        insertInVarTable(currentVarTable,idName,true,type,0);
                        updateArrayVarDynamic(currentVarTable,idName,lowId,highId);
                    }
                }  
                temp = temp->next; // N1 or NULL
            }

            //printVarTable(currentVarTable);
            // Code for input_plist ends

            // printf("Check7\n");
            // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
            temp = root->child; // ID
            temp = temp->next->next;

            // printf("Check8 : %s\n",temp->name);

            // constructST(temp);
            // temp = temp->next; // <ret> = <output_plist>
            // Code for output_plist starts
            // <output_plist>  -->  ID COLON <type><N2>
            // <ret>  = <output_plist> -->  ε

                if(temp->child!=NULL){
                
                temp = temp->child; // ID
                char idName[40];
                strcpy(idName,temp->val.s);
                //printf("OUTPUT VAR - %s\n",temp->val.s);
                
                // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
                // printf("Check9 : %s\n",temp->name);
                temp = temp->next; // <type>
                temp2 = temp;

                if(!checkIfArrayType(temp))
                {
                    insertInVarTable(currentVarTable,idName,false,temp->tag,0); 
                    // doing for function, nesting level will always be zero. // TODO - check for errors
                }
                else
                {
                    temp2 = temp2->child; // <range_arrays>
                    int type = temp2->next->tag;    
                    int low,high;
                    char lowId[40], highId[40];

                    // printf("Check51 : %d!\n",temp->tag);

                    temp2 = temp2->child;


                    if(temp2->tag==1)
                    {
                        low = temp2->val.i;
                        high = temp2->next->val.i;
                        //printf("low = %d , high = %d \n",low,high);
                        insertInVarTable(currentVarTable,idName,true,type,0);
                        updateArrayVarStatic(currentVarTable,idName,low,high);
                    }

                    else
                    {
                        strcpy(lowId,temp2->val.s);
                        strcpy(highId,temp2->next->val.s);

                        insertInVarTable(currentVarTable,idName,true,type,0);
                        updateArrayVarDynamic(currentVarTable,idName,lowId,highId);
                    }
                }
                // printf("Check10 : %s\n",temp->name);
                temp = temp->next; // <N2> = ID <type> ID <type> ...
                while(temp!=NULL){
                    // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
                    // temp is ID
                    strcpy(idName,temp->val.s);
                    temp = temp->next; // <type>
                    temp2 = temp;

                    if(!checkIfArrayType(temp))
                    {
                        insertInVarTable(currentVarTable,idName,false,temp->tag,0); 
                        // doing for function, nesting level will always be zero. // TODO - check for errors
                    }
                    
                    else
                    {
                        temp2 = temp2->child; // <range_arrays>
                        int type = temp2->next->tag;    
                        int low,high;
                        char lowId[40], highId[40];

                        // printf("Check51 : %d!\n",temp->tag);

                        temp2 = temp2->child;

                        if(temp2->tag==1)
                        {
                            low = temp2->val.i;
                            high = temp2->next->val.i;
                            //printf("low = %d , high = %d \n",low,high);
                            insertInVarTable(currentVarTable,idName,true,type,0);
                            updateArrayVarStatic(currentVarTable,idName,low,high);
                        }

                        else
                        {
                            strcpy(lowId,temp2->val.s);
                            strcpy(highId,temp2->next->val.s);

                            insertInVarTable(currentVarTable,idName,true,type,0);
                            updateArrayVarDynamic(currentVarTable,idName,lowId,highId);
                        }
                    }

                    temp = temp->next; // ID or NULL
                }
                printVarTable(currentVarTable);
                   // Code for output_plist ends
                }


            temp = root->child; // ID
            temp = temp->next->next->next; // <moduleDef>  
            constructST(temp);
        }

        if(strcmp(root->name,"moduleDef")==0){
            // <moduleDef>   -->  START <statements> END
            // <statements>  -->  <statement> <statements>  
            temp = temp->child; // <statement>
            while(temp!=NULL){
                // TODO : Start Different Scope!
                constructST(temp);
                temp = temp->next;
            }
        }

        if(strcmp(root->name,"ioStmt1")==0){
            // Input Stmt
            // Nothing much with Symbol Table

            temp = temp->child; // ID 
            if(strcmp(temp->name,"ID")==0){
                // TODO:  Check if ID exists in var table! If not, give error!
                // <ioStmt>  -->  GET_VALUE BO ID BC SEMICOL
                // ID will already be there in symbol table!
                return;
            }
        }

        if(strcmp(root->name,"ioStmt2")==0){
            // Output Stmt
            // Nothing much with Symbol Table

            return;
        }


        if(strcmp(root->name,"simpleStmt")==0){
            // <simpleStmt>   -->  <assignmentStmt>
            // <simpleStmt>   -->  <moduleReuseStmt>

            temp = temp->child;
            if(strcmp(temp->name,"assignmentStmt")==0){
                // <assignmentStmt>   -->   ID <whichStmt>
                // TODO : Check if the ID type on RHS matches! 
                // printf("Check1 : %s\n",temp->name);
                struct expNode* exp = generateExpression(temp);
            }
            
            if(strcmp(temp->name,"moduleReuseStmt")==0){
                // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
                
                temp = temp->child; // optional
                 //  TODO : Check if func parameters size and type are correct! 

                if(temp->child==NULL){
                    // <optional>  -->  ε
                    temp = temp->next;  // ID
                    // TODO : This is the function ID

                    temp = temp->next;  //  <idList>
                    temp = temp->child; // ID->ID->ID....

                    while(temp!=NULL){
                        // temp has an ID(parameter of function)
                        // TODO : Function Parameters
                        temp = temp->next;
                    }
                    return;
                }
                else{
                    // TODO : Return type of MODULE(ID) matches with L.H.S if optional isn't ε
                    // <optional>  -->  SQBO <idList> SQBC ASSIGNOP
                    temp = temp->child; // <idList>
                    temp = temp->child; // ID->ID->ID....
                    while(temp!=NULL){
                        // temp has an IDs where return value of function is stored
                        // TODO : Return Storing Parameters
                        temp = temp->next;
                    }

                    temp = root->child->child;  //moduleReuseStmt
                    temp = temp->child->next;   // ID
                    // TODO : This is the function ID

                    temp = temp->next;  //  <idList>
                    temp = temp->child; // ID->ID->ID....

                    while(temp!=NULL){
                        // temp has an ID(parameter of function)
                        // TODO : Function Parameters
                        temp = temp->next;
                    }
                }
            }
        } // simpleStmt Ends

        if(strcmp(root->name,"declareStmt")==0){
            // return;
            // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
            // TODO : Get dataType from right <dataType> and set Ids type to that! ADD to Symbol Table(Nothing to add here)

            temp = temp->child; // <idList>
            temp = temp->next; // <dataType>
            int datatype;

            // printf("Check1\n");

            if(temp->child==NULL){
                // Not an Array 
                // printf("Check2\n");
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
                // printf("Check22\n");
                while(temp!=NULL){
                    
                    // DONE: Now temp is a linked list of IDs - Add them to Symbol Table
                    //printf("DECLARE - IDS - %s\n",temp->val.s);
                    char varName[40];
                    strcpy(varName,temp->val.s);

                    insertInVarTable(currentVarTable,varName,false,datatype,globalNestingLevel); // TODO - Check for errors
                    temp = temp->next;
                }

            }else{
                // printf("Check3\n");
                // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
                // <range_arrays>  -->  <index> RANGEOP <index>
                temp = temp->child; // <range_arrays>  -->  <index> RANGEOP <index>

                datatype = temp->next->tag; // <type> -> Array datatype

                temp = temp->child; // <index>
                int low_tag,high_tag;
                int low,high;
                char lowId[40],highId[40];

                low_tag = temp->tag; // 1 or 4
                high_tag =  temp->next->tag;
                
                // If 4, it's an ID 
                // If 1, it's an Integer
                if(low_tag==1){
                    low = temp->val.i;
                }else{
                    strcpy(lowId,temp->val.s);
                }
                
                if(high_tag==1){
                    high = temp->val.i;
                }else{
                    strcpy(highId,temp->val.s);
                }

                // TODO : How to handle dynamic and static arrays?


                temp = root->child; // <idList>
                //<idList>  -->  ID <N3>
                temp = temp->child; // ID

                while(temp!=NULL){

                    // DONE: Now temp is a linked list of IDs - Add them to Symbol Table
                    char varName[40];
                    strcpy(varName,temp->val.s);

                    if(low_tag==1) // STATIC ARRAY
                    {
                        insertInVarTable(currentVarTable,varName,true,datatype,globalNestingLevel);
                        updateArrayVarStatic(currentVarTable,varName,low,high);
                    }

                    if(low_tag==4)
                    {
                        insertInVarTable(currentVarTable,varName,true,datatype,globalNestingLevel);
                        updateArrayVarDynamic(currentVarTable,varName,lowId,highId);
                    }
                    temp = temp->next;
                }

                // <type>  -->  INTEGER
                // <type>  -->  REAL
                // <type>  -->  BOOLEAN
            }

            //printVarTable(currentVarTable);

        } // declareStmt Ends

        if(strcmp(root->name,"conditionalStmt")==0){
            // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
            // DONE : Create another var table for conditionalStmt
            variableTable* tempTable = currentVarTable;
            globalNestingLevel++;
            if(tempTable->child == NULL)
            {
                variableTable* newTable = initializeVarTable();
                newTable->parent = tempTable;
                tempTable->child = newTable;
                currentVarTable = newTable;
            }
            else
            {
                variableTable* traverse = tempTable->child;

                while(traverse->next != NULL)
                    traverse = traverse->next;

                variableTable* newTable = initializeVarTable();
                traverse->next = newTable;
                newTable->parent = tempTable;
                currentVarTable = newTable;
            }
            // <caseStmts>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
            
            temp = temp->child; // <caseStmts>
            
            while(temp!=NULL){
                temp = temp->child; // <value>
                // <value>  -->  NUM
                // <value>  -->  TRUE
                // <value>  -->  FALSE
                // TODO : Check on Value Type Consistent - NUM/Boolean
                // TODO : Check if Boolean Value Type then no Default Stmt?
                temp = temp->next; // <statements>
                constructST(temp);

                temp = temp->next; // N9
                // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
                // <N9>  -->  ε
            }

            temp = root->child->next;  // <default> = <statements>
            constructST(temp);
             // <default>  -->  DEFAULT COLON <statements> BREAK SEMICOL
             // <default>  -->  ε 

            // DONE : Change Scope!
            currentVarTable = tempTable;
            globalNestingLevel--;
        } // conditionalStmt Ends

        if(strcmp(root->name,"iterativeStmt")==0){

            // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
            // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END

            variableTable* tempTable = currentVarTable;
            globalNestingLevel++;
            if(tempTable->child == NULL)
            {
                variableTable* newTable = initializeVarTable();
                newTable->parent = tempTable;
                tempTable->child = newTable;
                currentVarTable = newTable;
            }
            else
            {
                variableTable* traverse = tempTable->child;

                while(traverse->next != NULL)
                    traverse = traverse->next;

                variableTable* newTable = initializeVarTable();
                traverse->next = newTable;
                newTable->parent = tempTable;
                currentVarTable = newTable;
            }

            if(strcmp(root->val.s,"FOR")==0){
                temp = temp->child; // <ID>
                // TODO : ID for iterativeStmt 
                temp = temp->next; // <range>
                // <range>  -->  NUM1 RANGEOP NUM2
                int low,high;
                temp = temp->child; 

                low = temp->val.i;  // NUM1
                high = temp->next->val.i; // NUM2

                temp = root->child; // <ID>
                temp = temp->next;  // <range>
                temp = temp->next;  // <statements>

                constructST(temp);
                printf("FOR ENDS\n");
            }

            else if(strcmp(root->val.s,"WHILE")==0){
                temp = temp->child; // <arithmeticOrBooleanExpr>
                struct expNode* exp = generateExpression(temp);
                // TODO : Type Checking!
                temp = temp->next;  // <statements>
                constructST(temp);
                printf("WHILE ENDS\n");
            }

            // DONE : Change Scope!
            //printVarTable(currentVarTable);
            currentVarTable = tempTable;
            globalNestingLevel--;
        } // iterativeStmt Ends

}

void runConstructST(FILE* testFile, FILE* parseTreeFile){
    driverVarTable = initializeVarTable();
    globalFuncTable = initializeFunTable();
    currentVarTable = driverVarTable;
    printFunTable(globalFuncTable);

    // Same as runAst
    printf("Starting runAST\n");
    parserFree();

    FILE* fp = fopen("grammar.txt","r");

    readGrammerTextFile(fp);

    // printAllGrammar();

    if(printFlag)
    printf("\nComputing Firsts And Follows Started\n");

    computeFirstAndFollow();

    if(printFlag)
    printf("\nComputing Firsts And Follows Ended\n\n");

    if(printFlag)
    printf("\nComputing Parse Table Started\n");

    createParseTable();

    if(printFlag)
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
    
    if(printFlag)
    printf("Parsing Input Source Started\n");    

    struct treeNode* rootParseTree = parseInputSourceCode(head->next, Table, grammar, firstFollowSets,parseTreeFile);
    
    // inOrderParseTree(rootParseTree,parseTreeFile);    

    printf("\nParseTreeComputed\n");

    struct astNode* rootAstNode = generateAST(rootParseTree);
    // printLevelOrder(rootAstNode);
    //generateAST(rootParseTree);
    if(printFlag)
    printf("\nAST Complete\n");

    // runAst()

    printf("\nGenerating Symbol Table\n");
    
    constructST(rootAstNode);
}
