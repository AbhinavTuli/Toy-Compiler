#include "parser.h"
#include "parserDef.h"
#include "ast.h"
#include "astDef.h"
#include "lexerDef.h"

int astNodes = 0;

void printExpression(struct astNode* root){
    if(root==NULL){
        return;
    }
    printExpression(root->left);
    printf("%s ",root->name);
    printExpression(root->right);
}
void printInOrderAst(struct astNode* root){ //prints child, parent, then siblings
    if(root==NULL){
        return;
    }
    if(strcmp(root->name,"expression")==0){
        printExpression(root->child);
        return;
    }
    struct astNode* ch=root->child;
    printInOrderAst(ch);

    if(strcmp(root->val.s,"ε")==0)
    return;

    printf("%s ",root->name);
    if(ch!=NULL){
        ch=ch->next;
    }
    while(ch!=NULL){
        printInOrderAst(ch);
        ch=ch->next;
    }
}


void printGivenLevel(struct astNode* root, int level); 
void printLevelOrder(struct astNode* root) 
{ 
    int h = 25;
    int i; 
    for (i=1; i<=h; i++) {
        printGivenLevel(root, i); 
            printf("\n"); 
    }
} 

/* Print nodes at a given level */
void printGivenLevel(struct astNode* root, int level) 
{ 
    if (root == NULL) 
        return; 

    if (level == 1) {
        // numAstNodes++;
        if(root->tag==4)
        printf("%s#%s  ", root->name,root->val.s); 
        else
        printf("%s ", root->name); 

    }

    else if (level > 1) 
    {   
        struct astNode* temp = root->child;
        while(temp!=NULL){
            printGivenLevel(temp, level-1);
            temp = temp->next;
        } 
        printf("|");
    }
}  

struct astNode* makeAstNode(char* name, value val,int tag,struct astNode* child){

    astNodes++;

    struct astNode* newNode = (struct astNode*) malloc(sizeof(struct astNode));
    newNode->child = child;
    newNode->tag = tag;
    newNode->next = NULL;
    newNode->lineno = gLineNo;

    strcpy(newNode->name,name);
    switch (tag)
    {
        case 1:     newNode->val.i = val.i;
        break;
        
        case 2:     newNode->val.f = val.f;
        break;
            
        case 3:     newNode->val.b = val.b;
        break;
        
        case 4:     strcpy(newNode->val.s,val.s);
        break;

        default:
            break;
    }

    return newNode;
}

struct astNode* generateAST(struct treeNode* root){
    if(root==NULL){
        printf("ERROR!\n");
        return NULL;
    }
    // printf("AST: %s\n",root->tnt);

    struct treeNode* temp;

    struct astNode* tempAstNode;
    struct astNode* tempAstNode1;
    struct astNode* tempAstNode2;

    struct astNode* childAstNode;
    value valAstNode;
    
    char tempName[50];
    // check for null   
    
    // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
    if(strcmp(root->tnt,"program")==0){
        strcpy(tempName,"moduleDeclarations");
        temp=root->child; // <moduleDeclarations>
        gLineNo = temp->lineno;
        childAstNode=makeAstNode(tempName,valAstNode,0,generateAST(temp));
        temp = temp->next; // <otherModules>
        tempAstNode = childAstNode;
        while(temp!=NULL){
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }

        gLineNo = root->lineno;
        strcpy(tempName,"program");
        return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    // <moduleDeclarations>  -->  <moduleDeclaration><moduleDeclarations> 
    else if(strcmp(root->tnt,"moduleDeclarations")==0 && strcmp(root->child->tnt,"moduleDeclaration")==0){
        temp=root->child; // <moduleDeclaration>
        childAstNode=generateAST(temp);
        temp = temp->next; // <moduleDeclarations>
        tempAstNode = childAstNode;
        tempAstNode->next = generateAST(temp); 
        return childAstNode;
    }
    
    // <moduleDeclarations>  -->  ε
    
    else if(strcmp(root->tnt,"moduleDeclarations")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"moduleDeclarations");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,4,NULL));
        //return NULL;
    }
    

    // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL 
    else if(strcmp(root->tnt,"moduleDeclaration")==0){
        temp=root->child;
        temp=temp->next; //at MODULE now
        temp=temp->next; //at ID
        strcpy(valAstNode.s,temp->val.s); 
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);

        return(childAstNode);
    }

    // <otherModules>   -->  <module> <otherModules> 
    else if(strcmp(root->tnt,"otherModules")==0 && strcmp(root->child->tnt,"module")==0){
        temp=root->child; // <module>
        childAstNode=generateAST(temp);
        temp = temp->next; // <otherModules>
        childAstNode->next = generateAST(temp);
        strcpy(tempName,"otherModules");
        gLineNo = temp->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }

    // <otherModules>   -->  ε
    else if(strcmp(root->tnt,"otherModules")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"otherModules");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,4,NULL));
        // return NULL;
    }

    // <driverModule> -->  DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
    else if(strcmp(root->tnt,"driverModule")==0){
        temp=root->child; // DRIVERDEF
        temp = temp->next; // DRIVER
        temp = temp->next; // PROGRAM
        temp = temp->next; // DRIVERENDDEF
        temp = temp->next; // <moduleDef>
        childAstNode=generateAST(temp);
        strcpy(tempName,"driverModule");
        gLineNo = temp->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }

    // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
    else if(strcmp(root->tnt,"module")==0){
        temp=root->child; // DEF
        temp=temp->next; // MODULE
        temp=temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode=childAstNode;
        temp=temp->next; // ENDDEF
        temp=temp->next; // TAKES
        temp=temp->next; // INPUT
        temp=temp->next; // SQBO
        temp=temp->next; // <input_plist>

        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode->next;
        temp=temp->next; // SQBC
        temp=temp->next; // SEMICOL
        temp=temp->next; // <ret>
        
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;
        temp=temp->next; // <moduleDef>
        tempAstNode->next = generateAST(temp);

        strcpy(tempName,"module");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <ret>  -->  RETURNS SQBO <output_plist> SQBC SEMICOL
    else if(strcmp(root->tnt,"ret")==0 && strcmp(root->child->tnt,"RETURNS")==0){
        temp=root->child;
        temp=temp->next;
        temp=temp->next; // <output_plist>
        strcpy(tempName,"output_plist");
        gLineNo = temp->lineno;
        return (makeAstNode(tempName,valAstNode,0,generateAST(temp)));
        // return generateAST(temp);
    }

    // <ret>  -->  ε
    else if(strcmp(root->tnt,"ret")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"output_plist");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,4,NULL));
        // return NULL;
    }

    // <input_plist>  -->  ID COLON <dataType><N1>
    else if(strcmp(root->tnt,"input_plist")==0){
        temp=root->child; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp=temp->next;
        temp=temp->next;
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp=temp->next;
        tempAstNode->next=generateAST(temp);
        strcpy(tempName,"input_plist");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <N1>  -->  COMMA ID COLON <dataType> <N1>
    else if(strcmp(root->tnt,"N1")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child; // COMMMA
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp = temp->next; //COLON
        temp = temp->next; //<dataType>
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp = temp->next; //<N1>
        tempAstNode->next=generateAST(temp);
        // strcpy(tempName,"N1");
        // gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,0,childAstNode));
        return childAstNode;
    }
    // <N1>  -->  ε
    else if(strcmp(root->tnt,"N1")==0){
        // strcpy(valAstNode.s,"ε");
        // strcpy(tempName,"N1");
        // gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,4,NULL));
        return NULL;
    }
    // <output_plist>  -->  ID COLON <type><N2>
    else if(strcmp(root->tnt,"output_plist")==0){
        temp=root->child; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp=temp->next; //COLON
        temp=temp->next; // <type>
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp=temp->next; // <N2>
        tempAstNode->next=generateAST(temp);
        // strcpy(tempName,"output_plist");
        // gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,0,childAstNode));
        return childAstNode;
    }
    // <N2>  -->  COMMA ID COLON <type><N2>
    else if(strcmp(root->tnt,"N2")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child; //COMMA
        temp = temp->next; //ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp = temp->next; //COLON
        temp = temp->next; //<type>
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp = temp->next; //<N2>
        tempAstNode->next=generateAST(temp);
        // strcpy(tempName,"N2");
        // gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,0,childAstNode));
        return childAstNode;
    }

    // <N2>  -->  ε
    else if(strcmp(root->tnt,"N2")==0){
        // strcpy(valAstNode.s,"ε");
        // strcpy(tempName,"N2");
        // gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,4,NULL));
        return NULL;
    }
    // <dataType>  -->   INTEGER
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        valAstNode.i = root->child->val.i;
        strcpy(tempName,"dataType");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,1,NULL));
    }
    // <dataType>  -->   REAL
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"REAL")==0){
        valAstNode.f = root->child->val.f;
        strcpy(tempName,"dataType");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,2,NULL));
    }
    // <dataType>  -->   BOOLEAN
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"datatype");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }
    // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"ARRAY")==0){
        temp=root->child; //ARRAY
        temp = temp->next; //SQBO
        temp = temp->next; //<range_arrays>
        childAstNode=generateAST(temp);
        temp = temp->next; //SQBC
        temp = temp->next; //OF
        temp = temp->next; //<type>
        childAstNode->next=generateAST(temp);
        strcpy(tempName,"dataType");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <range_arrays>  -->  <index> RANGEOP <index>
    else if(strcmp(root->tnt,"range_arrays")==0){
        temp=root->child; //<index>
        childAstNode=generateAST(temp);
        temp=temp->next; //RANGEOP
        temp=temp->next; //<index>
        childAstNode->next=generateAST(temp);
        strcpy(tempName,"range_arrays");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <type>  -->  INTEGER
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        valAstNode.i = root->child->val.i;
        strcpy(tempName,"type");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,1,NULL));
    }
    // <type>  -->  REAL
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"REAL")==0){
        valAstNode.f = root->child->val.f;
        strcpy(tempName,"type");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,2,NULL));
    }
    // <type>  -->  BOOLEAN
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"type");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }
    // <moduleDef>   -->  START <statements> END
    else if(strcmp(root->tnt,"moduleDef")==0){

        strcpy(tempName,"moduleDef");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,generateAST(root->child->next)));
        // return(generateAST(root->child->next));
    }
    // <statements>  -->  <statement> <statements>  
    else if(strcmp(root->tnt,"statements")==0 && strcmp(root->child->tnt,"statement")==0){
        temp = root->child;
        childAstNode = generateAST(temp);

        childAstNode->next = generateAST(temp->next);

        // return childAstNode;
        strcpy(tempName,"statements");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));

    }
    // <statements>  -->  ε
    else if(strcmp(root->tnt,"statements")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"statements");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,NULL));
        //return NULL;
    }
    // <statement>  -->  <ioStmt>  
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"ioStmt")==0){
        // strcpy(tempName,"ioStmt");
        // gLineNo = root->child->lineno;
        // return(makeAstNode(tempName,valAstNode,0,generateAST(root->child)));
        return(generateAST(root->child));
    }
    // <statement>  -->  <simpleStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"simpleStmt")==0){
        strcpy(tempName,"simpleStmt");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,0,generateAST(root->child)));
        // return(generateAST(root->child));
    }
    // <statement>  -->  <declareStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"declareStmt")==0){
        // strcpy(tempName,"declareStmt");
        // gLineNo = root->child->lineno;
        // return(makeAstNode(tempName,valAstNode,0,generateAST(root->child)));
        return(generateAST(root->child));
    }
    // <statement>  -->  <conditionalStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"conditionalStmt")==0){
        // strcpy(tempName,"conditionalStmt");
        // gLineNo = root->child->lineno;
        // return(makeAstNode(tempName,valAstNode,0,generateAST(root->child)));
        return(generateAST(root->child));
    }
    // <statement>  -->  <iterativeStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"iterativeStmt")==0){
        // strcpy(tempName,"iterativeStmt");
        // gLineNo = root->child->lineno;
        // return(makeAstNode(tempName,valAstNode,0,generateAST(root->child)));
        return(generateAST(root->child));
    }
    // <ioStmt>  -->  GET_VALUE BO ID BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"GET_VALUE")==0){  
        temp =root->child; // GET_VALUE
        temp = temp->next; // BO
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);

        strcpy(tempName,"ioStmt1");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
        // return childAstNode;
    }
    // <ioStmt>  -->  PRINT BO <var> BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"PRINT")==0){
        temp=root->child; //PRINT
        temp=temp->next; //BO
        temp=temp->next; //<var>
        strcpy(tempName,"ioStmt2");
        childAstNode=generateAST(temp);
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <var>  -->  <var_id_num>
    else if(strcmp(root->tnt,"var")==0 && strcmp(root->child->tnt,"var_id_num")==0){
        return(generateAST(root->child));
    }
    // <var>  -->  <boolConstt>
    else if(strcmp(root->tnt,"var")==0){
        return(generateAST(root->child));
    }

    // <var_id_num>  -->  ID <whichId>
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"ID")==0){
        temp=root->child; //ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);

        temp=temp->next; //<whichId>
        tempAstNode = generateAST(temp);

        if(tempAstNode==NULL)
        return childAstNode;
        else{
            strcpy(tempName,"ARRAY_ELE");
            gLineNo = root->child->lineno;
            tempAstNode2 = makeAstNode(tempName,valAstNode,0,NULL);

            tempAstNode2->left = childAstNode;
            tempAstNode2->right = tempAstNode;

            return tempAstNode2;
        }
    }
    // <var_id_num>  -->  NUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"NUM")==0){
        valAstNode.i = root->child->val.i;
        strcpy(tempName,"NUM");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,1,NULL));
    }

    // <var_id_num>  -->  RNUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"RNUM")==0){
        valAstNode.f = root->child->val.f;
        strcpy(tempName,"RNUM");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,2,NULL));
    }

    // <boolConstt>  -->  TRUE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"TRUE")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"boolConstt");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }
    // <boolConstt>  -->  FALSE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"FALSE")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"boolConstt");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }
    // <whichId>   -->  SQBO <index> SQBC
    else if(strcmp(root->tnt,"whichId")==0 && strcmp(root->child->tnt,"SQBO")==0){
        childAstNode=generateAST(root->child->next);
        return childAstNode;
    }
    // <whichId>   -->  ε
    else if(strcmp(root->tnt,"whichId")==0){
        return NULL;
    }
    // <simpleStmt>   -->  <assignmentStmt> 
    else if(strcmp(root->tnt,"simpleStmt")==0 && strcmp(root->child->tnt,"assignmentStmt")==0){
        return(generateAST(root->child));
    }
    // <simpleStmt>   -->  <moduleReuseStmt>
    else if(strcmp(root->tnt,"simpleStmt")==0){
        return(generateAST(root->child));
    }
    // <assignmentStmt>   -->   ID <whichStmt>
    else if(strcmp(root->tnt,"assignmentStmt")==0){
        temp=root->child; //ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        temp=temp->next; //<whichStmt>
        childAstNode->next=generateAST(temp);
        strcpy(tempName,"assignmentStmt");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }
    // <whichStmt>  -->  <lvalueIDStmt>
    else if(strcmp(root->tnt,"whichStmt")==0 && strcmp(root->child->tnt,"lvalueIDStmt")==0){
        childAstNode = generateAST(root->child);
        // type same
        return childAstNode;
    }
    // <whichStmt>  -->  <lvalueARRStmt>
    else if(strcmp(root->tnt,"whichStmt")==0 && strcmp(root->child->tnt,"lvalueARRStmt")==0){
        return(generateAST(root->child));
    }
    // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
    else if(strcmp(root->tnt,"lvalueIDStmt")==0){
        strcpy(valAstNode.s,"=");
        strcpy(tempName,"ASSIGNOP");
        gLineNo = root->child->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);

        childAstNode->next=generateAST(root->child->next);

        // printf("EXPRESSION START1\n");
        // printLevelOrderExp(childAstNode->next);   // PRINT Expression
        // printf("EXPRESSION END1\n");

        strcpy(tempName,"lvalueIDStmt");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));

        // return childAstNode;
    }
    // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
    else if(strcmp(root->tnt,"lvalueARRStmt")==0){
        temp=root->child; //SQBO
        temp=temp->next; //<index>
        childAstNode=generateAST(temp);
        temp=temp->next; //SQBC
        temp=temp->next; //ASSIGNOP

        strcpy(valAstNode.s,"=");
        strcpy(tempName,"ASSIGNOP");
        gLineNo = temp->lineno;
        childAstNode->next = makeAstNode(tempName,valAstNode,4,NULL);

        temp=temp->next; //<expression>
        childAstNode->next->next=generateAST(temp);

        // printf("EXPRESSION START2\n");
        // printLevelOrderExp(childAstNode->next->next);   // PRINT Expression
        // printf("EXPRESSION END2\n");

        strcpy(tempName,"lvalueARRStmt");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));
    }


    // <index>  -->  NUM
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"NUM")==0){
        valAstNode.i = root->child->val.i;
        strcpy(tempName,"index");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,1,NULL));
    }
    // <index>  -->  ID
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"ID")==0){
        strcpy(valAstNode.s,root->child->val.s);
        strcpy(tempName,"index");
        gLineNo = root->child->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        return childAstNode;
    }

    // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
    else if(strcmp(root->tnt,"moduleReuseStmt")==0 && strcmp(root->child->tnt,"optional")==0){
        temp=root->child; // <optional>
        childAstNode=generateAST(temp);
        tempAstNode = childAstNode;
        temp=temp->next; // USE
        temp=temp->next; // MODULE
        temp=temp->next; // ID
        
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        tempAstNode->next = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode=tempAstNode->next;
        temp=temp->next; // WITH
        temp=temp->next; // PARAMETERS
        temp=temp->next; //<idList>
        tempAstNode->next=generateAST(temp);
        strcpy(tempName,"moduleReuseStmt");
        gLineNo = temp->lineno;
        return(makeAstNode(tempName,valAstNode,0,childAstNode));

    }
    // <optional>  -->  SQBO <idList> SQBC ASSIGNOP
    else if(strcmp(root->tnt,"optional")==0 && strcmp(root->child->tnt,"SQBO")==0){
        temp=root->child;   // SQBO
        temp=temp->next;    // <idList>
        childAstNode=generateAST(temp);
        
        temp = temp->next;  // SQBC
        temp = temp->next;  // ASSIGNOP

        strcpy(valAstNode.s,"=");
        strcpy(tempName,"ASSIGNOP");
        gLineNo = temp->lineno;
        childAstNode->next = makeAstNode(tempName,valAstNode,4,NULL);
        
        strcpy(tempName,"optional");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,4,childAstNode));
    }
    // <optional>  -->  ε
    else if(strcmp(root->tnt,"optional")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"optional");
        gLineNo = root->lineno;
        return(makeAstNode(tempName,valAstNode,0,NULL));
        // return NULL;
    }
