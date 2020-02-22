/*
 *  COMPILER PROJECT- ERPLAG COMPILER
 *  Batch Number 26
 *  Abhinav Tuli : 2017A7PS0048P
 *  Kushagra Raina : 2017A7PS0161P
 *  Tanmay Moghe : 2017A7PS0184P
 *  Rohit Bohra : 2017A7PS0225P
 *  Amratanshu Shrivastava : 2017A7PS0224P 
 */

#include<string.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "lexer.h"
#include "lexerDef.h"
#include <errno.h>

// #define report_error(s, e) {
//     errno=(e); 
//     perror((s)); 
//     exit((e)); 
// }

#define BUFFER_MAX 100
#define MAX_IDENTIFIER_LENGTH 20
FILE* programFile;

int curr_state=0;
int buffPtr=0;
int linePosition=0;
int lineNumber=1;
char buff[BUFFER_MAX];
int bcount = 0;

token* currentToken;
token* prevToken = NULL;
token* head; 

FILE *getStream(FILE *fp)
{
    printf("In getStream\n");
    char ch;
    int count=0;
    
    if(buffPtr > BUFFER_MAX - 20)
    {   
        printf("In getStream if \n");
        char temp[20];
        int i = 0;

        while(buffPtr != BUFFER_MAX)
        {
            temp[i] = buff[buffPtr];
            i++;
            buffPtr++;
        }           

        memset(buff,' ',sizeof(char)*BUFFER_MAX);

        while(count<i)
        {
            buff[count] = temp[i];
            count++;
        }
    }
    else{
        printf("In getStream else\n");
        memset(buff,' ',sizeof(char)*BUFFER_MAX);
    }
    printf("After if\n");
    //printf("%d\n",count);
    while (count<BUFFER_MAX )
    {   
        printf("before\n");
        ch = fgetc(fp);
        printf("%c",ch);
        // if(ch==EOF){
        //     break;
        // }
        //printf("In while\n");
        if(ch=='\n')
            lineNumber++;
        buff[count] = ch;
        count++;  
        printf("%d\n",count); 
    }
    // for(int i=0;i<100;i++){
    //     printf("1 %d %c\n", i, buff[i]);
    // }
    printf("After while %d",count);
    buffPtr = 0;
    return fp;
}

token* retTokenINT(int value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.i = value;
    newToken->tag = 1;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    prevToken->next = newToken;
    prevToken=newToken;
    
    return newToken;
}

token* retTokenREAL(float value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.f = value;
    newToken->tag = 2;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    prevToken->next = newToken;
    prevToken=newToken;
    
    return newToken;
}

token* retTokenBOOL(bool value,term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    newToken->val.b = value;
    newToken->tag = 3;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    prevToken->next = newToken;
    prevToken=newToken;
    
    return newToken;
}


token* retTokenSTR(char value[],term t)
{   
    token* newToken = (token*)malloc(sizeof(token));

    strcpy(newToken->val.s,value);
    newToken->tag = 4;
    newToken->tokterm=t;   
    newToken->prev = prevToken;
    newToken->next = NULL;
    newToken->lineno = lineNumber;

    prevToken->next = newToken;
    prevToken=newToken;
    
    return newToken;
}

token* checkASSIGNOP(){
    if(buff[buffPtr]=='='){
        buffPtr++;
        return retTokenSTR("=",ASSIGNOP); 
    }
    else{
        return retTokenSTR(":",COLON);
    }
}


token* checkEQ(){
    if(buff[buffPtr]=='='){
        buffPtr++;
        return retTokenSTR("==",EQ);
    }
    else {
        printf("Lexical Error on line no. %d, character %d",lineNumber, linePosition); 
        return NULL;
    }
}

token* checkGT(){
    if(buff[buffPtr] == '='){
        buffPtr++;
        return retTokenSTR(">=",GE);
    }
    else if(buff[buffPtr]=='>'){
        buffPtr++;

        if(buff[buffPtr] == '>' )
        {
            buffPtr++;
            return retTokenSTR(">>>",DRIVERENDDEF);
        }

        return retTokenSTR(">>",ENDDEF);
    }
    else{
        return retTokenSTR(">",GT);
    }
}

