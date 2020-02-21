/*
 *	COMPILER PROJECT- ERPLAG COMPILER
 *	Batch Number 26
 *	Abhinav Tuli : 2017A7PS0048P
 *	Kushagra Raina : 2017A7PS0161P
 *  Tanmay Moghe : 2017A7PS0184P
 *  Rohit Bohra : 2017A7PS0225P
 *  Amratanshu Shrivastava : 2017A7PS0224P 
 */

#include<string.h> 
#include <stdio.h>
#include <stdbool.h>
#include "lexer.h"
#include "lexerDef.h"
#include <errno.h>

#define report_error(s, e) {
    errno=(e); 
    perror((s)); 
    exit((e)); 
}

#define BUFFER_MAX 100
#define MAX_IDENTIFIER_LENGTH 20
int curr_state=0;
int buffPtr=0;
int linePosition=0;
int lineNumber=1;
char* buff[BUFFER_MAX];
token* currentToken = NULL;
token* prevToken = NULL;

FILE *getStream(FILE *fp)
{
    char ch;
    int count=0;
    
    memset(buff,' ',sizeof(char)*BUFFER_MAX);
    
    while ( (ch = fgetc(fp)) != EOF && count<BUFFER_MAX)
    {
        if(ch=='\n')
            lineNumber++;
        buff[count] = ch;
        count++;   
    }
    return fp;
}

void removeComments(char *testcaseFile, char *cleanFile)
{
    int len;
    len = strlen(testcaseFile);
    int i=0;
    char c;
    char result[len];
    int j=0;
    char temp,next,tempNext;
    while(i<len) //i++ in the end
    {
        c=testcaseFile[i];
        
        if(i+1 == len)
        break;
        
        next=testcaseFile[i+1];
        
        if(c=='*' && next=='*') //comment starting
        {

            printf("Comment Start Found\n");

            //start reading till **
            //start reading from the character just after the *

            //put a check on i+2 and i+3
            i=i+2;
            //j=j+2;
            if(i == len)
                break;
            
             
            

            while(i<len)
            {
                c = testcaseFile[i];  
                if(c=='\n')
                {
                    result[j]='\n';
                    j++; //to keep the number of lines same
                }   

                //if next ele is there
                if(i+1 == len)
                {
                    break;
                }
                
                next = testcaseFile[i+1];
                if(c=='*' && next=='*') //comment end found
                {   

                    printf("Comment End Found\n");

                    i=i+2;
                    break;
                }    
            

                i++; //j++;  
            }
            
        }
        else //not a comment
        {
            result[j] = testcaseFile[i]; //copy as it is
            i++;
            j++;        
        }
        
    
    } //end of while

    
    for(int i=0;i<len;i++)
    {
        if(result[i] == '\0')
            break;
        
        cleanFile[i]=result[i];

    
    }
    
}

//getNextToken( ): This function reads the input character stream and uses efficient mechanism to recognize lexemes.
// The function tokenizes the lexeme appropriately and returns all relevant information it collects in this phase
// (lexical analysis phase) encapsulated as tokenInfo and passes to the parser on demand. 
//The function also displays all lexical errors appropriately.
token* checkASSIGNOP(){
    if(buff[buffPtr]=='='){
        buffPtr++;
        token* n = retToken();
        n->tokterm = ASSIGNOP;
        n->val = "="; 
        return n; 
    }
    else{
        return retToken(":",COLON);
    }
}


token* checkEQ(){
    if(buff[buffPtr]=='='){
        buffPtr++;
        return retToken("==",EQ);
        return n;
    }
    else {
        printf("Lexical Error on line no. %d, character %d",lineNumber, linePosition); 
        return NULL;
    }
}


token* checkGT(){
    if(buff[buffPtr]=='='){
        return retToken(">=",EQ);
    }
    else if(buff[buffPtr]=='>'){
        buffPtr++;
        return retToken(">>",ENDDEF);
    }
    else{
        return retToken(">",GT);
    }
}

void getNextToken()
{   
    char curr = buff[buffPtr];
    buffPtr++;
    
    switch(curr){
        case ':' :  checkASSIGNOP();
                    break;
                    
        case '=' :  checkEQ();
                    break;

        case '>' :  checkGT();
                    break;

        case '<' :  checkLT();
                    break;
                    
        case '\t':  //it will fall through this case (no break statement)
              
        case ' ' :  while(buff[buffPtr]==' ' || '\t')
                    {
                        buffptr++;
                    }
                    break;

        case '\n' : lineNumber++;
                    buffPtr++;
                    break;

        case '+':   return retToken("+",PLUS);
                    break;

        case '-':   return retToken("-",MINUS);
                    break;

        case '/':   return retToken("/",DIV);
                    break;

        case '*':   return retToken("*",MUL); // Change because comment ** can also be there
                    break;

        case '[':   return retToken("[",SQBO);
                    break;       
                 
        case ']':   return retToken("]",SQBC);
                    break;

        case '(':   return retToken("(",BO);
                    break;

        case ')':   return retToken(")",BC);
                    break;
        case ';':   return retToken(";",SEMICOL);
                    break;
        case ',':   return retToken(",",COMMA);          
                    break;
                    
        default:            
                    if(('a'<=curr && curr<'z') || ('A'<=curr && curr<='Z'))    // First character should be [a-z][A-Z]
                    {
                        char identifier[MAX_IDENTIFIER_LENGTH];      // Check for identifier(max_size = 20)
                        identifier[0] = curr;

                        curr = buff[++buffPtr];
                        int len = 1;

                        // Identifier : [a-z|A-Z][a-z|A-Z|0-9|_]*

                        while (('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9') || (curr=='_') ){

                            if(len==MAX_IDENTIFIER_LENGTH){
                                // Report lexical error 
                                report_error("Lexical Error : Exceeding identifier's maximum length\n",1); // Assign error codes for lexical,syntax.. error. Here lexical error code=1!
                            }

                            identifier[len++] = curr;
                            curr = buff[++buffPtr];
                            // add to table?
                        }

                        return  retToken(identifier,"ID");         // identifier code ends
                    }
                    else if('0'<=curr && curr<='9'){

                    }

                    

    }
    buffptr=0; // What's this?
}

token* retTokenALL(char* value,term t,int choice)
{   
    token* newToken = (token*)malloc(sizeof(token));

    switch (choice)
    {
    case 1: // string value
        strcpy(newToken->val.s, value);    // n->val=v;
        break;
    case 2: // Integer value
        // convert to Integer first
        int temp = 1;
        for()
        break;
    case 3: // REAL value
        // convert to REAL first
        break;
    case 4: // Boolean value
        if(strcmp(value,"true")==0)
        newToken->val.b = true;
        else
        newToken->val.b = false;
        break;
    default:
        break;
    }

    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    if(prevToken!=NULL){
        prevToken->next = newToken;
    }
    prevToken=newToken;
    
    return newToken;
}


token* retToken(char* value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    strcpy(newToken->val.s, value);    // n->val=v;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    if(prevToken!=NULL){
        prevToken->next = newToken;
    }
    prevToken=newToken;
    
    return newToken;
}

token* retTokenINT(int value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.i = value;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    if(prevToken!=NULL){
        prevToken->next = newToken;
    }
    prevToken=newToken;
    
    return newToken;
}

token* retTokenREAL(float value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.f = value;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    if(prevToken!=NULL){
        prevToken->next = newToken;
    }
    prevToken=newToken;
    
    return newToken;
}

token* retTokenBOOL(bool value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.b = value;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    if(prevToken!=NULL){
        prevToken->next = newToken;
    }
    prevToken=newToken;
    
    return newToken;
}

