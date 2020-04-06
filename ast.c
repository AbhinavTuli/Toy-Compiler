#include "parser.h"
#include "parserDef.h"
#include "lexer.c"
#include "ast.h"

struct astNode* makeAstNode(){
    struct astNode* newNode = (struct astNode*) malloc(sizeof(struct ASTNode));

}

struct astNode* generateAST(struct treeNode* root){
    // tRoot - TreeNode , aRoot - astNode
    struct treeNode* temp;
    struct astNode* tempAstNode;
    struct astNode* childAstNode;


    // check for null
    
    // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
    if(strcmp(root->tnt,"program")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp = temp->next;
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

        return(makeASTNode("moduleDeclarations","",childAstNode));
    }
    
    // <moduleDeclarations>  -->  ε
    
    else if(strcmp(root->tnt,"moduleDeclarations")==0){
        return(makeASTNode("moduleDeclarations","ε",NULL));
    }
    

    // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL 
    else if(strcmp(root->tnt,"moduleDeclaration")==0){
        temp=root->child;
        temp=temp->next; //at module now
        // Todo : Change in Parser.c 
        //  Pass Function Name  
        return(makeASTNode("moduleDeclaration",temp->val,NULL));
    }

    // <otherModules>   -->  <module> <otherModules> 
    else if(strcmp(root->tnt,"otherModules")==0 && strcmp(root->child->tnt,"module")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp = temp->next;
        tempAstNode = childAstNode;
        while(temp!=NULL){
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }
        return(makeASTNode("otherModules","",childAstNode));
    }

    // <otherModules>   -->  ε
    else if(strcmp(root->tnt,"otherModules")==0){
        return(makeASTNode("otherModules","ε",NULL));
    }

    // <driverModule> -->  DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
    else if(strcmp(root->tnt,"driverModule")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp = temp->next;
        tempAstNode = childAstNode;
        while(temp!=NULL){
            if(strcmp(temp->tnt,"moduleDef")!=0){
                temp=temp->next;
                continue;
            }
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }
        return(makeASTNode("driverModule","",childAstNode));
    }

    // <module> -->  DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
    else if(strcmp(root->tnt,"module")==0){
        temp=root->child;
        
        while(strcmp(temp->tnt,"input_plist")!=0){
            temp=temp->next;
        }
        childAstNode=generateAST(temp);
        temp=temp->next;
        tempAstNode=childAstNode;
        while(temp!=NULL){
            if(strcmp(temp->tnt,"ret")!=0 && strcmp(temp->tnt,"moduleDef")!=0){
                temp=temp->next;
                continue;
            }
            tempAstNode->next = generateAST(temp);
            temp=temp->next;
            tempAstNode = tempAstNode->next;
        }
        return(makeASTNode("module","",childAstNode));
    }
    // <ret>  -->  RETURNS SQBO <output_plist> SQBC SEMICOL
    else if(strcmp(root->tnt,"ret")==0 && strcmp(root->child->tnt,"RETURNS")==0){
        temp=root->child;
        while(temp!=NULL){
            if(strcmp(temp->tnt,"output_plist")==0){
                return generateAST(temp);
            }
            temp==temp->next;
        }
    }

    // <ret>  -->  ε
    else if(strcmp(root->tnt,"ret")==0){
        return(makeASTNode("ret","ε",NULL));
    }

