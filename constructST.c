#include "ast.h"
#include "symbolTableDef.h"

void constructST(struct astNode* root){

        if(root==NULL)
        {
            // Report Error!
            printf("Root is NULL");
            exit(0);
        }

        struct astNode* temp = root;

        if(strcmp(root->name,"program")==0){
            // <program>  -->  <moduleDeclarations> <otherModules><driverModule><otherModules>
            temp = temp->child; // <moduleDeclarations>
            constructST(temp);
            temp = temp->next; // <otherModules>
            constructST(temp);
            temp = temp->next; // <driverModule>
            constructST(temp);
            temp = temp->next; // <otherModules>
            constructST(temp);

            return; // Not necessary!!
        }

        if(strcmp(root->name,"moduleDeclarations")==0){
           // <moduleDeclarations>  -->  <moduleDeclaration><moduleDeclarations> 
           // <moduleDeclaration>  -->  DECLARE MODULE ID SEMICOL 

           // <moduleDeclarations>  -->  ε
            temp = temp->child; // ID or NULL

            while(temp!=NULL){
                // TODO : Wil get Module ID from here!
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
            // TODO:  Add this ID = temp->child to function table
            temp = temp->next; // <input_plist>
            constructST(temp);
            temp = temp->next; // <ret> = <output_plist>
            constructST(temp);
            temp = temp->next; // <moduleDef>  
            constructST(temp);
        }

        if(strcmp(root->name,"input_plist")==0){
            // <input_plist>  -->  ID COLON <dataType><N1>

            temp = temp->child; // ID
            // TODO:  Add this ID = temp->child to function table (input)  with type at temp->next
            temp = temp->next; // <dataType>
            temp = temp->next; // <N1>
            while(temp!=NULL){
                temp = temp->next; // ID
                // TODO:  Add this ID = temp->child to function table (input)  with type at temp->next
                temp = temp->next; // <dataType>
                temp = temp->next; // N1 or NULL
            }
        }

        if(strcmp(root->name,"output_plist")==0){
            // <output_plist>  -->  ID COLON <type><N2>

            temp = temp->child; // ID
            // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
            temp = temp->next; // <type>
            temp = temp->next; // <N2>
            while(temp!=NULL){
                temp = temp->next; // ID
                // TODO:  Add this ID = temp->child to function table (output)  with type at temp->next
                temp = temp->next; // <type>
                temp = temp->next; // N2 or NULL
            }
        }

        if(strcmp(root->name,"moduleDef")==0){
            // <moduleDef>   -->  START <statements> END
            // <statements>  -->  <statement> <statements>  
            temp = temp->child; // <statement>
            while(temp!=NULL){
                constructST(temp);
                temp = temp->child;
            }
        }

        if(strcmp(root->name,"ioStmt")==0){
            
            temp = temp->child; // ID 
            if(strcmp(temp->name,"ID")==0){
                // TODO:  Add this ID = temp->child to function table (input)
                // <ioStmt>  -->  GET_VALUE BO ID BC SEMICOL
                return;
            }
            // <ioStmt>  -->  PRINT BO <var> BC SEMICOL
            // <var>  -->  <var_id_num>
            // <var>  -->  <boolConstt>
            
            // or temp now will be equal to "var_id_num" / "boolConstt"
            
            if(strcmp(temp->name,"var_id_num")==0){
                if(temp->child==NULL){
                    // <var_id_num>  -->  NUM
                    // <var_id_num>  -->  RNUM
                    // TODO : Add NUM or RNUM to variable table
                }else{
                    // <var_id_num>  -->  ID <whichId>
                    // TODO:  Add this ID = temp->child to variable table
                    
                    temp = temp->next; // <whichId> = <index>
                    if(temp!=NULL){
                        // <whichId>   -->  SQBO <index> SQBC
                        // <index>  -->  NUM
                        // <index>  -->  ID
                        // TODO : Add NUM or ID to variable table
                    }
                    // <whichId>   -->  ε  , return
                }
                return;
            }

            if(strcmp(temp->name,"boolConstt")==0){ 
                // <boolConstt>  -->  TRUE
                // <boolConstt>  -->  FALSE
                // TODO : Add boolConstt to variable table
                return;
            }

        }

        if(strcmp(root->name,"simpleStmt")==0){
            temp = root->child;
            if(strcmp(temp->name,"assignmentStmt")==0){
                // <simpleStmt>   -->  <assignmentStmt>
                // <assignmentStmt>   -->   ID <whichStmt>
                temp = temp->child; // ID
                // TODO:  Add the value to this ID wherever it exists in the symbol table
                // TODO: If not exists, Report Error!
                temp = temp->next; // <whichStmt>

                // <whichStmt>  -->  <lvalueIDStmt> = <expression> = <U> / <arithmeticOrBooleanExpr>
                // <whichStmt>  -->  <lvalueARRStmt>
                if(strcmp(temp->name,"lvalueARRStmt")==0){
                    // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL 
                    temp = temp->child; // <index>
                    if(strcmp(temp->name,"index")==0){

                    }
                }
                // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
                // <expression>  -->  <arithmeticOrBooleanExpr>
                // <expression>  -->  <U>
                if(strcmp(temp->name,"U")==0){
                    // <U>  -->  <unary_op> <new_NT>
                    temp = temp->child; // <unary_op>
                    if(strcmp(temp->name,"unary_op")==0){
                        
                    }
                    temp = temp->next; // <new_NT>
                    if(strcmp(temp->name,"new_NT")==0){
                        // <new_NT>  -->  BO <arithmeticExpr> BC
                        // <arithmeticExpr>  -->  <term> <N4>
                        // <N4>  -->  <op1> <term> <N4>
                        // <N4>  -->  ε
                        // <term>  -->  <factor> <N5>
                        // <factor>  -->  BO <arithmeticOrBooleanExpr> BC  
                        // <factor>  -->  <var_id_num>
                        // <N5>  -->  <op2> <factor> <N5> 
                        // <N5>  -->  ε 
                        // <new_NT>  -->  <var_id_num>
                    }

                }
                
                if(strcmp(temp->name,"arithmeticOrBooleanExpr")==0){
                    // <arithmeticOrBooleanExpr>  -->  <AnyTerm> <N7>
                    temp = temp->child;
                }
            }
            
            if(strcmp(temp->name,"moduleReuseStmt")==0){
                // <simpleStmt>   -->  <moduleReuseStmt>
                // <moduleReuseStmt>  -->  <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
            }
        } // simpleStmt Ends

        if(strcmp(root->name,"declareStmt")==0){
            // <declareStmt>  -->  DECLARE <idList> COLON <dataType> SEMICOL
            // TODO : Get dataType from right <dataType> and set Ids type to that! ADD to Symbol Table(Nothing to add here)

            temp = temp->child; // <idList>
            temp = temp->next; // <dataType>
            int datatype;

            if(temp->child==NULL){
                // Not an Array 
                datatype = temp->tag; 
                /*
                    1 - INTEGER
                    2 - REAL/FLOAT
                    3 - BOOLEAN
                */
                temp = root->child; // <idList>
                //<idList>  -->  ID <N3>
                temp = temp->child; // ID
                
                while(temp!=NULL){

                    // TODO: Now temp is a linked list of IDs - Add them to Symbol Table
                    temp = temp->next;
                }

            }else{
                // <dataType>  -->   ARRAY SQBO <range_arrays> SQBC OF <type>
                // <range_arrays>  -->  <index> RANGEOP <index>
                temp = temp->child; // <range_arrays>  -->  <index> RANGEOP <index>

                datatype = temp->next->tag; // <type> -> tag

                temp = temp->child; // <index>
                int low_tag,high_tag;

                low_tag = temp->tag; // 1 or 4
                high_tag =  temp->next->tag;
                
                // If 4, it's an ID 
                // If 1, it's an Integer


                // TODO : How to handle dynamic and static arrays?


                temp = root->child; // <idList>
                //<idList>  -->  ID <N3>
                temp = temp->child; // ID

                while(temp!=NULL){

                    // TODO: Now temp is a linked list of IDs - Add them to Symbol Table
                    temp = temp->next;
                }

                // <type>  -->  INTEGER
                // <type>  -->  REAL
                // <type>  -->  BOOLEAN
            }

        } // declareStmt Ends

        if(strcmp(root->name,"conditionalStmt")==0){
             // <conditionalStmt>  -->  SWITCH BO ID BC START <caseStmts> <default> END
             // <caseStmts>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>
            
            temp = temp->child; // <caseStmts>

            // <value>  -->  NUM
            // <value>  -->  TRUE
            // <value>  -->  FALSE

            // <N9>  -->  CASE <value> COLON <statements> BREAK SEMICOL <N9>




            temp = temp->next;  // <default>
             // <default>  -->  DEFAULT COLON <statements> BREAK SEMICOL
             // <default>  -->  ε 

        } // conditionalStmt Ends

        if(strcmp(root->name,"iterativeStmt")==0){

            // <iterativeStmt>  -->  FOR BO ID IN <range> BC START <statements> END
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
        } // iterativeStmt Ends

}
