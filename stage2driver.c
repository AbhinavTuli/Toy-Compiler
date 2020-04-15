/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
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


#define FILELEN 10000

int main(int argc, char *argv[]){

    // strcpy(argv[1],"t7.txt");
    // strcpy(argv[2],"a.txt");
    
    FILE* fp1;
    FILE* fp2;

    printf("Work Status\n");
    printf("1. Generate Token List!\n");
    printf("2. Generate Parse Tree!\n");
    printf("3. Generate AST!\n");
    printf("4. Display Allocated Memory! \n");
    printf("5. Generate Symbol Table!\n");
    printf("6. Display Record Size!\n");
    printf("7. Display all Static and Dynamic Arrays!\n");
    printf("8. Generate Error Report and Compile Time\n");
    printf("9. Generate Assembly Code!");

    printf(" Enter 0 to exit!!!");
    // printf("1. FIRST and FOLLOW set automated. Function is computeFirstAndFollow() in parser.c\n");
    // printf("2. Both Lexical and Syntax analysis modules implemented. \n");                              // token list?
    // printf("3. Modules compile and work with all test cases.\n");
    // printf("4. Parse Tree Constructed for all syntactically correct cases.\n");                          // case 2 now
    // printf("5. Both lexical and syntactical errors are printed with line numbers.\n");
    // printf("6. Error Recovery is working");
    printf("\n\n");
    // case 3-ast
    // case 4-memory allocated in parse tree and ast respectively
    // case 5-printing symbol table 


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

        removeComments(fp1);                            // removing comments
        switch(choice)
        {
             case 1:  ;
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
                        printFlag = true;           
                        
                        // return  number of nodes and allocated memory, not void!!!         
                        runParser(fp1,fp2);         
                        // only display parse tree here!!!

                        lexerFree(); 
                        break;

            case 3:     ;               // For printing the Abstract Syntax Tree 
                        printFlag = true;

                        // return  number of nodes and allocated memory, not void!!!
                        runAST(fp1,fp2);
                        // only display AST here!!!

                        lexerFree();      
                        break;  
                                    
            case 4:     ;               // For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree
                        printf("Parse Tree Number Of Nodes = %d \t Allocated Memory = %d", , );
                        printf("AST Number Of Nodes = %d \t Allocated Memory = %d", ,); 
                        // printFlag = false;
                        // ticks = clock();
                        // runParser(fp1,fp2);
                        // lexerFree();
                        // ticks = clock() - ticks; 
                        // double time_taken = ((double)ticks)/CLOCKS_PER_SEC;
                        // printf("Total Clock ticks : %ld\n",ticks);
                        // printf("Total time taken by parser : %f seconds\n", time_taken);
                        // break;

            case 5:     
                        
                        break;  

            case 6:
                        break;

            case 7:
                        break;

            case 8:                     //  For compiling to verify the syntactic and semantic correctness of the input source code 
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
