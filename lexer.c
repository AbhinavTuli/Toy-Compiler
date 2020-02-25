/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0225P
    Rohit Bohra             -   2017A7PS0225P
*/
#include "lexer.h"
#include "lexerDef.h"

int hash(char * str) {
    int hash = 401;
    while (*str != '\0') {
        hash = ((hash << 4) + (int)(*str)) % MAX_TABLE;
        str++;
    }

    return hash % MAX_TABLE;
}


void insertEntry(char* c,term te){
    if(keywords[hash(c)]==NULL){
        keywords[hash(c)]=(struct node*)malloc(sizeof(struct node));
        strcpy(keywords[hash(c)]->value,c);
        keywords[hash(c)]->next=NULL;
        keywords[hash(c)]->tt = te;
        
    }
    else{
        struct node* t;
        t=keywords[hash(c)];
        while(t->next!=NULL){
            t=t->next;
        }
        struct node* curr=(struct node*)malloc(sizeof(struct node));
        strcpy(curr->value,c);
        curr->next=NULL;
        t->next=curr;
        curr->tt = te;
    }
}

int findEntry(char* c){
    struct node*t=keywords[hash(c)];
    while(t!=NULL){
        if(strcmp(c,t->value)==0){
            return 1;
        }
        t=t->next;
    }
    return 0;
}
struct node* retNode(char *c)
{
    struct node*t=keywords[hash(c)];
    while(t!=NULL){
        if(strcmp(c,t->value)==0){
            return t;
        }
        t=t->next;
    }
    return t;
}
int printHashtable(){
    for(int i=0;i<MAX_TABLE;i++){
        if(keywords[i]!=NULL){
            printf("Hash entry %d\n",i);
            struct node* t=keywords[i];
            while(t!=NULL){
                printf("%s    %d\n",t->value,t->tt);
                t=t->next;
            }
        }
    }
}

void initializeHashtable(){

    for(int i=0;i<MAX_TABLE;i++){
        keywords[i]=NULL;
    }
}

const char* getLexeme(term t) 
{
   switch (t) 
   {
      case INTEGER: return "INTEGER"; 
      case REAL: return "REAL"; 
      case BOOLEAN : return "BOOLEAN"; 
      case OF : return "OF"; 
      case ARRAY : return "ARRAY"; 
      case START : return "START"; 
      case END : return "END"; 
      case DECLARE : return "DECLARE"; 
      case MODULE : return "MODULE";
      case DRIVER : return "DRIVER";
      case DRIVERDEF : return "DRIVERDEF";
      case DRIVERENDDEF : return "DRIVERENDDEF";
      case PROGRAM : return "PROGRAM";
      case TAGGED : return "TAGGED";
      case UNION : return "UNION";
      case GET_VALUE : return "GET_VALUE";
      case PRINT : return "PRINT";
      case USE : return "USE";
      case WITH : return "WITH";
      case PARAMETERS : return "PARAMETERS";
      case TRUE : return "TRUE";
      case FALSE :return "FALSE" ;
      case TAKES : return "TAKES";
      case INPUT : return "INPUT";
      case RETURNS : return "RETURNS";
      case AND : return "AND" ;
      case OR : return "OR";
      case FOR : return "FOR";
      case IN : return "IN" ; 
      case SWITCH : return "SWITCH";
      case CASE : return "CASE";
      case BREAK: return "BREAK";
      case DEFAULT : return "DEFAULT";
      case WHILE : return "WHILE";
      case PLUS : return "PLUS";
      case MINUS : return "MINUS";
      case MUL : return "MUL";
      case DIV : return "DIV";
      case LT : return "LT";
      case LE : return "LE";
      case GE : return "GE";
      case GT : return "GT";
      case EQ : return "EQ";
      case NE : return "NE";
      case DEF :  return "DEF";
      case ENDDEF :return "ENDDEF";
      case COLON : return "COLON";
      case RANGEOP : return "RANGEOP";
      case SEMICOL : return "SEMICOL";
      case COMMA : return "COMMA";
      case ASSIGNOP : return "ASSIGNOP";
      case SQBO : return "SQBO";
      case SQBC : return "SQBC";
      case BO : return "BO";
      case BC : return "BC";
      case COMMENTMARK : return "COMMENTMARK";
      case ID : return "ID";
      case NUM : return "NUM";
      case RNUM : return "RNUM";
      case $ : return "$";
      case e: return "ε";
      case HEAD : return "HEAD";
      default : return "\0";
   }
}

