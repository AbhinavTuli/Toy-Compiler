#include <stdlib.h>
#include <stdio.h>
#define NTSIZE 		30
#define TSIZE		20

char table[NTSIZE][TSIZE];

int hashfunc(char *nonterm)
{
    char c;
    int sum=0;
    while(c=*nonterm++)
    {
        sum+=(int)c;
    }
    return sum/131;
}

int main(){
    FILE *ptr;
    char c;
    char str[20];
    memset(str,'\0',20);
    ptr = fopen("grammar.txt","r");
    fscanf(ptr, "%c", c);
    while(ptr!=EOF)
    {
        if(c=='<')
        { 
            int i=0;
            str[i] = c;
            i++;
            while(c!='>')
            {
                fscanf(ptr, "%c", c);
                str[i] = c;
                i++;
            }
            str[i] = '>';
            printf("%d\n",hashfunc(str));
            memset(str,'\0',20);
        }
        fscanf(ptr,"%c", c);    
    }
    fclose(ptr);
}
// int main(){
//     char nonterminal;
//     char line[];
//     int i=0;
//     scanf("%s",line);
//     while(line[i]!='\n')
//     {
//         if(line[i]=='<')
//         {
//             while(line[i]!='>')
//             {
//                 nonterminal = 
//             }
//                 break;
//         }
//         terminal = 
//     }
    
// }