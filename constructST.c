#include "ast.h"
#include "symbolTableDef.h"

void constructST(struct astNode* root){

        struct astNode* temp = root;

        assert(strcmp(root->name,"program"));
        struct astNode *node = root->child;
        struct astNode *parent;
        node=root->child;    
        
        // initializing entry in the global function table, 
        //we can create ` table when we encounter the complete function definition and return error in case definition is not present. 
        
        // There are three possible cases with the moduleDeclaration - ε, moduleDeclaration moduleDeclarations and declare module

        while(strcmp(node->val.s,"ε")!=0){                                    // case 1-check if there is no module declaration!!!
            assert(strcmp(root->name,"moduleDeclarations"));
            if(node->child==NULL)
            {
                node=node->next; 
                break; 
            }

            astNode *childnode = node->child;
        
            if (strcmp(childnode->name,"ID") && (childnode->next==NULL))                // case 3-declare Module!!!
            {
                /* code to  initialize entry in the global function table*/
                break;
            }

            else if(strcmp(childnode->name,"moduleDeclaration"))
            {
                //  code to reach end of module declarations
                
            }

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
        }

        if(strcmp(root->name,"declareStmt")==0){
        }

        if(strcmp(root->name,"conditionalStmt")==0){
        }

        if(strcmp(root->name,"iterativeStmt")==0){
        }

        // while(1){                                    //  otherModules-- possible module definition without prior declaration
        // // assert(root->name=="otherModules");
        // if(node->child==NULL)
        //     {
        //         node=node->next;
        //         break;
        //     }
        // }        

        while(1){                                    // driverModule-- create driver function table from here!!!
        assert(root->name=="driverModule"); 
        if(node->child==NULL)
            {
                node = node->next;
                break;
            }
        }

        while(1){                                    //  otherModules-- possible module definition without prior declaration
        assert(root->name=="otherModules");
        if(node->child==NULL)
            {
                break;
            }
        }                


        // while(){
        //     if((node->child)->child==NULL)
        //         parent = node; 
        //     node=node->child;
        //     if(node->next!=NULL)
        //         {
        //             node = node->next; 
        //             continue;      
        //         }
        //         else{
        //             node = parent; 
        //         }
        // }
}
