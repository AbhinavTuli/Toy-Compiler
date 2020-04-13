#include "ast.h"
#include "symbolTableDef.h"

void constructST(struct astNode* root){

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




        while(1){                                    //  otherModules-- possible module definition without prior declaration
        assert(root->name=="otherModules");
        if(node->child==NULL)
            {
                node=node->next;
                break;
            }
        }        

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