void populateKeywordTable()
{
    initializeHashtable();

    insertEntry("integer",INTEGER);
    insertEntry("real",REAL);
    insertEntry("boolean",BOOLEAN);
    insertEntry("of",OF);
    insertEntry("array",ARRAY);
    insertEntry("start",START);
    insertEntry("end",END);
    insertEntry("declare",DECLARE);
    insertEntry("module",MODULE);
    insertEntry("driver",DRIVER);
    insertEntry("program",PROGRAM);
    insertEntry("record",RECORD);
    insertEntry("tagged",TAGGED);
    insertEntry("union",UNION);
    insertEntry("get_value",GET_VALUE);
    insertEntry("print",PRINT);
    insertEntry("use",USE);
    insertEntry("with",WITH);
    insertEntry("parameters",PARAMETERS);
    insertEntry("true",TRUE);
    insertEntry("false",FALSE);
    insertEntry("takes",TAKES);
    insertEntry("input",INPUT);
    insertEntry("returns",RETURNS);
    insertEntry("AND",AND);
    insertEntry("OR",OR);
    insertEntry("for",FOR);
    insertEntry("in",IN);
    insertEntry("switch",SWITCH);
    insertEntry("case",CASE);
    insertEntry("break",BREAK);
    insertEntry("default",DEFAULT);
    insertEntry("while",WHILE);
}


