
struct expNode* generateExpression(struct astNode* temp){

    struct astNode* root = temp;

    printf("GeneratingExpression : %s\n",root->name);

    if(temp==NULL){
         // Report Error!
        printf("generateExpression : temp is NULL\n");
        exit(0);
    }

    struct expNode* exp;
    struct expNode* expTemp;

    if(strcmp(root->name,"assignmentStmt")==0){
        // <assignmentStmt>   -->   ID <whichStmt>
        // <whichStmt>  -->  <lvalueIDStmt> 
        // <whichStmt>  -->  <lvalueARRStmt>
        temp = temp->child; // ID
        strcpy(tempStr,temp->val.s);

        if(strcmp(temp->next->name,"lvalueIDStmt")==0){
            // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
            exp = makeExpNode(4,tempStr,false,tempArrIndex);
            temp = temp->next->child;
            strcpy(tempStr,temp->val.s);
            exp->next = makeExpNode(0,tempStr,false,tempArrIndex);  // ASSIGNOP
            temp = temp->next;  // <expression>
            exp->next->next = generateExpression(temp);
        } 
        else{
            // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
            temp = temp->next;  // <lvalueARRStmt>
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
    // else if(strcmp(root->name,"lvalueIDStmt")==0){
    //     // <lvalueIDStmt>   -->  ASSIGNOP <expression> SEMICOL
    //     temp = temp->child; // ASSIGNOP
    //     strcpy(tempStr,temp->val.s);
    //     exp = makeExpNode(0,tempStr,false,tempArrIndex);

    //     temp = temp->next; // <expression>
    //     exp->next = generateExpression(temp);
    // }
    // else if(strcmp(root->name,"lvalueARRStmt")==0){
    //     // <lvalueARRStmt>  -->  SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
    //     temp = temp->child; // <index>
        
    //     temp = temp->next;  // ASSIGNOP

    //     temp = temp->next;  // <expression>

    //     generateExpression(temp);

    // }
    else if(strcmp(root->name,"U")==0){
        temp = temp->child;  // unary_op
        strcpy(tempStr,temp->val.s);
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp=temp->next;  //  new_NT
        exp->next = generateExpression(temp);
    }
    else if(strcmp(root->name,"arithmeticOrBooleanExpr")==0){
        temp = temp->child;
        exp = generateExpression(temp); // Anyterm
        temp = temp->next;  
        if(temp!=NULL)
        exp->next = generateExpression(temp);   // N7
    }
    else if(strcmp(root->name,"arithmeticOrBooleanExprBracket")==0){
        strcpy(tempStr,"(");
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->child; // arithmeticOrBooleanExpr
        exp->next = generateExpression(temp);
        expTemp = exp->next;
        while(expTemp->next!=NULL){
            expTemp = expTemp->next;
        }
        strcpy(tempStr,")");
        expTemp->next = makeExpNode(0,tempStr,false,tempArrIndex);
    }
    else if(strcmp(root->name,"N7")==0){
        temp = temp->child; 
        strcpy(tempStr,temp->val.s); // logicalOp
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;
        expTemp = exp;
        while(expTemp->next!=NULL){
                expTemp = expTemp->next;
        }
        expTemp->next = generateExpression(temp); // AnyTerm
        temp=temp->next;
        expTemp = expTemp->next;
        if(temp!=NULL){
            while(expTemp->next!=NULL){
                expTemp = expTemp->next;
            }
            expTemp->next = generateExpression(temp); // N7
        }
        
    }

    else if(strcmp(root->name,"boolConstt")==0){
        exp = makeExpNode(3,NULL,false,tempArrIndex);
    }
    else if(strcmp(root->name,"AnyTerm")==0){
        temp = temp->child; // arithmeticExpr
        exp = generateExpression(temp);
        temp = temp->next;  
        expTemp = exp;
        if(temp!=NULL){
            while(expTemp->next!=NULL){
                expTemp = expTemp->next;
            }
            expTemp->next = generateExpression(temp); // N8
        }
    }
    else if(strcmp(root->name,"arithmeticExpr")==0){
        temp = temp->child; // term
        exp = generateExpression(temp);
        temp = temp->next;  
        expTemp = exp;
        if(temp!=NULL){
            while(expTemp->next!=NULL){
                expTemp = expTemp->next;
            }
            expTemp->next = generateExpression(temp); // N4
        }
    }
    else if(strcmp(root->name,"arithmeticExprBracket")==0){

        strcpy(tempStr,"(");
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->child; // arithmeticExpr
        exp->next = generateExpression(temp);
        expTemp = exp->next;
        while(expTemp->next!=NULL){
            expTemp = expTemp->next;
        }
        strcpy(tempStr,")");
        expTemp->next = makeExpNode(0,tempStr,false,tempArrIndex);
        // temp = temp->child;
        // exp = generateExpression(temp);
    }
    else if(strcmp(root->name,"N4")==0){
        temp = temp->child; // op1
        strcpy(tempStr,temp->val.s); // op1
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;  
        exp->next = generateExpression(temp); // term
        temp=temp->next;
        expTemp = exp->next;
        if(temp!=NULL){
            while(expTemp->next!=NULL){
                expTemp = expTemp->next;
            }
            expTemp->next = generateExpression(temp); // N4
        }
    }
    else if(strcmp(root->name,"N8")==0){
        temp = temp->child; // relationalOp
        exp = generateExpression(temp);
        temp = temp->next;  
        exp->next = generateExpression(temp); // arithmeticExpr
    }
    else if(strcmp(root->name,"relationalOp")==0){
        strcpy(tempStr,temp->val.s);
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
    }
    else if(strcmp(root->name,"term")==0){
        temp = temp->child; // factor = arithmeticOrBooleanExpr/ var_id_num
        exp = generateExpression(temp);
        expTemp = exp;
        temp = temp->next;  
        if(temp!=NULL){
            while(expTemp->next!=NULL){
                expTemp = expTemp->next;
            }
            expTemp->next = generateExpression(temp); // N5
        }
    }
    else if(strcmp(root->name,"N5")==0){
        temp = temp->child; // op2
        strcpy(tempStr,temp->val.s); // op2
        exp = makeExpNode(0,tempStr,false,tempArrIndex);
        temp = temp->next;  
        exp->next = generateExpression(temp); // factor
        temp=temp->next;
        if(temp!=NULL)
        (exp->next)->next = generateExpression(temp); // N5
    }
    else if(strcmp(root->name,"var_id_num")==0){
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
    // printf("GeneratingExpression Ends: %s\n",root->name);
    else{
        // do nothin
    }
    return exp;
}