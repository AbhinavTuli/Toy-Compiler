#include "parser.h"
#include "parserDef.h"
#include "ast.h"
#include "lexerDef.h"

void printGivenLevel(struct astNode* root, int level); 
int height(struct astNode* node); 

void printLevelOrder(struct astNode* root) 
{ 
    //printf("Level order Called\n");
    int h = height(root); 
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
    if (level == 1) 
        printf("%s ", root->name); 
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
  
/* Compute the "height" of a tree -- the number of 
    nodes along the longest path from the root node 
    down to the farthest leaf node.*/
int height(struct astNode* node) 
{   
    return 25;
    if (node==NULL) 
        return 0;
    int maxh=1;
    struct astNode* temp=node;
    struct astNode* temp2=node;
    if(temp->child!=NULL){
        maxh=1+height(temp->child);
        //temp=temp->child;
    }
    /* compute the height of each subtree */
    while(temp2!=NULL){
        temp2=temp2->next;
        int t=height(temp2);
        if(t>maxh){
                maxh=t;
        }
    } 
    return maxh;
} 

struct astNode* makeAstNode(char* name, value val,int tag,struct astNode* child){
    struct astNode* newNode = (struct astNode*) malloc(sizeof(struct astNode));
    newNode->child = child;
    newNode->tag = 0;
    //printf("Check!! : %s\n",name);
    strcpy(newNode->name,name);
    // printf("Check!! : %s\n",newNode->name);
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
    printf("%s\n",root->tnt);
    // tRoot - TreeNode , aRoot - astNode
    struct treeNode* temp;
    struct astNode* tempAstNode;
    struct astNode* childAstNode;
    value valAstNode;
    
    // check for null   
    
    // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
    if(strcmp(root->tnt,"program")==0){
        temp=root->child; // <moduleDeclarations>
        childAstNode=generateAST(temp);
        temp = temp->next; // <otherModules>
        tempAstNode = childAstNode;
        while(temp!=NULL){
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }

        return(childAstNode);
    }

    // <moduleDeclarations>  -->  <moduleDeclaration><moduleDeclarations> 
    else if(strcmp(root->tnt,"moduleDeclarations")==0 && strcmp(root->child->tnt,"moduleDeclaration")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp = temp->next;
        tempAstNode = childAstNode;
        while(temp!=NULL){
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }

        return(makeAstNode("moduleDeclarations",valAstNode,0,childAstNode));
    }
    
    // <moduleDeclarations>  -->  ε
    
    else if(strcmp(root->tnt,"moduleDeclarations")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("moduleDeclarations",valAstNode,4,NULL));
    }
    

    // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL 
    else if(strcmp(root->tnt,"moduleDeclaration")==0){
        temp=root->child;
        temp=temp->next; //at MODULE now
        temp=temp->next; //at ID
        strcpy(valAstNode.s,temp->val.s); 
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);

        return(childAstNode);
    }

    // <otherModules>   -->  <module> <otherModules> 
    else if(strcmp(root->tnt,"otherModules")==0 && strcmp(root->child->tnt,"module")==0){
        temp=root->child; // <module>
        childAstNode=generateAST(temp);
        temp = temp->next; // <otherModules>
        childAstNode->next = generateAST(temp);

        return(makeAstNode("otherModules",valAstNode,0,childAstNode));
    }

    // <otherModules>   -->  ε
    else if(strcmp(root->tnt,"otherModules")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("otherModules",valAstNode,4,NULL));
    }

    // <driverModule> -->  DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
    else if(strcmp(root->tnt,"driverModule")==0){
        temp=root->child; // DRIVERDEF
        temp = temp->next; // DRIVER
        temp = temp->next; // PROGRAM
        temp = temp->next; // DRIVERENDDEF
        temp = temp->next; // <moduleDef>
        childAstNode=generateAST(temp);

        return(makeAstNode("driverModule",valAstNode,0,childAstNode));
    }

    // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
    else if(strcmp(root->tnt,"module")==0){
        temp=root->child; // DEF
        temp=root->child; // MODULE
        temp=root->child; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode=childAstNode;
        temp=root->child; // ENDDEF
        temp=root->child; // TAKES
        temp=root->child; // INPUT
        temp=root->child; // SQBO
        
        tempAstNode->next=generateAST(temp);

        temp=temp->next; // SQBC
        temp=temp->next; // SEMICOL
        temp=temp->next; // <ret>
        
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;
        temp=temp->next; // <moduleDef>
        tempAstNode->next = generateAST(temp);

        return(makeAstNode("module",valAstNode,0,childAstNode));
    }
    // <ret>  -->  RETURNS SQBO <output_plist> SQBC SEMICOL
    else if(strcmp(root->tnt,"ret")==0 && strcmp(root->child->tnt,"RETURNS")==0){
        temp=root->child;
        temp=temp->next;
        temp=temp->next;
        return generateAST(temp);
    }

    // <ret>  -->  ε
    else if(strcmp(root->tnt,"ret")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("ret",valAstNode,4,NULL));
    }

    // <input_plist>  -->  ID COLON <dataType><N1>
    else if(strcmp(root->tnt,"input_plist")==0){
        temp=root->child; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp=temp->next;
        temp=temp->next;
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp=temp->next;
        tempAstNode->next=generateAST(temp);
        return(makeAstNode("input_plist",valAstNode,0,childAstNode));
    }
    // <N1>  -->  COMMA ID COLON <dataType> <N1>
    else if(strcmp(root->tnt,"N1")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child; // COMMMA
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp = temp->next;
        temp = temp->next;
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp = temp->next;
        childAstNode->next=generateAST(temp);
        return(makeAstNode("N1",valAstNode,0,childAstNode));
    }
    // <N1>  -->  ε
    else if(strcmp(root->tnt,"N1")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("N1",valAstNode,4,NULL));
    }
    // <output_plist>  -->  ID COLON <type><N2>
    else if(strcmp(root->tnt,"output_plist")==0){
        temp=root->child; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp=temp->next;
        temp=temp->next; // <type>
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp=temp->next; // <N2>
        tempAstNode->next=generateAST(temp);
        return(makeAstNode("output_plist",valAstNode,0,childAstNode));
    }
    // <N2>  -->  COMMA ID COLON <type><N2>
    else if(strcmp(root->tnt,"N1")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child;
        temp = temp->next;
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;
        temp = temp->next;
        temp = temp->next;
        tempAstNode->next=generateAST(temp);
        tempAstNode = tempAstNode ->next;
        temp = temp->next;
        tempAstNode->next=generateAST(temp);
        return(makeAstNode("N2",valAstNode,0,childAstNode));
    }

    // <N2>  -->  ε
    else if(strcmp(root->tnt,"N2")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("N2",valAstNode,4,NULL));
    }
    // <dataType>  -->   INTEGER
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        valAstNode.i = root->child->val.i;
        return(makeAstNode("dataType",valAstNode,1,NULL));
    }
    // <dataType>  -->   REAL
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"REAL")==0){
        valAstNode.f = root->child->val.f;
        return(makeAstNode("dataType",valAstNode,2,NULL));
    }
    // <dataType>  -->   BOOLEAN
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("dataType",valAstNode,3,NULL));
    }
    // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
    else if(strcmp(root->tnt,"datatype")==0 && strcmp(root->child->tnt,"ARRAY")==0){
        temp=root->child;
        temp = temp->next;
        temp = temp->next;
        childAstNode=generateAST(temp);
        temp = temp->next;
        temp = temp->next;
        temp = temp->next;
        childAstNode->next=generateAST(temp);
        return(makeAstNode("datatype",valAstNode,0,childAstNode));
    }
    // <range_arrays>  -->  <index> RANGEOP <index>
    else if(strcmp(root->tnt,"range_arrays")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp=temp->next;
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeAstNode("range_arrays",valAstNode,0,childAstNode));
    }
    // <type>  -->  INTEGER
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        valAstNode.i = root->child->val.i;
        return(makeAstNode("type",valAstNode,1,NULL));
    }
    // <type>  -->  REAL
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"REAL")==0){
        valAstNode.f = root->child->val.f;
        return(makeAstNode("type",valAstNode,2,NULL));
    }
    // <type>  -->  BOOLEAN
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("type",valAstNode,3,NULL));
    }
    // <moduleDef>   -->  START <statements> END
    else if(strcmp(root->tnt,"moduleDef")==0){
        return(generateAST(root->child->next));
    }
    // <statements>  -->  <statement> <statements>  
    else if(strcmp(root->tnt,"statements")==0 && strcmp(root->child->tnt,"statement")==0){
        temp = root->child;
        childAstNode = generateAST(temp);
        childAstNode->next = generateAST(temp->next);
        return makeAstNode("statements",valAstNode,0,childAstNode);

    }
    // <statements>  -->  ε
    else if(strcmp(root->tnt,"statements")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("statements",valAstNode,4,NULL));
    }
    // <statement>  -->  <ioStmt>  
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"ioStmt")==0){
        return(generateAST(root->child));
    }
    // <statement>  -->  <simpleStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"simpleStmt")==0){
        return(generateAST(root->child));
    }
    // <statement>  -->  <declareStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"declareStmt")==0){
        return(generateAST(root->child));
    }
    // <statement>  -->  <conditionalStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"conditionalStmt")==0){
        return(generateAST(root->child));
    }
    // <statement>  -->  <iterativeStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"iterativeStmt")==0){
        return(generateAST(root->child));
    }
    // <ioStmt>  -->  GET VALUE BO ID BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"GET")==0){  
        temp =root->child; // GET
        temp = temp->next; // VALUE
        temp = temp->next; // BO
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        return childAstNode;
    }
    // <ioStmt>  -->  PRINT BO <var> BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"PRINT")==0){
        return(makeAstNode("ioStmt",valAstNode,0,NULL));
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
        temp=root->child;
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        temp=temp->next;
        childAstNode->next = generateAST(temp);
        return(makeAstNode("var_id_num",valAstNode,0,childAstNode));
    }
    // <var_id_num>  -->  NUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"NUM")==0){
        valAstNode.i = root->child->val.i;
        return(makeAstNode("var_id_num",valAstNode,1,NULL));
    }

    // <var_id_num>  -->  RNUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"RNUM")==0){
        valAstNode.f = root->child->val.f;
        return(makeAstNode("var_id_num",valAstNode,2,NULL));
    }

    // <boolConstt>  -->  TRUE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"TRUE")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("boolConstt",valAstNode,3,NULL));
    }
    // <boolConstt>  -->  FALSE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"FALSE")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("boolConstt",valAstNode,3,NULL));
    }
    // <whichId>   -->  SQBO <index> SQBC
    else if(strcmp(root->tnt,"whichId")==0 && strcmp(root->child->tnt,"SQBO")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=generateAST(temp);
        return childAstNode;
    }
    // <whichId>   -->  ε
    else if(strcmp(root->tnt,"whichId")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("whichId",valAstNode,4,NULL));
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
        temp=root->child;
        strcpy(valAstNode.s,temp->val.s);
        temp=temp->next;
        childAstNode=generateAST(temp);
        return(makeAstNode("assignmentStmt",valAstNode,4,childAstNode));
    }
    // <whichStmt>  -->  <lvalueIDStmt>
    else if(strcmp(root->tnt,"whichStmt")==0 && strcmp(root->child->tnt,"lvalueIDStmt")==0){
        childAstNode = generateAST(root->child);
        // type same
        return childAstNode;
    }
    // <whichStmt>  -->  <lvalueARRStmt>
    else if(strcmp(root->tnt,"whichStmt")==0){
        return(generateAST(root->child));
    }
    // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
    else if(strcmp(root->tnt,"lvalueIDStmt")==0){
        return generateAST(root->child->next);
    }
    // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
    else if(strcmp(root->tnt,"lvalueARRStmt")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=generateAST(temp);
        temp=temp->next;
        temp=temp->next;
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeAstNode("lvalueARRStmt",valAstNode,0,childAstNode));
    }
    // <index>  -->  NUM
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"NUM")==0){
         astNode.val.i = root->child->val.i;
        return(makeAstNode("index",valAstNode,1,NULL));
    }
    // <index>  -->  ID
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"ID")==0){
        strcpy(valAstNode.s,root->child->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
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
        
        strcpy(valAstNode.s,root->child->val.s);
        tempAstNode->next = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode=tempAstNode->next;
        temp=temp->next; // WITH
        temp=temp->next; // PARAMETERS
        temp=temp->next;
        tempAstNode->next=generateAST(temp);
        return(makeAstNode("moduleReuseStmt",valAstNode,0,childAstNode));

    }
    // <optional>  -->  SQBO <idList> SQBC ASSIGNOP
    else if(strcmp(root->tnt,"optional")==0 && strcmp(root->child->tnt,"SQBO")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=generateAST(temp);
        return childAstNode;
    }
    // <optional>  -->  ε
    else if(strcmp(root->tnt,"optional")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("optional",valAstNode,4,NULL));
    }