////////////////////////////////////////////////////////////
    //<idList>  -->  ID <N3>

   else if(strcmp(root->tnt,"idList")==0){

        temp = root->child;
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);// ID
        childAstNode->next = generateAST(temp->next); // <N3>
        strcpy(tempName,"idList");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    // <N3> -->  COMMA ID <N3>
    else if(strcmp(root->tnt,"N3")==0 && strcmp(root->child->tnt,"COMMA")==0){
        // COMMA Ignored
        temp = root->child; //COMMA
        temp= temp->next; //ID
        
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        childAstNode->next = generateAST(temp->next);

        // strcpy(tempName,"N3");
        // gLineNo = root->lineno;
        // return makeAstNode(tempName,valAstNode,0,childAstNode);
        return childAstNode;
    }

    // <N3> -->  ε
    else if(strcmp(root->tnt,"N3")==0){
        strcpy(valAstNode.s,"ε");
        strcpy(tempName,"N3");
        gLineNo = root->lineno;
        // return(makeAstNode(tempName,valAstNode,4,NULL));

        return NULL;
    }

    // <expression>  -->  <arithmeticOrBooleanExpr> 
    else if(strcmp(root->tnt,"expression")==0 && strcmp(root->child->tnt,"arithmeticOrBooleanExpr")==0){
        strcpy(tempName,"expression");
        gLineNo = root->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,0,generateAST(root->child));
        return childAstNode; 
    }

    // <expression>  -->  <U>
    else if(strcmp(root->tnt,"expression")==0 && strcmp(root->child->tnt,"U")==0){
        strcpy(tempName,"expression");
        gLineNo = root->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,0,generateAST(root->child));
        return childAstNode; 
        // return generateAST(root->child); 
    }

    // <U>  -->  <unary_op> <new_NT>
    else if(strcmp(root->tnt,"U")==0){
        childAstNode = generateAST(root->child); // MINUS/PLUS Termianl Node return
        tempAstNode = generateAST(root->child->next);
        
        // No Right Child for Unary op
        childAstNode->left = tempAstNode;
        return childAstNode;
    }

    // <new_NT>  -->  BO <arithmeticExpr> BC
    else if(strcmp(root->tnt,"new_NT")==0 && strcmp(root->child->tnt,"BO")==0){
        strcpy(tempName,"arithmeticExprBracket");
        gLineNo = root->child->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,0,childAstNode);
        childAstNode->left = generateAST(root->child->next);
        return childAstNode;
        // childAstNode = generateAST(root->child->next);
        // childAstNode->
        // return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    // <new_NT>  -->  <var_id_num>
    else if(strcmp(root->tnt,"new_NT")==0){
        return generateAST(root->child);
    }

    // <unary_op>  -->  PLUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"PLUS")==0){
        strcpy(valAstNode.s,"++");
        strcpy(tempName,"unary_op");
        gLineNo = root->child->lineno;
        return makeAstNode(tempName,valAstNode,4,NULL);
    }

    // <unary_op>  -->  MINUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"MINUS")==0){
        strcpy(valAstNode.s,"--");
        strcpy(tempName,"unary_op");
        gLineNo = root->child->lineno;
        return makeAstNode(tempName,valAstNode,4,NULL);
    }

    // <arithmeticOrBooleanExpr>  -->  <AnyTerm> <N7>
    else if(strcmp(root->tnt,"arithmeticOrBooleanExpr")==0){
        // left child
        childAstNode = generateAST(root->child);
        // right child
        tempAstNode = generateAST(root->child->next);

        if(tempAstNode==NULL){
            return childAstNode;
        }else{

            tempAstNode->left = childAstNode;
            return tempAstNode;
        }
    }

    // <N7>  -->  <logicalOp> <AnyTerm> <N7>
    else if(strcmp(root->tnt,"N7")==0 && strcmp(root->child->tnt,"logicalOp")==0){
        childAstNode=generateAST(root->child);
        // logicalOp Right
        tempAstNode1=generateAST(root->child->next); // AnyTerm
        tempAstNode2=generateAST(root->child->next->next);  // N7

        if(tempAstNode2!=NULL){
            childAstNode->right = tempAstNode2; // logicalOp ka right N7
            tempAstNode2->left = tempAstNode1;  // N7 ka left AnyTerm
        }else{
            childAstNode->right = tempAstNode1;
        }

        return childAstNode;
    }

    // <N7>  -->  ε
    else if(strcmp(root->tnt,"N7")==0){
        return NULL;
    }

    // <AnyTerm>  -->  <arithmeticExpr> <N8>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"arithmeticExpr")==0){
         // left
        childAstNode = generateAST(root->child);    // arithmeticExpr
        // right
        tempAstNode = generateAST(root->child->next); // N8

        if(tempAstNode==NULL){
            return childAstNode;
        }else{
            tempAstNode->left = childAstNode;
            return tempAstNode;
        }
    }

    // <AnyTerm>  -->  <boolConstt>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"boolConstt")==0){
       return generateAST(root->child);
    }

    else if(strcmp(root->tnt,"N8")==0){ 
        // <N8>  -->  <relationalOp> <arithmeticExpr>
        if(strcmp(root->child->tnt,"relationalOp")==0){
            childAstNode = generateAST(root->child);
            tempAstNode = generateAST(root->child->next);

            childAstNode->right = tempAstNode;
            return childAstNode;
        }
        // <N8>  -->  ε
        else{
            return NULL;
        }
    }


    // <arithmeticExpr>  -->  <term> <N4>
    else if(strcmp(root->tnt,"arithmeticExpr")==0){
        childAstNode = generateAST(root->child);
        tempAstNode = generateAST(root->child->next);
        
        if(tempAstNode==NULL){
            return childAstNode;
        }else{
            tempAstNode->left = childAstNode;
            return tempAstNode;
        }
    } 

    else if(strcmp(root->tnt,"N4")==0){ 

        // <N4>  -->  <op1> <term> <N4>
        if(strcmp(root->child->tnt,"op1")==0){
            childAstNode=generateAST(root->child);  // op1
            tempAstNode1=generateAST(root->child->next); // term
            tempAstNode2=generateAST(root->child->next->next);  // N4

            if(tempAstNode2!=NULL){
                childAstNode->right = tempAstNode2;  // op1 ka right N4
                tempAstNode2->left = tempAstNode1;  // N4 ka left term
            }else{
                childAstNode->right = tempAstNode1; // op1 ka right term
            }

            return childAstNode;
        }
        // <N4>  -->  ε
        else{
            return NULL;
        }
    }

    // <term>  -->  <factor> <N5>
    else if(strcmp(root->tnt,"term")==0){ 
        childAstNode = generateAST(root->child);
        tempAstNode=generateAST(root->child->next);

        if(tempAstNode==NULL){
            return childAstNode;
        }else{
            tempAstNode->left = childAstNode;
            return tempAstNode;
        }
    }

    else if(strcmp(root->tnt,"N5")==0){ 
         // <N5>  -->  <op2> <factor> <N5> 
        if(strcmp(root->child->tnt,"op2")==0){
            childAstNode=generateAST(root->child);  // op2
            tempAstNode1=generateAST(root->child->next); // factor
            tempAstNode2=generateAST(root->child->next->next);  // N5

            if(tempAstNode2!=NULL){
                childAstNode->right = tempAstNode2;  // op2 ka right N5
                tempAstNode2->left = tempAstNode1;  // N5 ka left factor
            }else{
                childAstNode->right = tempAstNode1; // op2 ka right factor
            }

            return childAstNode;
        }
        // <N5>  -->  ε 
        else{
            return NULL;
        }
    }

    else if(strcmp(root->tnt,"factor")==0){ 

        // <factor>  -->  BO <arithmeticOrBooleanExpr> BC  
        if(strcmp(root->child->tnt,"BO")==0){
            strcpy(tempName,"arithmeticOrBooleanExprBracket");
            gLineNo = root->lineno;
            childAstNode =  makeAstNode(tempName,valAstNode,0,NULL);
            childAstNode->left = generateAST(root->child->next);
            return childAstNode;
            // return generateAST(root->child->next);
        }
        // <factor>  -->  <var_id_num>
        else{
            return generateAST(root->child);
        }
    }


    else if(strcmp(root->tnt,"op1")==0){ 

        // <op1>  -->  PLUS
        if(strcmp(root->child->tnt,"PLUS")==0){
            strcpy(valAstNode.s,"+");
            strcpy(tempName,"op1");
            gLineNo = root->child->lineno;  // PLUS ka line no
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        
        // <op1>  -->  MINUS
        else if(strcmp(root->child->tnt,"MINUS")==0){
            strcpy(valAstNode.s,"-");
            strcpy(tempName,"op1");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
    }
    
    else if(strcmp(root->tnt,"op2")==0){ 

        // <op2>  -->  MUL
        if(strcmp(root->child->tnt,"MUL")==0){
            strcpy(valAstNode.s,"*");
            strcpy(tempName,"op2");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        
        // <op2>  -->  DIV
        else if(strcmp(root->child->tnt,"DIV")==0){
            strcpy(valAstNode.s,"/");
            strcpy(tempName,"op2");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
    }

    else if(strcmp(root->tnt,"logicalOp")==0){ 
        // <logicalOp>  -->  AND
        if(strcmp(root->child->tnt,"AND")==0){
            strcpy(valAstNode.s,"AND");
            strcpy(tempName,"logicalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        
        // <logicalOp>  -->  OR
        else if(strcmp(root->child->tnt,"OR")==0){
            strcpy(valAstNode.s,"OR");
            strcpy(tempName,"logicalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
    }

    
    else if(strcmp(root->tnt,"relationalOp")==0){

        // <relationalOp>  -->  LT
        if(strcmp(root->child->tnt,"LT")==0){
            strcpy(valAstNode.s,"<");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        // <relationalOp>  -->  LE
        else if(strcmp(root->child->tnt,"LE")==0){
            strcpy(valAstNode.s,"<=");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        // <relationalOp>  -->  GT
        else if(strcmp(root->child->tnt,"GT")==0){
            strcpy(valAstNode.s,">");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        // <relationalOp>  -->  GE
        else if(strcmp(root->child->tnt,"GE")==0){
            strcpy(valAstNode.s,">=");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        // <relationalOp>  -->  EQ
        else if(strcmp(root->child->tnt,"EQ")==0){
            strcpy(valAstNode.s,"==");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
        // <relationalOp>  -->  NE
        else if(strcmp(root->child->tnt,"NE")==0){
            strcpy(valAstNode.s,"!=");
            strcpy(tempName,"relationalOp");
            gLineNo = root->child->lineno;
            return makeAstNode(tempName,valAstNode,4,NULL);
        }
    }

    // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
    else if(strcmp(root->tnt,"declareStmt")==0){
        temp = root->child; // DECLARE
        temp = temp->next;  // <idList>
        childAstNode = generateAST(temp);

        temp = temp->next;  // COLON
        temp = temp->next;  // <dataType>
        childAstNode->next = generateAST(temp);

      
        strcpy(tempName,"declareStmt");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
        // return childAstNode;
    }

    // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
    else if(strcmp(root->tnt,"conditionalStmt")==0){
        temp = root->child; // SWITCH
        temp = temp->next;  // BO
        temp = temp->next;  // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        tempAstNode = childAstNode;

        temp = temp->next;  // BC
        temp = temp->next;  // START
        temp = temp->next;  // <caseStmts>

        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next;  // <default>
        tempAstNode->next = generateAST(temp);
        strcpy(tempName,"conditionalStmt");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
        // return childAstNode;
    }

    // <caseStmts>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
    else if(strcmp(root->tnt,"caseStmts")==0){
        temp = root->child; // CASE
        temp = temp->next;  // <value>
        childAstNode = generateAST(temp);
        tempAstNode = childAstNode;

        temp = temp->next;  // COLON
        temp = temp->next;  // <statements>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next;  // BREAK
        temp = temp->next;  // SEMICOL
        temp = temp->next;  // <N9>
        tempAstNode->next = generateAST(temp);
        strcpy(tempName,"caseStmts");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    // <N9> is <caseStmt_1>
    // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
    else if(strcmp(root->tnt,"N9")==0 && strcmp(root->child->tnt,"CASE")==0){
        temp = root->child; // CASE
        temp = temp->next;  // <value>
        childAstNode = generateAST(temp);
        temp = temp->next; // COLON
        temp = temp->next; // <statements>
        childAstNode->next = generateAST(temp);
        temp = temp->next; // 
        temp = temp->next;
        temp = temp->next; //<N9>
        childAstNode->next->next = generateAST(temp);
        strcpy(tempName,"N9");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    // <N9>  -->  ε
    else if(strcmp(root->tnt,"N9")==0){
        return NULL;
    }

    // <value>  -->  NUM
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"NUM")==0){
        valAstNode.i = root->child->val.i;
        strcpy(tempName,"value");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,1,NULL));
    }

    // <value>  -->  TRUE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"TRUE")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"value");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }

    // <value>  -->  FALSE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"FALSE")==0){
        valAstNode.b = root->child->val.b;
        strcpy(tempName,"value");
        gLineNo = root->child->lineno;
        return(makeAstNode(tempName,valAstNode,3,NULL));
    }

    // <default>  -->  DEFAULT COLON <statements> BREAK SEMICOL
    else if(strcmp(root->tnt,"default")==0 && strcmp(root->child->tnt,"DEFAULT")==0){
        temp = root->child; // DEFAULT
        temp = temp->next; // COLON
        temp = temp->next; // <statements>
        return generateAST(temp);
    }

    // <default>  -->  ε 
    else if(strcmp(root->tnt,"default")==0){
        return NULL;
    }

    // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
    else if(strcmp(root->tnt,"iterativeStmt")==0 && strcmp(root->child->tnt,"FOR")==0){
        temp = root->child; // FOR
        temp = temp->next; // BO
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        strcpy(tempName,"ID");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,4,NULL);
        temp = temp->next; // IN
        
        tempAstNode = childAstNode;

        temp = temp->next; // <range>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next; // BC
        temp = temp->next; // START
        
        temp = temp->next; // <statements>
        tempAstNode->next = generateAST(temp);

        strcpy(valAstNode.s,"FOR");
        strcpy(tempName,"iterativeStmt");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,4,childAstNode);

    }

    // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END
    else if(strcmp(root->tnt,"iterativeStmt")==0 && strcmp(root->child->tnt,"WHILE")==0){
        temp = root->child; // WHILE
        temp = temp->next; // BO
        temp = temp->next; // <arithmeticOrBooleanExpr>
        childAstNode = generateAST(temp);
        tempAstNode = childAstNode;
        temp = temp->next; // BC
        temp = temp->next; // START
        temp = temp->next; // <statements>
        tempAstNode->next = generateAST(temp);

        strcpy(valAstNode.s,"WHILE");
        strcpy(tempName,"iterativeStmt");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,4,childAstNode);
    }


    // <range>  -->  NUM RANGEOP NUM
    else if(strcmp(root->tnt,"range")==0){
        temp = root->child; // NUM
        valAstNode.i = temp->val.i;
        strcpy(tempName,"NUM");
        gLineNo = temp->lineno;
        childAstNode = makeAstNode(tempName,valAstNode,1,NULL); // pass value of NUM and type (TerminalNode)

        temp = temp->next; // RANGEOP
        temp = temp->next; // NUM
        valAstNode.i = temp->val.i;
        strcpy(tempName,"NUM");
        gLineNo = temp->lineno;
        childAstNode->next = makeAstNode(tempName,valAstNode,1,NULL); 
        strcpy(tempName,"range");
        gLineNo = root->lineno;
        return makeAstNode(tempName,valAstNode,0,childAstNode);
    }

    printf("Not Reached :  %s\n",root->tnt);
}

int runAST(FILE* testFile){
    parserFree();

    FILE* fp = fopen("grammar.txt","r");

    readGrammerTextFile(fp);

    computeFirstAndFollow();
    createParseTable();

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
    struct treeNode* rootParseTree = parseInputSourceCode(head->next, Table, grammar, firstFollowSets);   
    if (rootParseTree==NULL){
        return 0;
    }
    //printLevelOrder(generateAST(rootParseTree));

    struct astNode* rootAstTree=generateAST(rootParseTree);

    if(printFlag)
    printInOrderAst(rootAstTree);

    return astNodes;
}