FILE *getStream(FILE *fp)
{
    //tempCount++;

    char ch;
    int count=0;
    
    if(buffPtr > BUFFER_MAX - 20)
    {   
        char temp[20];
        int i = 0;

        while(buffPtr != BUFFER_MAX)
        {
            temp[i] = buff[buffPtr];
            i++;
            buffPtr++;
        }           

        memset(buff,' ',sizeof(char)*BUFFER_MAX);
        int j=0;
        while(count<i)
        {
            buff[count] = temp[j];
            count++;
            j++;
        }
    }
    
    else{
        memset(buff,' ',sizeof(char)*BUFFER_MAX);
    }
    
    while (count<100 && ch!=EOF)
    {   
        ch = fgetc(fp);
   
        buff[count] = ch;
        count++;  
    }

    if(ch == EOF)
    {
        if(!endfile)
        {
        if(count != BUFFER_MAX)
        {
            endfile = true;
            buff[count-1] = '$';
        }
        }
    }

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
        return retTokenSTR(":=",ASSIGNOP); 
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
        error = true;
        return prevToken;
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
        error = true;
        return prevToken;  // No need of prevToken here, just needed something to return, not modifying prevtoken
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

void removeComments(FILE* fptr)
{   
    char curr = fgetc(fptr);
    char next = fgetc(fptr);;
    int flag = 0;

    while (curr != EOF) 
    { 
        if(next==EOF){
            printf("%c",curr);
            break;
        }

        if(flag==1 && next!='*'){
            printf("#%c",next);
        }

        flag=0;

        if(curr =='*' && next=='*') //comment starting
        {
            curr = fgetc(fptr);
            next = fgetc(fptr);

            // printf("\nComment Start Found\n");          
            while(curr!=EOF)
            { 
                if(curr=='\n')
                {
                    printf("\n");
                    //j++; //to keep the number of lines same
                }   

                if(next==EOF){
                    break;
                }
                if(curr=='*' && next=='*') //comment end found
                {   
                    // printf("\nComment End Found\n");
                    curr = fgetc(fptr);
                    next = fgetc(fptr);
                    break;
                }    
                curr = next;
                next = fgetc(fptr);
            }
        }
        else //not a comment
        {
            if(next=='*'){
                printf("%c",curr);
                flag = 1;
                curr = next;
                next = fgetc(fptr);
            }else{
                printf("%c%c",curr,next);
                curr = fgetc(fptr);
                next = fgetc(fptr);
            }
        }
    } 
  
    fclose(fptr); 
}

void getNextToken()
{   
    if(buffPtr > BUFFER_MAX - 20){
        programFile = getStream(programFile);
    }

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
                    
        case '=' :  ;token* ttoken = checkEQ();
                    if(error)
                    {
                        if(printFlag)
                        printf("Line %d: Lexical Error due to just a single equal to (=) sign.\n",lineNumber);

                        lexicalErrors[numLexicalErrors++] = lineNumber;

                        char tempChar=buff[buffPtr];
                        
                        while(tempChar!='\n' && tempChar!='$')
                        {    
                            buffPtr++;
                            if(buffPtr > BUFFER_MAX - 20)
                            programFile = getStream(programFile);
                            tempChar=buff[buffPtr];
                        }
                            
                        curr = tempChar;
                        buffPtr++;
                        error = false;
                        break;   
                    }
                    
                    else
                    {
                        currentToken = ttoken;
                        tokenGet = true;
                        break;
                    }

        case '>' :  currentToken = checkGT();
                    tokenGet = true;
                    break;

        case '<' :  currentToken = checkLT();
                    tokenGet = true;
                    break;

        case '$' :  currentToken = retTokenSTR("$",$);
                    tokenGet = true;
                    break;

        case '\r':  curr = buff[buffPtr];
                    buffPtr++;
                    break;
                    
        case '\t':  //it will fall through this case (no break statement)
              
        case ' ' :  while(buff[buffPtr]==' ' || buff[buffPtr]=='\t')
                    {    
                        buffPtr++;
                        
                        if(buffPtr > BUFFER_MAX - 20)
                            programFile = getStream(programFile);
                    }
                    curr = buff[buffPtr];
                    buffPtr++;

                    break;

        case '\n' : lineNumber++;
                    
                    if(buffPtr > BUFFER_MAX - 20)
                            programFile = getStream(programFile);

                    curr = buff[buffPtr];
                    buffPtr++;

                    break;

        case '*' :  if(buff[buffPtr] == '*')
                    {
                        buffPtr++;

                        bool comment = true;

                        while(comment)
                        {
                            if(buff[buffPtr] == '*')
                            {
                                if(buff[buffPtr+1] == '*')
                                {
                                    buffPtr++;
                                    buffPtr++;
                                    comment = false;
                                    //printf("   Comment end %c ",buff[buffPtr]);
                                    break;
                                }
                            }

                            else if(buff[buffPtr] == '\n')
                            {
                                lineNumber++;
                                //printf("3.  line No. = %d  \n",lineNumber);
                            }

                            buffPtr++;

                            if(buffPtr > BUFFER_MAX - 20)
                                programFile = getStream(programFile);
                        }

                        curr = buff[buffPtr];
                        buffPtr++;
                        //if(curr == '\n')
                          //  printf("WOOLOO\n");
                        break;
                    }
                    else
                    {
                        currentToken = retTokenSTR("*",MUL);
                        tokenGet = true;
                        break;
                    }
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
                    
        case '.':   ;ttoken = checkRANGEOP();
                    if(error)
                    {
                        if(printFlag)
                        printf("Line %d: Lexical Error due to Just a single dot (.).\n",lineNumber);
                        
                        lexicalErrors[numLexicalErrors++] = lineNumber;
                        char tempChar=buff[buffPtr];
                        
                        while(tempChar!='\n' && tempChar!='$')
                        {    
                            buffPtr++;
                            if(buffPtr > BUFFER_MAX - 20)
                            programFile = getStream(programFile);
                            tempChar=buff[buffPtr];
                        }
                            
                        curr = tempChar;
                        buffPtr++;
                        error = false;
                        break;   
                    }
                    
                    else
                    {
                        //printf("Before");
                        currentToken = ttoken;
                        //printf("After");
                        tokenGet = true;
                        break;
                    }

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
                        //printf("%c",tempchar);
                        while(isAlpha(tempchar) || isDigit(tempchar) || tempchar == '_')
                        {
                            count++;
                            id[count] = tempchar;

                            if(count>=20)
                            {
                                if(printFlag)
                                printf("Line %d: Lexical Error. Identifier cannot be more than 20 characters.\n",lineNumber);

                                lexicalErrors[numLexicalErrors++] = lineNumber; 
                                char tempChar=buff[buffPtr];
                                while(tempChar!='\n' && tempChar!='$'){
                                    buffPtr++;
                                    if(buffPtr > BUFFER_MAX - 20)
                                    programFile = getStream(programFile);
                                    tempChar=buff[buffPtr];
                                }
                                curr = tempChar;
                                buffPtr++;
                                break;
                            }

                            buffPtr++;
                            tempchar = buff[buffPtr];
                        }

                        if(count>=20)
                            break;

                        if(findEntry(id))
                        {
                            struct node* rN = retNode(id);
                            currentToken = retTokenSTR(id,rN->tt);
                            tokenGet = true;
                            break;
                        }
                        currentToken = retTokenSTR(id,ID);
                        tokenGet = true;
                        break;
                    }

                    else if(isDigit(curr))
                    {
                        //printf("WOOOLOOO\n");
                        int numct=0;
                        char num1[21];
                        memset(num1,'\0',21*sizeof(char));
                        
                        int count = 0;
                        num1[count] = curr;

                        //buffPtr++;
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
                                //buffPtr--;

                                num1[count] = '\0';

                                currentToken = retTokenINT(atoi(num1),NUM);
                                tokenGet = true;
                                break;
                            }

                            if(!isDigit(tempchar))
                            {
                                if(printFlag)
                                printf("Line %d: Lexical Error. There is no number after Decimal Point\n",lineNumber);

                                lexicalErrors[numLexicalErrors++] = lineNumber;
                                char tempChar1=buff[buffPtr];
                        
                                while(tempChar1!='\n' && tempChar1!='$')
                                {    
                                    buffPtr++;
                                    if(buffPtr > BUFFER_MAX - 20)
                                        programFile = getStream(programFile);
                                    tempChar1=buff[buffPtr];
                                }
                            
                                curr = tempChar1;
                                buffPtr++;
                                error = false;
                                break;
                            }

                            while(isDigit(tempchar))
                            {
                                numct++;
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
                                bool afterEnot = false;
                                if(!isDigit(tempchar))
                                    afterEnot = true;

                                while(isDigit(tempchar))
                                {
                                    count++;
                                    num1[count] = tempchar;

                                    buffPtr++;
                                    tempchar = buff[buffPtr];
                                }

                                double fRet = 0;
                                char num2[21];
                                memset(num2,'\0',21*sizeof(char));

                                int trav=0;
                                while(num1[trav] != '.')
                                {
                                    num2[trav] = num1[trav];
                                    trav++;
                                }
                                //printf("bc %s\n",num1);
                                fRet = fRet + atoi(num2);
                                //printf("bh %lf",fRet);
                                trav++;
                                int j = 0;
                                memset(num2,'\0',21*sizeof(char));

                                while(!(num1[trav]=='E' || num1[trav]=='e'))
                                {
                                    num2[j] = num1[trav];
                                    trav++;
                                    j++; 
                                }

                                if(atoi(num2)!=0)
                                {
                                int l = log10(atoi(num2));
                                fRet = fRet + atoi(num2)/pow(10,l+1);
                                }
                                
                                trav++;
                                j=0;

                                if(afterEnot)
                                {
                                    currentToken = retTokenREAL(fRet,RNUM);

                                    if(num1[trav] == '+' || num1[trav] == '-')
                                    {
                                        buffPtr--;
                                    }

                                    tokenGet = true;
                                    break;
                                }
                                bool plus = true;

                                if(num1[trav] == '+' || num1[trav] == '-')
                                {
                                    if(num1[trav] == '-')
                                        plus = false;
                                    trav++;
                                }
                                memset(num2,'\0',21*sizeof(char));
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
                                currentToken = retTokenREAL(fRet,RNUM);
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
                                if (atoi(num2) != 0)
                                {
                                int l = log10(atoi(num2));
                                fRet = fRet + atoi(num2)/pow(10,l+1);
                                }

                                currentToken = retTokenREAL(fRet,RNUM);
                                tokenGet = true;
                                break;
                            }
                        }
                        else
                        {
                            currentToken = retTokenINT(atoi(num1),NUM);
                            tokenGet = true;
                            break;
                        }
                    }
                    else{
                        if(printFlag)
                        printf("Line %d: Lexical Error due to Unexpected Character.\n",lineNumber);

                        lexicalErrors[numLexicalErrors++] = lineNumber;
                        char tempChar=buff[buffPtr];
                        //printf("%c ",tempChar);
                        while(tempChar!='\n' && tempChar!='$'){
                             buffPtr++;
                             if(buffPtr > BUFFER_MAX - 20)
                                 programFile = getStream(programFile);
                             tempChar=buff[buffPtr];
                             //printf("%c ",tempChar);
                        }
                        buffPtr++;
                        curr = tempChar;
                    }
        }
    }
}

token* lexerRun(FILE* fptr)
{
    programFile = fptr;
    programFile = getStream(programFile);


    if(first)
    {
        populateKeywordTable();
        first = false;
    }

    token* trav = head;

    while(true)
    {
        getNextToken();

        if(currentToken->tokterm == 60)
            break;
    }

    return head;
}

void lexerFree()
{
    prevToken = NULL;
    error = false;
    lineNumber=1;
    endfile = false;
    
}
