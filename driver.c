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
#include "symbolTable.c"
#include "parser.c"
#include "astNew.c"
#include "constructST.c"

#define FILELEN 10000

int main(int argc, char *argv[]){

    // strcpy(argv[1],"t7.txt");
    // strcpy(argv[2],"a.txt");
    
    FILE* fp1;
    FILE* fp2;

    printf("Work Status\n");
    printf("1. FIRST and FOLLOW set automated. Function is computeFirstAndFollow() in parser.c\n");
    printf("2. Both Lexical and Syntax analysis modules implemented. \n");
    printf("3. Modules compile and work with all test cases.\n");
    printf("4. Parse Tree Constructed for all syntactically correct cases.\n");
    printf("5. Both lexical and syntactical errors are printed with line numbers.\n");
    printf("6. Error Recovery is working");
    printf("\n\n");

    int choice,i=0;

    clock_t ticks;

    printf(" Enter Integer Corresponding to what you want to do\n 0 => Exit\n 1 => Removal of Comments\n 2=> Print Token List\n 3=> Verify Syntactic Correctness and Print Parse Tree\n 4=> Print total time required for total code\n");
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

        switch(choice)
        {
            case 1:     printf("File without comments from next line.\n");
                        removeComments(fp1);
                        break;
            
            case 2:     ;
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

            case 3:   
                        printFlag = true;
                        runParser(fp1,fp2);
                        lexerFree();
                        break;  
                                    
            case 4:     
                        printFlag = false;
                        ticks = clock();
                        runParser(fp1,fp2);
                        lexerFree();
                        ticks = clock() - ticks; 
                        double time_taken = ((double)ticks)/CLOCKS_PER_SEC;
                        printf("Total Clock ticks : %ld\n",ticks);
                        printf("Total time taken by parser : %f seconds\n", time_taken);
                        break;

            case 5:     // AST
                        printFlag = true;
                        runAST(fp1,fp2);
                        lexerFree();
                        break;  

            case 6:     // Symbol Table
                        printFlag = true;
                        runConstructST(fp1,fp2);
                        lexerFree();
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
