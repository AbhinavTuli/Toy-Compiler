#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#define NTSIZE 	30
#define TSIZE	20
#include "test3.c"
//#include "lexerDef.h"
// #include "test3.c"
// const char* getfield(char* line, int num){
//     const char* tok;
//     for (tok = strtok(line, "<"); tok && *tok; tok = strtok(NULL, ",\n"))
//     {
//         if (!--num)
//             return tok;
//     }
//     return NULL;
// }

// struct rule{
//     token nt;
//     token** heads;
// };


const char *readLine(FILE *file, char* line) {

    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = 128;
    char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += 128;
            lineBuffer = realloc(lineBuffer, maximumLineLength);
            if (lineBuffer == NULL) {
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    //char line[count + 1];
    strncpy(line, lineBuffer, (count + 1));
    free(lineBuffer);
    return line;

}


int main(){
    //printf("%d\n",grammarLength);
    int NTnum=0;
    char c;
    char prev[NTSIZE];
    FILE *ptr = fopen("grammar.txt","r");
    // fscanf(ptr, "%c", c); 
    char linebuffer[256];
    int f=0;
    struct ruleToken *rtPrev=NULL;
    while (!feof(ptr)) {
        const char *line = readLine(ptr, linebuffer);
        if(f==1){
            f=0;
            continue;
        }
        f=1;
        int rhsCount=0;
        int i=0;
        int s=0;
        int start=1;
        char startNT[NTSIZE];
        char rhs[NTSIZE];
        int j=0;
        while(i<256){
            //printf("%d\n",i);
            if(start==1){
                if(line[i]=='>'){
                    s=0;
                    start=0;
                    startNT[j]=0;
                    int x=strcmp(prev,startNT); //assuming strcmp with NULL is valid
                    strcpy(prev,startNT);
                    strcpy(grammar[NTnum].nt,startNT);
                    //printf("");
                    i++;
                    if(x != 0){
                        //printf("%s",startNT);
                        NTnum++;
                    }
                }
                if(s==1){
                    startNT[j]=line[i];
                    j++;
                    i++;
                }
                if((int)(line[i])>122 || (int)line[i]<32){
                    printf("line end\n");
                    break;
                }
                if(line[i]=='<'){
                    s=1;
                    j=0;
                    //printf("");
                    i++;
                }
                // else{
                //     printf("%c",line[i],i);
                //     i++;
                // }
            }
            else{
                
                if(line[i]=='>'){
                    s=0;
                    rhs[j]=0;
                    struct ruleToken rt;
                    rt.tag=0;
                    strcpy(rt.tnt,rhs);
                    if(rtPrev!=NULL){
                        rtPrev->next=&rt;
                    }
                    rtPrev=&rt;
                    if(rhsCount==0){
                        grammar[NTnum].numRules++; //assuming number of rules to be initialized to 0 
                        grammar[NTnum].heads[grammar[NTnum].numRules]=rt;
                    }
                    i++;
                    rhsCount++;
                }
                if(line[i]=='-'){
                    i+=5; //skipping arrow and 2 spaces after it
                }
                if(s==0){
                    j=0;
                    while(line[i]==' '){
                        i++;
                        s=1;
                }

                }
                
                if(line[i]=='-'){
                    i+=5; //skipping arrow and 2 spaces after it
                }

                if(s==1){
                    while(line[i]!=' '){
                        startNT[j]=line[i];
                        j++;
                        i++;
                    }

                    s=0;
                    rhs[j]=0;
                    struct ruleToken rt;
                    rt.tag=0;
                    strcpy(rt.tnt,rhs);
                    if(rtPrev!=NULL){
                        rtPrev->next=&rt;
                    }
                    rtPrev=&rt;
                    if(rhsCount==0){
                        grammar[NTnum].numRules++; //assuming number of rules to be initialized to 0 
                        grammar[NTnum].heads[grammar[NTnum].numRules]=rt;
                    }
                    rhsCount++;
                }
                if((int)(line[i])>122 || (int)line[i]<32){
                    break;
                }
                if(line[i]=='<'){
                    s=1;
                    j=0;
                    //printf("");
                    i++;
                }
                // else{
                //     printf("%c",line[i],i);
                //     i++;
                // }
            }
            
        }
        
        //printf("\n");
        //printf("%s\n", line);
}
    printf("%d",NTnum);
}