    // <input_plist>  -->  ID COLON <dataType><N1>
    else if(strcmp(root->tnt,"input_plist")==0){
        temp=root->child;
        temp=temp->next;
        temp=temp->next;
        childAstNode=generateAST(temp);
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("input_plist","",childAstNode));
    }
    // <N1>  -->  COMMA ID COLON <dataType> <N1>
    else if(strcmp(root->tnt,"N1")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child;
        temp = temp->next;
        temp = temp->next;
        temp = temp->next;
        childAstNode=generateAST(temp);
        temp = temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("N1","",childAstNode));
    }
    // <N1>  -->  ε
    else if(strcmp(root->tnt,"N1")==0){
        return(makeASTNode("N1","ε",NULL));
    }
    // <output_plist>  -->  ID COLON <type><N2>
    else if(strcmp(root->tnt,"output_plist")==0){
        temp=root->child;
        temp=temp->next;
        temp=temp->next;
        childAstNode=generateAST(temp);
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("output_plist","",childAstNode));
    }
    // <N2>  -->  COMMA ID COLON <type><N2>
    else if(strcmp(root->tnt,"N1")==0 && strcmp(root->child->tnt,"COMMA")==0){
        temp=root->child;
        temp = temp->next;
        temp = temp->next;
        temp = temp->next;
        childAstNode=generateAST(temp);
        temp = temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("N2","",childAstNode));
    }

    // <N2>  -->  ε
    else if(strcmp(root->tnt,"N2")==0){
        return(makeASTNode("N2","ε",NULL));
    }
    // <dataType>  -->   INTEGER
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        return(makeASTNode("dataType","INTEGER",NULL));
    }
    // <dataType>  -->   REAL
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"REAL")==0){
        return(makeASTNode("dataType","REAL",NULL));
    }
    // <dataType>  -->   BOOLEAN
    else if(strcmp(root->tnt,"dataType")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        return(makeASTNode("dataType","BOOLEAN",NULL));
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
        return(makeASTNode("dataytype","",childAstNode));
    }
    // <range_arrays>  -->  <index> RANGEOP <index>
    else if(strcmp(root->tnt,"range_arrays")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp=temp->next;
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("range_arrays","",childAstNode));
    }
    // <type>  -->  INTEGER
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"INTEGER")==0){
        return(makeASTNode("type","INTEGER",NULL));
    }
    // <type>  -->  REAL
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"REAL")==0){
        return(makeASTNode("type","REAL",NULL));
    }
    // <type>  -->  BOOLEAN
    else if(strcmp(root->tnt,"type")==0 && strcmp(root->child->tnt,"BOOLEAN")==0){
        return(makeASTNode("type","BOOLEAN",NULL));
    }
    // <moduleDef>   -->  START <statements> END
    else if(strcmp(root->tnt,"moduleDef")==0){
        temp=root->child;
        temp=temp->next;
        return(generateAST(temp));
    }
    // <statements>  -->  <statement> <statements>  
    else if(strcmp(root->tnt,"statements")==0 && strcmp(root->child->tnt,"statement")==0){
        return(makeASTNode("statements","ε",NULL));
    }
    // <statements>  -->  ε
    else if(strcmp(root->tnt,"statements")==0){
        return(makeASTNode("statements","ε",NULL));
    }
    // <statement>  -->  <ioStmt>  
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"ioStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <statement>  -->  <simpleStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"simpleStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <statement>  -->  <declareStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"declareStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <statement>  -->  <conditionalStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"conditionalStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <statement>  -->  <iterativeStmt>
    else if(strcmp(root->tnt,"statement")==0 && strcmp(root->child->tnt,"iterativeStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <ioStmt>  -->  GET_VALUE BO ID BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"GET_VALUE")==0){   
        return(makeASTNode("ioStmt",ID.name,NULL));
    }
    // <ioStmt>  -->  PRINT BO <var> BC SEMICOL
    else if(strcmp(root->tnt,"ioStmt")==0 && strcmp(root->child->tnt,"GET_VALUE")==0){   
        return(makeASTNode("ioStmt",ID.name,NULL));
    }
    // <var>  -->  <var_id_num>
    else if(strcmp(root->tnt,"var")==0 && strcmp(root->child->tnt,"var_id_num")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <var>  -->  <boolConstt>
    else if(strcmp(root->tnt,"var")==0){
        temp=root->child;
        return(generateAST(temp));
    }

    // <var_id_num>  -->  ID <whichId>
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"ID")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=makeASTNode(temp);
        return(makeASTNode("var_id_num","id",childAstNode));
    }
    // <var_id_num>  -->  NUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"NUM")==0){
        return(makeASTNode("var_id_num","NUM",NULL));
    }

    // <var_id_num>  -->  RNUM
    else if(strcmp(root->tnt,"var_id_num")==0 && strcmp(root->child->tnt,"RNUM")==0){
        return(makeASTNode("var_id_num","RNUM",NULL));
    }

    // <boolConstt>  -->  TRUE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"TRUE")==0){
        return(makeASTNode("boolConstt","TRUE",NULL));
    }
    // <boolConstt>  -->  FALSE
    else if(strcmp(root->tnt,"boolConstt")==0 && strcmp(root->child->tnt,"FALSE")==0){
        return(makeASTNode("boolConstt","FALSE",NULL));
    }
    // <whichId>   -->  SQBO <index> SQBC
    else if(strcmp(root->tnt,"whichId")==0 && strcmp(root->child->tnt,"SQBO")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=makeASTNode(temp);
        return(makeASTNode("whichId","ε",childAstNode));
    }
    // <whichId>   -->  ε
    else if(strcmp(root->tnt,"whichId")==0){
        return(makeASTNode("whichId","ε",NULL));
    }
    // <simpleStmt>   -->  <assignmentStmt> 
    else if(strcmp(root->tnt,"simpleStmt")==0 && strcmp(root->child->tnt,"assignmentStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <simpleStmt>   -->  <moduleReuseStmt>
    else if(strcmp(root->tnt,"simpleStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <assignmentStmt>   -->   ID <whichStmt>
    else if(strcmp(root->tnt,"assignmentStmt")==0){
        temp=root->child;
        //take id
        temp=temp->next;
        childAstNode=generateAST(temp);
        return(makeASTNode("assignmentStmt","id",childAstNode));
    }
    // <whichStmt>  -->  <lvalueIDStmt>
    else if(strcmp(root->tnt,"whichStmt")==0 && strcmp(root->child->tnt,"lvalueIDStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <whichStmt>  -->  <lvalueARRStmt>
    else if(strcmp(root->tnt,"whichStmt")==0){
        temp=root->child;
        return(generateAST(temp));
    }
    // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
    else if(strcmp(root->tnt,"lvalueIDStmt")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        return(makeASTNode("lvalueIDStmt","",childAstNode));
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
        return(makeASTNode("lvalueARRStmt","",childAstNode));
    }
    // <index>  -->  NUM
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"NUM")==0){
        return(makeASTNode("index","NUM",NULL));
    }
    // <index>  -->  ID
    else if(strcmp(root->tnt,"index")==0 && strcmp(root->child->tnt,"ID")==0){
        return(makeASTNode("index","ID",NULL));
    }

    // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
    else if(strcmp(root->tnt,"moduleReuseStmt")==0 && strcmp(root->child->tnt,"optional")==0){
        temp=root->child;
        childAstNode=generateAST(temp);
        temp=temp->next;
        temp=temp->next;
        temp=temp->next;
        //take id
        temp=temp->next;
        temp=temp->next;
        temp=temp->next;
        childAstNode->next=generateAST(temp);
        return(makeASTNode("moduleReuseStmt","id",childAstNode));

    }
    // <optional>  -->  SQBO <idList> SQBC ASSIGNOP
    else if(strcmp(root->tnt,"optional")==0 && strcmp(root->child->tnt,"SQBO")==0){
        temp=root->child;
        temp=temp->next;
        childAstNode=generateAST(temp);
        return(makeASTNode("optional","",childAstNode));
    }
    // <optional>  -->  ε
    else if(strcmp(root->tnt,"optional")==0){
        return(makeASTNode("optional","ε",NULL));
    }
////////////////////////////////////////////////////////////
   // <idList>  -->  ID <N3>
    else if(strcmp(root->tnt,"idList")==0){

        temp = root->child;
        childAstNode = makeASTNode(temp->tnt,temp->value,NULL)// ID
        childAstNode->next = generateAST(temp->next); // <N3>
        return(childAstNode);
    }

    // <N3> -->  COMMA ID <N3>
    else if(strcmp(root->tnt,"N3")==0 && strcmp(root->child->tnt,"COMMA")==0){
        // COMMA Ignored
        childAstNode = generateAST(temp);
        childAstNode->next = generateAST(temp->next);

        return(childAstNode);
    }

    // <N3> -->  ε
    else if(strcmp(root->tnt,"N3")==0){
        return(makeASTNode("N3","ε",NULL));
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
    else if(strcmp(root->tnt,"U")==0{
        temp = root->child;
        childAstNode = generateAST(temp); // MINUS/PLUS Termianl Node return
        childAstNode->next = generateAST(temp->next);

        // if(<U>.type != <U_1>.type)
        // ERROR;
        if (childAstNode.type != childAstNode.next.type)
            reportError();

        return childAstNode;
    }

    // <new_NT>  -->  BO <arithmeticExpr> BC
    else if(strcmp(root->tnt,"new_NT")==0 && strcmp(root->child->tnt,"BO")==0)){
       
    }

    // <new_NT>  -->  <var_id_num>
    else if(strcmp(root->tnt,"new_NT")==0){
       
    }

    // <unary_op>  -->  PLUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"PLUS")==0){
       
    }

    // <unary_op>  -->  MINUS
    else if(strcmp(root->tnt,"unary_op")==0 && strcmp(root->child->tnt,"MINUS")==0){
       
    }

    // <arithmeticOrBooleanExpr>  -->  <AnyTerm> <N7>
    else if(strcmp(root->tnt,"arithmeticOrBooleanExpr")==0){
       
    }

    // <N7>  -->  <logicalOp> <AnyTerm> <N7>
    else if(strcmp(root->tnt,"N7")==0 && strcmp(root->child->tnt,"logicalOp")==0){
       
    }

    // <N7>  -->  ε
    else if(strcmp(root->tnt,"N7")==0){
       return(makeASTNode("N7","ε",NULL));
    }

    // <AnyTerm>  -->  <arithmeticExpr> <N8>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"arithmeticExpr")==0){
        temp = root->child;
        childAstNode = generateAST(temp);
        childAstNode->next = generateAST(temp->next);
        return childAstNode; // TODO : Make AST Node
    }

    // <AnyTerm>  -->  <boolConstt>
    else if(strcmp(root->tnt,"AnyTerm")==0 && strcmp(root->child->tnt,"boolConstt")==0){
       return generateAST(root->child);
    }

    else if(strcmp(root->tnt,"N8")){ 
        // <N8>  -->  <relationalOp> <arithmeticExpr>
        if(strcmp(root->child->tnt,"relationalOp")){
            temp = root->child; 
            childAstNode = generateAST(temp);
            temp = temp->next; 
            tempAstNode = childAstNode;
            tempAstNode->next = generateAST(temp);

            return makeASTNode("term","",childAstNode);;
        }
        // <N8>  -->  ε
        else{
            return makeASTNode("N8","",NULL);
        }
    }


    // <arithmeticExpr>  -->  <term> <N4>

    else if(strcmp(root->tnt,"N4")){ 

        // <N4>  -->  <op1> <term> <N4>
        if(strcmp(root->child->tnt,"op1")){
            temp = root->child; // <op1>
            childAstNode = generateAST(temp);
            tempAstNode = childAstNode;

            temp = temp->next; // <factor>
            tempAstNode->next = generateAST(temp);
            tempAstNode = tempAstNode->next;

            temp = temp->next; // <N4>
            tempAstNode->next = generateAST(temp);

            return childAstNode;
        }
        // <N4>  -->  ε
        else{
            return makeASTNode("N4","",NULL);
        }
    }

    // <term>  -->  <factor> <N5>
    else if(strcmp(root->tnt,"term")){ 
        temp = root->child; 
        childAstNode = generateAST(temp);
        temp = temp->next; 
        tempAstNode = childAstNode;
        tempAstNode->next = generateAST(temp);

        return makeASTNode("term","",childAstNode);;
    }

    else if(strcmp(root->tnt,"N5")){ 

        // <N5>  -->  <op2> <factor> <N5> 
        if(strcmp(root->child->tnt,"op2")){
            temp = root->child; // <op2>
            childAstNode = generateAST(temp);
            tempAstNode = childAstNode;

            temp = temp->next; // <factor>
            tempAstNode->next = generateAST(temp);
            tempAstNode = tempAstNode->next;

            temp = temp->next; // <N5>
            tempAstNode->next = generateAST(temp);

            return childAstNode;
        }
        // <N5>  -->  ε 
        else{
            return makeASTNode("N5","",NULL);
        }
    }

    else if(strcmp(root->tnt,"factor")){ 

        // <factor>  -->  BO <arithmeticOrBooleanExpr> BC  
        if(strcmp(root->child->tnt,"BO")){
            return generateAST(root->child->next);
        }
        // <factor>  -->  <var_id_num>
        else{
            return generateAST(root->child);
        }
    }


    else if(strcmp(root->tnt,"op1")){ 

        // <op1>  -->  PLUS
        if(strcmp(root->child->tnt,"PLUS")){
            return makeASTNode("PLUS","+",NULL);
        }
        
        // <op1>  -->  MINUS
        else if(strcmp(root->child->tnt,"MINUS")){
            return makeASTNode("MINUS","-",NULL);
        }
    }
    
    else if(strcmp(root->tnt,"op2")){ 

        // <op2>  -->  MUL
        if(strcmp(root->child->tnt,"MUL")){
            return makeASTNode("MUL","*",NULL);
        }
        
        // <op2>  -->  DIV
        else if(strcmp(root->child->tnt,"DIV")){
            return makeASTNode("DIV","/",NULL);
        }
    }

    else if(strcmp(root->tnt,"logicalOp")){ 
        // <logicalOp>  -->  AND
        if(strcmp(root->child->tnt,"AND")){
            return makeASTNode("AND","AND",NULL);
        }
        
        // <logicalOp>  -->  OR
        else if(strcmp(root->child->tnt,"OR")){
            return makeASTNode("OR","OR",NULL);
        }
    }

    
    else if(strcmp(root->tnt,"relationalOp")){

        // <relationalOp>  -->  LT
        if(strcmp(root->child->tnt,"LT")){
            return makeASTNode("LT","<",NULL);
        }
        // <relationalOp>  -->  LE
        else if(strcmp(root->child->tnt,"LE")){
            return makeASTNode("LE","<=",NULL);
        }
        // <relationalOp>  -->  GT
        else if(strcmp(root->child->tnt,"GT")){
            return makeASTNode("GT",">",NULL);
        }
        // <relationalOp>  -->  GE
        else if(strcmp(root->child->tnt,"GE")){
            return makeASTNode("GE",">=",NULL);
        }
        // <relationalOp>  -->  EQ
        else if(strcmp(root->child->tnt,"EQ")){
            return makeASTNode("EQ","==",NULL);
        }
        // <relationalOp>  -->  NE
        else if(strcmp(root->child->tnt,"NE")){
            return makeASTNode("NE","!=",NULL);
        }
    }


    // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
    else if(strcmp(root->tnt,"declareStmt")==0){
        temp = root->child; // DECLARE
        temp = temp->next;  // <idList>
        childAstNode = generateAST(temp);
        tempAstNode = childAstNode;

        temp = temp->next;  // COLON
        temp = temp->next;  // <dataType>
        tempAstNode->next = generateAST(temp);
        
        return makeASTNode("declareStmt","",childAstNode);
    }

    // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
    else if(strcmp(root->tnt,"conditionalStmt")==0){
        temp = root->child; // SWITCH
        temp = temp->next;  // BO
        temp = temp->next;  // ID
        childAstNode = makeASTNode();
        tempAstNode = childAstNode;

        temp = temp->next;  // BC
        temp = temp->next;  // START
        temp = temp->next;  // <caseStmts>

        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next;  // <default>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        return makeASTNode("conditionalStmt","",childAstNode);
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

        return makeASTNode("caseStmts","",childAstNode);
    }

    // <N9> is <caseStmt_1>
    // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
    else if(strcmp(root->tnt,"N9")==0 && strcmp(root->child->tnt,"CASE")==0){
        temp = root->child; // CASE
        temp = temp->next;  // <value>
        childAstNode = generateAST(temp);
        temp = temp->next; // COLON
        temp = temp->next; // <statements>
        
        return makeASTNode(childAstNode)
    }

    // <N9>  -->  ε
    else if(strcmp(root->tnt,"N9")==0){
        return(makeASTNode("N9","ε",NULL));
    }

    // <value>  -->  NUM
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"NUM")==0){
        temp = root->child;
        return(makeASTNode("NUM",temp->val,NULL));
    }

    // <value>  -->  TRUE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"TRUE")==0){
        temp = root->child;
        return(makeASTNode("TRUE",temp->val,NULL));
    }

    // <value>  -->  FALSE
    else if(strcmp(root->tnt,"value")==0 && strcmp(root->child->tnt,"FALSE")==0){
        temp = root->child;
        return(makeASTNode("NUM",temp->val,NULL));
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
        return(makeASTNode("default","ε",NULL));
    }

    // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
    else if(strcmp(root->tnt,"iterativeStmt")==0 && strcmp(root->child->tnt,"FOR")==0){
        temp = root->child; // FOR
        temp = temp->next; // BO
        temp = temp->next; // ID
        childAstNode = makeASTNode("ID",temp->val,NULL);
        tempAstNode = childAstNode;

        temp = temp->next; // <range>
        tempAstNode->next = generateAST(temp);
        tempAstNode = tempAstNode->next;

        temp = temp->next; // BC
        temp = temp->next; // START
        
        temp = temp->next; // <statements>
        tempAstNode->next = generateAST(temp);

        return makeASTNode("iterativeStmt","FOR",childAstNode);

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
        tempAstNode->next = generateAST(temp);

        return makeASTNode("iterativeStmt","WHILE",childAstNode);
    }


    // <range>  -->  NUM RANGEOP NUM
    else if(strcmp(root->tnt,"range")==0){
        temp = root->child; // NUM
        childAstNode = makeASTNode("NUM",temp->val,NULL); // pass value of NUM and type (TerminalNode)
        tempAstNode = childAstNode;

        temp = temp->next; // RANGEOP
        tempAstNode->next = makeASTNode("NUM",temp->next->val,NULL);
        
        return makeASTNode("RANGEOP","",childAstNode);
    }

}
