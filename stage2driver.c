 /*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include "parser.c"
#include "ast.c"
#include "symbolTable.c"
#include "constructST.c"

#define FILELEN 10000

int main(int argc, char *argv[]){

    // strcpy(argv[1],"t7.txt");
    // strcpy(argv[2],"a.txt");
    
    FILE* fp1;
    FILE* fp2;

    printf("Work Status\n");
    printf("0. Exit\n");
    printf("1. Print Token List\n");
    printf("2. Print Parse Tree\n");
    printf("3. Print Abstract Syntax Tree(InOrderTraversal)\n");
    printf("4. Display Allocated Memory\n");
    printf("5. Printing the Symbol Table\n");
    printf("6. Printing the Total Memory Requirement\n");
    printf("7. Display all Static and Dynamic Arrays!\n");
    printf("8. Compute Error Report and Compile Time\n");
    printf("9. Generate Assembly Code!");
    printf("\n\n");

    int choice,i=0;

    clock_t ticks;

    // printf(" Enter Integer Corresponding to what you want to do\n 0 => Exit\n 1 => Removal of Comments\n 2=> Print Token List\n 3=> Verify Syntactic Correctness and Print Parse Tree\n 4=> Print total time required for total code\n");
    printf("Please Enter Choice : \t");
    printf("\n");
    scanf("%d",&choice);
    while(choice!=0)
    {   
        fp1 = fopen(argv[1],"r");
        fp2 = fopen(argv[2],"w");

        if(fp1==NULL){
            printf("Enter a valid TestFile\n");
            exit(0);
        }

        printFlag = true;

        switch(choice)
        {
             case 1:    ;
                        token* traverse = lexerRun(fp1);
                        traverse = traverse->next;

                        while(traverse->tokterm != 60)
                        {
                            if(traverse->tag == 1)
                                printf("%d\t%d\t\t%s\n",traverse->lineno,traverse->val.i,getLexeme(traverse->tokterm));
                            if(traverse->tag == 2)
                                printf("%d\t%f\t\t%s\n",traverse->lineno,traverse->val.f,getLexeme(traverse->tokterm));
                            if(traverse->tag == 3)
                                printf("%d\t%d\t\t%s\n",traverse->lineno,traverse->val.b,getLexeme(traverse->tokterm));
                            if(traverse->tag == 4)
                                printf("%d\t%s\t\t%s\n",traverse->lineno,traverse->val.s,getLexeme(traverse->tokterm));

                            traverse = traverse->next;
                        }

                        printf("%d\t%s\t%s\n",traverse->lineno,traverse->val.s,getLexeme(traverse->tokterm));

                        lexerFree();
                        break;
            
            case 2:     ;               // parsing to verify the syntactic correctness of the input source code and to produce parse tree 
                        printf("\n\nParse Tree : \n\n");             
                        // return  number of nodes and allocated memory, not void!!!         
                        runParser(fp1);         
                        // only display parse tree here!!!
                        lexerFree(); 
                        break;

            case 3:     ;               // For printing the Abstract Syntax Tree 
                        printf("\n\nAST Tree : \n\n"); 
                        // return  number of nodes and allocated memory, not void!!!
                        printFlag = true;
                        runAST(fp1);
                        lexerFree();      
                        break;  
                                    
            case 4:     ;              // For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree
                        printFlag = false;
                        int AstNodes = runAST(fp1);
                        lexerFree();    
                        int parseTreeMemory = numParseNodes*sizeof(struct treeNode);
                        printf("\nParse Tree Number Of Nodes = %d \t Allocated Memory = %d bytes\n\n",numParseNodes , parseTreeMemory);
                        int astTreeMemory = AstNodes*sizeof(struct astNode);
                        printf("AST Number Of Nodes = %d \t Allocated Memory = %d bytes\n\n",AstNodes ,astTreeMemory); 
                        
                        printf("Compression percentage : %f\n\n",((float)(parseTreeMemory-astTreeMemory)/(parseTreeMemory)*100));

                        break;

            case 5:     ;
                        printFlag = true;
                        runConstructST(fp1,fp2);
                        lexerFree();
                        break;
                        
                        break;  

            case 6:
                        break;

            case 7:
                        break;

            case 8:     ;   


                            //  For compiling to verify the syntactic and semantic correctness of the input source code 
                            // If the code is syntactically incorrect, report all syntax errors only. If the code is syntactically correct, 
                            //then report all type checking and semantic errors. Also print (on the console) the total time taken by your integrated compiler. 
                            //Print both total_CPU_time and total_CPU_time_in_seconds (as mentioned earlier)
                        break;
            
            case 9:                     //Produce Assembly Code
                        break;  
            default:
                    printf("\nEnter a Valid Choice!\n");
                    break;
        }
        printf("\n\nEnter Choice again : \t");
        scanf("%d",&choice);
	printf("\n");
    }
}   
