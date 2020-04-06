#include "parser.h"
#include "parserDef.h"
#include "lexer.c"
#include "ast.h"

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
        return(makeASTNode("moduleDeclaration",module.name,NULL));
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
        temp=toot->child;
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

    // <N3> -->  COMMA ID <N3>

    // <N3> -->  ε

    // <expression>  -->  <arithmeticOrBooleanExpr> 

    // <expression>  -->  <U>

    // <U>  -->  <unary_op> <new_NT>

    // <new_NT>  -->  BO <arithmeticExpr> BC

    // <new_NT>  -->  <var_id_num>

    // <unary_op>  -->  PLUS

    // <unary_op>  -->  MINUS

    // <arithmeticOrBooleanExpr>  -->  <AnyTerm> <N7>

    // <N7>  -->  <logicalOp> <AnyTerm> <N7>

    // <N7>  -->  ε

    // <AnyTerm>  -->  <arithmeticExpr> <N8>

    // <AnyTerm>  -->  <boolConstt>

    // <N8>  -->  <relationalOp> <arithmeticExpr>

    // <N8>  -->  ε

    // <arithmeticExpr>  -->  <term> <N4>

    // <N4>  -->  <op1> <term> <N4>

    // <N4>  -->  ε

    // <term>  -->  <factor> <N5>

    // <N5>  -->  <op2> <factor> <N5>

    // <N5>  -->  ε

    // <factor>  -->  BO <arithmeticOrBooleanExpr> BC  

    // <factor>  -->  <var_id_num>

    // <op1>  -->  PLUS

    // <op1>  -->  MINUS

    // <op2>  -->  MUL

    // <op2>  -->  DIV

    // <logicalOp>  -->  AND

    // <logicalOp>  -->  OR

    // <relationalOp>  -->  LT

    // <relationalOp>  -->  LE

    // <relationalOp>  -->  GT

    // <relationalOp>  -->  GE

    // <relationalOp>  -->  EQ

    // <relationalOp>  -->  NE

    // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL

    // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END

    // <caseStmts>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>

    // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>

    // <N9>  -->  ε

    // <value>  -->  NUM

    // <value>  -->  TRUE

    // <value>  -->  FALSE

    // <default>  -->  DEFAULT COLON <statements> BREAK SEMICOL

    // <default>  -->  ε 

    // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END

    // <iterativeStmt>  -->  WHILE BO <arithmeticOrBooleanExpr> BC START <statements> END

    // <range>  -->  NUM RANGEOP NUM

}