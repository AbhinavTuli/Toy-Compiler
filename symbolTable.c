#include "ast.h"
#include "symbolTableDef.h"

void constructST(struct astNode* root){
        assert(root->name=="program");
        struct astNode *node = root->child;
        struct astNode *parent;
        node=root->child;    
                           
        while(strcmp(node->val.s,"ε")!=0){                                    // at module declarations, initialize function table!!!
        assert(root->name=="moduleDeclarations");
            if(node->child==NULL){
                node=node->next; 
                break; 
            }
            else{
                                        
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