////////////////////////////////////////////////////////////
   // <idList>  -->  ID <N3>
    else if(strcmp(root->tnt,"idList")==0){

        temp = root->child;
        strcpy(valAstNode.s,temp->val.s);

        childAstNode = makeAstNode("ID",valAstNode,4,NULL);// ID
        childAstNode->next = generateAST(temp->next); // <N3>
        return makeAstNode("idList",valAstNode,0,childAstNode);
    }

    // <N3> -->  COMMA ID <N3>
    else if(strcmp(root->tnt,"N3")==0 && strcmp(root->child->tnt,"COMMA")==0){
        // COMMA Ignored
        
        temp = root->child;
        temp= temp->next;
        
        strcpy(valAstNode.s,temp->val.s);

        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        childAstNode->next = generateAST(temp->next);

        return makeAstNode("N3",valAstNode,0,childAstNode);
    }

    // <N3> -->  ε
    else if(strcmp(root->tnt,"N3")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("N3",valAstNode,4,NULL));
    }

    // <expression>  -->  <arithmeticOrBooleanExpr> 
    else if(strcmp(root->tnt,"expression")==0 && strcmp(root->child->tnt,"arithmeticOrBooleanExpr")==0){
        return generateAST(root->child); 
    }

    // <expression>  -->  <U>
    else if(strcmp(root->tnt,"expression")==0 && strcmp(root->child->tnt,"U")==0){
        return generateAST(root->child); 
    }

    // <U>  -->  <unary_op> <new_NT>
    else if(strcmp(root->tnt,"U")==0){
        temp = root->child;
        childAstNode = generateAST(temp); // MINUS/PLUS Termianl Node return
        childAstNode->next = generateAST(temp->next);
        return childAstNode;
    }

    // <new_NT>  -->  BO <arithmeticExpr> BC
    else if(strcmp(root->tnt,"new_NT")==0 && strcmp(root->child->tnt,"BO")==0){
        return generateAST(root->child->next);
    }

    // <new_NT>  -->  <var_id_num>
    else if(strcmp(root->tnt,"new_NT")==0){
        return generateAST(root->child);
    }

    // <unary_op>  -->  PLUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"PLUS")==0){
        strcpy(valAstNode.s,"+");
        return makeAstNode("unary_op",valAstNode,4,NULL);
    }

    // <unary_op>  -->  MINUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"MINUS")==0){
        strcpy(valAstNode.s,"-");
        return makeAstNode("unary_op",valAstNode,4,NULL);
    }

    // <arithmeticOrBooleanExpr>  -->  <AnyTerm> <N7>
    else if(strcmp(root->tnt,"arithmeticOrBooleanExpr")==0){
        childAstNode = generateAST(root->child);
        childAstNode->next = generateAST(root->child->next);

        return makeAstNode("arithmeticOrBooleanExpr",valAstNode,0,childAstNode);
    }

    // <N7>  -->  <logicalOp> <AnyTerm> <N7>
    else if(strcmp(root->tnt,"N7")==0 && strcmp(root->child->tnt,"logicalOp")==0){
        childAstNode=generateAST(root->child);
        childAstNode->next=generateAST(root->child->next);
        childAstNode->next->next=generateAST(root->child->next->next);

        return makeAstNode("N7",valAstNode,0,childAstNode);
    }

    // <N7>  -->  ε
    else if(strcmp(root->tnt,"N7")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("N7",valAstNode,4,NULL));
    }

    // <AnyTerm>  -->  <arithmeticExpr> <N8>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"arithmeticExpr")==0){
        temp = root->child;
        childAstNode = generateAST(temp);
        printf("Yo 1\n");
        childAstNode->next = generateAST(temp->next);
        printf("Yo 2\n");
        return makeAstNode("AnyTerm",valAstNode,0,childAstNode);
    }

    // <AnyTerm>  -->  <boolConstt>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"boolConstt")==0){
       return generateAST(root->child);
    }

    else if(strcmp(root->tnt,"N8")==0){ 
        // <N8>  -->  <relationalOp> <arithmeticExpr>
        if(strcmp(root->child->tnt,"relationalOp")==0){
            printf("Check1\n");  
            temp = root->child; 
            printf("Check2\n");
            childAstNode = generateAST(temp);
            printf("Check3\n");
            temp = temp->next; 
            tempAstNode = childAstNode;
            tempAstNode->next = generateAST(temp);

            return makeAstNode("term",valAstNode,0,childAstNode);;
        }
        // <N8>  -->  ε
        else{
            strcpy(valAstNode.s,"ε");
            childAstNode = makeAstNode("N8",valAstNode,4,NULL);
            return childAstNode;
        }
    }


    // <arithmeticExpr>  -->  <term> <N4>
    else if(strcmp(root->tnt,"arithmeticExpr")==0){
        childAstNode = generateAST(root->child);
        childAstNode->next = generateAST(root->child->next);
        return makeAstNode("arithmeticExpr",valAstNode,0,childAstNode);
    } 

    else if(strcmp(root->tnt,"N4")==0){ 

        // <N4>  -->  <op1> <term> <N4>
        if(strcmp(root->child->tnt,"op1")==0){
            temp = root->child; // <op1>
            childAstNode = generateAST(temp);
            tempAstNode = childAstNode;

            temp = temp->next; // <factor>
            tempAstNode->next = generateAST(temp);
            tempAstNode = tempAstNode->next;

            temp = temp->next; // <N4>
            tempAstNode->next = generateAST(temp);

            return makeAstNode("N4",valAstNode,0,childAstNode);;
        }
        // <N4>  -->  ε
        else{
            return makeAstNode("N4",valAstNode,0,NULL);
        }
    }

    // <term>  -->  <factor> <N5>
    else if(strcmp(root->tnt,"term")==0){ 
        temp = root->child; 
        childAstNode = generateAST(temp);
        temp = temp->next; 
        tempAstNode = childAstNode;
        tempAstNode->next = generateAST(temp);

        return makeAstNode("term",valAstNode,0,childAstNode);
    }

    else if(strcmp(root->tnt,"N5")==0){ 

        // <N5>  -->  <op2> <factor> <N5> 
        if(strcmp(root->child->tnt,"op2")==0){
            temp = root->child; // <op2>
            childAstNode = generateAST(temp);
            tempAstNode = childAstNode;

            temp = temp->next; // <factor>
            tempAstNode->next = generateAST(temp);
            tempAstNode = tempAstNode->next;

            temp = temp->next; // <N5>
            tempAstNode->next = generateAST(temp);

            return makeAstNode("N5",valAstNode,0,childAstNode);
        }
        // <N5>  -->  ε 
        else{
            return makeAstNode("N5",valAstNode,0,NULL);
        }
    }

    else if(strcmp(root->tnt,"factor")==0){ 

        // <factor>  -->  BO <arithmeticOrBooleanExpr> BC  
        if(strcmp(root->child->tnt,"BO")==0){
            return generateAST(root->child->next);
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
            return makeAstNode("op1",valAstNode,4,NULL);
        }
        
        // <op1>  -->  MINUS
        else if(strcmp(root->child->tnt,"MINUS")==0){
            strcpy(valAstNode.s,"-");
            return makeAstNode("op1",valAstNode,4,NULL);
        }
    }
    
    else if(strcmp(root->tnt,"op2")==0){ 

        // <op2>  -->  MUL
        if(strcmp(root->child->tnt,"MUL")==0){
            strcpy(valAstNode.s,"*");
            return makeAstNode("op2",valAstNode,4,NULL);
        }
        
        // <op2>  -->  DIV
        else if(strcmp(root->child->tnt,"DIV")==0){
            strcpy(valAstNode.s,"/");
            return makeAstNode("op2",valAstNode,4,NULL);
        }
    }

    else if(strcmp(root->tnt,"logicalOp")==0){ 
        // <logicalOp>  -->  AND
        if(strcmp(root->child->tnt,"AND")==0){
            strcpy(valAstNode.s,"AND");
            return makeAstNode("logicalOp",valAstNode,4,NULL);
        }
        
        // <logicalOp>  -->  OR
        else if(strcmp(root->child->tnt,"OR")==0){
            strcpy(valAstNode.s,"OR");
            return makeAstNode("logicalOp",valAstNode,4,NULL);
        }
    }

    
    else if(strcmp(root->tnt,"relationalOp")==0){

        // <relationalOp>  -->  LT
        if(strcmp(root->child->tnt,"LT")==0){
            strcpy(valAstNode.s,"LT");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
        // <relationalOp>  -->  LE
        else if(strcmp(root->child->tnt,"LE")==0){
            strcpy(valAstNode.s,"LE");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
        // <relationalOp>  -->  GT
        else if(strcmp(root->child->tnt,"GT")==0){
            strcpy(valAstNode.s,"GT");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
        // <relationalOp>  -->  GE
        else if(strcmp(root->child->tnt,"GE")==0){
            strcpy(valAstNode.s,"GE");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
        // <relationalOp>  -->  EQ
        else if(strcmp(root->child->tnt,"EQ")==0){
            strcpy(valAstNode.s,"EQ");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
        // <relationalOp>  -->  NE
        else if(strcmp(root->child->tnt,"NE")==0){
            strcpy(valAstNode.s,"NE");
            return makeAstNode("relationalOp",valAstNode,4,NULL);
        }
    }

    // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
    else if(strcmp(root->tnt,"declareStmt")==0){
        temp = root->child; // DECLARE
        temp = temp->next;  // <idList>
        ;
        childAstNode = generateAST(temp);
        tempAstNode = childAstNode;

        temp = temp->next;  // COLON
        temp = temp->next;  // <dataType>
        tempAstNode->next = generateAST(temp);
        
        return makeAstNode("declareStmt",valAstNode,0,childAstNode);
    }

    // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
    else if(strcmp(root->tnt,"conditionalStmt")==0){
        temp = root->child; // SWITCH
        temp = temp->next;  // BO
        temp = temp->next;  // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;

        temp = temp->next;  // BC
        temp = temp->next;  // START
        temp = temp->next;  // <caseStmts>

        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next;  // <default>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        return makeAstNode("conditionalStmt",valAstNode,0,childAstNode);
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

        return makeAstNode("caseStmts",valAstNode,0,childAstNode);
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

        return makeAstNode("N9",valAstNode,0,childAstNode);
    }

    // <N9>  -->  ε
    else if(strcmp(root->tnt,"N9")==0){
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("N9",valAstNode,4,NULL));
    }

    // <value>  -->  NUM
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"NUM")==0){
        valAstNode.i = root->child->val.i;
        return(makeAstNode("value",valAstNode,1,NULL));
    }

    // <value>  -->  TRUE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"TRUE")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("value",valAstNode,3,NULL));
    }

    // <value>  -->  FALSE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"FALSE")==0){
        valAstNode.b = root->child->val.b;
        return(makeAstNode("value",valAstNode,3,NULL));
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
        strcpy(valAstNode.s,"ε");
        return(makeAstNode("default",valAstNode,4,NULL));
    }

    // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
    else if(strcmp(root->tnt,"iterativeStmt")==0 && strcmp(root->child->tnt,"FOR")==0){
        temp = root->child; // FOR
        temp = temp->next; // BO
        temp = temp->next; // ID
        strcpy(valAstNode.s,temp->val.s);
        childAstNode = makeAstNode("ID",valAstNode,4,NULL);
        tempAstNode = childAstNode;

        temp = temp->next; // <range>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next; // BC
        temp = temp->next; // START
        
        temp = temp->next; // <statements>
        tempAstNode->next = generateAST(temp);

        strcpy(valAstNode.s,"FOR");
        return makeAstNode("iterativeStmt",valAstNode,4,childAstNode);

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
        return makeAstNode("iterativeStmt",valAstNode,4,childAstNode);
    }


    // <range>  -->  NUM RANGEOP NUM
    else if(strcmp(root->tnt,"range")==0){
        temp = root->child; // NUM
        valAstNode.i = temp->val.i;
        childAstNode = makeAstNode("NUM",valAstNode,1,NULL); // pass value of NUM and type (TerminalNode)

        temp = temp->next; // RANGEOP
        temp = temp->next; // NUM
        valAstNode.i = temp->val.i;
        childAstNode->next = makeAstNode("NUM",valAstNode,1,NULL); 
        
        return makeAstNode("RANGEOP",valAstNode,0,childAstNode);
    }

}

void runAST(FILE* testFile, FILE* parseTreeFile){
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
    
    inOrderParseTree(rootParseTree,parseTreeFile);    

    generateAST(rootParseTree);
    if(printFlag)
    printf("\nParser Complete\n");


    /*
        To generate Files : ParseTable.txt
        Uncomment above lines
    */
}