token* checkLT(){
    if(buff[buffPtr] == '='){
        buffPtr++;
        return retTokenSTR("<=",LE);
    }
    else if(buff[buffPtr]=='<'){
        buffPtr++;

        if(buff[buffPtr] == '<')
        {
            buffPtr++;
            return retTokenSTR("<<<",DRIVERDEF);
        }

        return retTokenSTR("<<",DEF);
    }
    else{
        return retTokenSTR("<",GT);
    }
}

token* checkRANGEOP(){
    if(buff[buffPtr]=='.'){
        buffPtr++;
        return retTokenSTR("..",RANGEOP);
    }
    else {
        printf("Lexical Error on line no. %d, character %d",lineNumber, linePosition); 
        return NULL;
    }
}

int isAlpha(char c) 
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return 1;
    }
    return 0;
}

int isDigit(char c) 
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    return 0;
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
void getNextToken()
{   
    if(buffPtr > BUFFER_MAX - 20){
        programFile = getStream(programFile);
       // buffPtr=0; 
    }
    
    //  for(int i=0;i<100;i++){
    //      printf("%d %c\n", i, buff[i]);
    //  }

    if(prevToken == NULL)
    {
        prevToken = (token *)malloc(sizeof(token));

        prevToken->tokterm = HEAD;
        prevToken->lineno = -1;
        prevToken->prev = NULL;
        prevToken->next = NULL;
        prevToken->val.i = -1;
        prevToken->tag = 1;

        head = prevToken;
        printf("head assigned\n");
    }

    currentToken = head;
    char curr = buff[buffPtr];
    buffPtr++;
    
    bool tokenGet = false;
    while(!tokenGet)
    {
        switch(curr){
        case ':' :  currentToken = checkASSIGNOP();
                    tokenGet = true;
                    break;
                    
        case '=' :  currentToken = checkEQ();
                    tokenGet = true;
                    break;

        case '>' :  currentToken = checkGT();
                    tokenGet = true;
                    break;

        case '<' :  currentToken = checkLT();
                    tokenGet = true;
                    break;
                    
        case '\t':  //it will fall through this case (no break statement)
              
        case ' ' :  while(buff[buffPtr]==' ' || buff[buffPtr]=='\t')
                    {
                        buffPtr++;
                    }
                    tokenGet=true;
                    break;

        case '\n' : lineNumber++;
                    //buffPtr++;
                    tokenGet=true;
                    break;

        case '+':   currentToken = retTokenSTR("+",PLUS);
                    tokenGet = true;
                    break;

        case '-':   currentToken = retTokenSTR("-",MINUS);
                    tokenGet = true;
                    break;

        case '/':   currentToken = retTokenSTR("/",DIV);
                    tokenGet = true;
                    break;

        case '*':   currentToken = retTokenSTR("*",MUL); // Change because comment ** can also be there
                    tokenGet = true;
                    break;

        case '[':   currentToken = retTokenSTR("[",SQBO);
                    tokenGet = true;
                    break;       
                 
        case ']':   currentToken = retTokenSTR("]",SQBC);
                    tokenGet = true;
                    break;

        case '(':   currentToken = retTokenSTR("(",BO);
                    tokenGet = true;
                    break;

        case ')':   currentToken = retTokenSTR(")",BC);
                    tokenGet = true;
                    break;

        case ';':   currentToken = retTokenSTR(";",SEMICOL);
                    tokenGet = true;
                    break;

        case ',':   currentToken = retTokenSTR(",",COMMA);
                    tokenGet = true;          
                    break;
                    
        case '.':   currentToken = checkRANGEOP();
                    tokenGet = true;
                    break;

        default : 
                    if(isAlpha(curr))
                    {
                        int count = 0;
                        char tempchar = curr;
                        //printf("abc %c\n",tempchar);
                        char id[21];
                        memset(id,'\0',21*sizeof(char));
                        id[count] = tempchar; 
                        //buffPtr++;
                        tempchar = buff[buffPtr];
                        while(isAlpha(tempchar) || isDigit(tempchar) || tempchar == '_')
                        {
                            count++;
                            id[count] = tempchar;

                            if(count>=20)
                            {
                                printf("Lexical Error on line no. %d, character %d",lineNumber, linePosition); 
                                currentToken = NULL;
                                break;
                            }

                            buffPtr++;
                            tempchar = buff[buffPtr];
                        }

                        currentToken = retTokenSTR(id,ID);
                        tokenGet = true;
                        break;
                    }           

                    else if(isDigit(curr))
                    {
                        char num1[21];
                        memset(num1,'\0',21*sizeof(char));
                        
                        int count = 0;
                        num1[count] = curr;

                        buffPtr++;
                        char tempchar = buff[buffPtr];

                        while(isDigit(tempchar))
                        {
                            count++;
                            num1[count] = tempchar;

                            buffPtr++;
                            tempchar = buff[buffPtr];
                        }

                        if(tempchar == '.')
                        {
                            count++;
                            num1[count] = tempchar;

                            buffPtr++;
                            tempchar = buff[buffPtr];

                            if(tempchar == '.')
                            {
                                buffPtr--;
                                buffPtr--;

                                num1[count] = '\0';

                                currentToken = retTokenINT(atoi(num1),INTEGER);
                                tokenGet = true;
                                break;
                            }

                            while(isDigit(tempchar))
                            {
                                count++;
                                num1[count] = tempchar;

                                buffPtr++;
                                tempchar = buff[buffPtr];
                            }

                            if(tempchar == 'e' || tempchar == 'E')
                            {
                                count++;
                                num1[count] = tempchar;

                                buffPtr++;
                                tempchar = buff[buffPtr];

                                if(tempchar == '+' || tempchar == '-')
                                {
                                    count++;
                                    num1[count] = tempchar;

                                    buffPtr++;
                                    tempchar = buff[buffPtr];
                                }

                                while(isDigit(tempchar))
                                {
                                    count++;
                                    num1[count] = tempchar;

                                    buffPtr++;
                                    tempchar = buff[buffPtr];
                                }

                                float fRet = 0;
                                char num2[21];
                                memset(num2,'\0',21*sizeof(char));

                                int trav=0;
                                while(num1[trav] != '.')
                                {
                                    num2[trav] = num1[trav];
                                    trav++;
                                }

                                fRet = fRet + atoi(num2);

                                trav++;
                                int j = 0;
                                memset(num2,'\0',21*sizeof(char));

                                while(!(num1[trav]=='E' || num1[trav]=='e'))
                                {
                                    num2[j] = num1[trav];
                                    trav++;
                                    j++; 
                                }

                                int l = log10(atoi(num2));
                                fRet = fRet + atoi(num2)/pow(10,l+1);

                                trav++;
                                j=0;

                                bool plus = true;

                                if(num1[trav] == '+' || num1[trav] == '-')
                                {
                                    trav++;

                                    if(num1[trav] == '-')
                                        plus = false;
                                }

                                while(num1[trav] != '\0')
                                {
                                    num2[j] = num1[trav];
                                    j++;
                                    trav++;
                                }

                                int raise;
                                if(plus)
                                    raise = atoi(num2);
                                else
                                    raise = 0 - atoi(num2);

                                fRet = fRet*pow(10,raise);

                                currentToken = retTokenREAL(fRet,REAL);
                                tokenGet = true;
                                break;
                            }   

                            else
                            {
                                float fRet = 0;
                                char num2[21];
                                memset(num2,'\0',21*sizeof(char));

                                int trav=0;
                                while(num1[trav] != '.')
                                {
                                    num2[trav] = num1[trav];
                                    trav++;
                                }

                                fRet = fRet + atoi(num2);

                                trav++;
                                int j = 0;
                                memset(num2,'\0',21*sizeof(char));

                                while(!(num1[trav]=='\0'))
                                {
                                    num2[j] = num1[trav];
                                    trav++;
                                    j++; 
                                }

                                int l = log10(atoi(num2));
                                fRet = fRet + atoi(num2)/pow(10,l+1);

                                currentToken = retTokenREAL(fRet,REAL);
                                tokenGet = true;
                                break;
                            }
                        }
                        else
                        {
                            currentToken = retTokenINT(atoi(num1),INTEGER);
                            tokenGet = true;
                            break;
                        }
                    }
        }
    }
}

int main(){
    programFile=fopen("lexerTest.txt","rb");
    programFile=getStream(programFile);
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    getNextToken();
    
    getNextToken();
    

    token* trav = head;

    while(trav!=NULL)
    {
        printf("%d  %s\n",trav->tokterm,trav->val.s);
        trav = trav->next;
    }

    //printf("%d   buffptr",buffPtr);
}