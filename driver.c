#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include lexer.h
#include parser.h
#include lexerDef.h
#define FILELEN 10000

int main(char* testfile, char* parsefile){

    // FILE* fp = fopen(argv[1],"r");
    int choice,i=0;

    char teststring[FILELEN];
    char outstring[FILELEN];
    memset(teststring, '\0', FILELEN);
    
    FILE* ptr = open(testfile,'r');
    
    while((ch = fgetc(ptr)) != EOF)
    {
        teststring[i] = ch;
        i++;
    }

    clock_t start_time_lexer, end_time_lexer,start_time_parser, end_time_parser, total_time;
    double total_CPU_time_lexer,total_CPU_time_parser, total_CPU_time_in_seconds_lexer, total_CPU_time_in_seconds_parser;

    printf("Please Enter Choice:");
    scanf("%d",&choice);
    while(choice!=0)
    {
        switch(choice)
        {

            case 1:     removeComments(teststring, outstring);
                        break;

            case 2:     start_time_lexer = clock();

                        while(token->next!=NULL)
                        {
                            switch(token->tag)
                            {
                                case 1:     printf("\t%d\t%d\t%s",token->lineno,token->val,GetLexeme(token->tokterm));
                                                break;
                                case 2:     printf("\t%d\t%f\t%s",token->lineno,token->val,GetLexeme(token->tokterm));
                                                break;
                                case 3:     printf("\t%d\t%d\t%s",token->lineno,token->val,GetLexeme(token->tokterm));
                                                break;
                                case 4:     printf("\t%d\t%c\t%s",token->lineno,token->val,GetLexeme(token->tokterm));
                                                break;
                            }

                        }
                        end_time_lexer = clock();
                        total_CPU_time_lexer = end_time_lexer - start_time_lexer ;
                        total_CPU_time_in_seconds_lexer = total_CPU_time_lexer/CLOCKS_PER_SEC;
                        break;

            case 3:     start_time_parser = clock();
                        retTokenALL(char* value,term t,int choice);
                        end_time_parser = clock();
                        total_CPU_time_parser = end_time_parser - start_time_parser ;
                        total_CPU_time_in_seconds_parser = total_CPU_time_parser/CLOCKS_PER_SEC;
                        break;  
                                    
            case 4:     if(total_CPU_time_in_seconds_lexer==0)
                        {
                            printf("\nLexer not yet run");
                        }
                        else 
                        {
                            printf("\n Total time taken by the lexer is :%d",total_CPU_time_in_seconds_lexer);
                        }
                        if(total_CPU_time_in_seconds_parser==0)
                        {
                            printf("\nParser not yet run");
                        }
                        else 
                        {
                            printf("\n Total time taken by the parser is :%d",total_CPU_time_in_seconds_lexer);
                        }
        }
        printf("Enter Choice again");
        scanf("%d",&choice);
    }
}   