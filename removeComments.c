#include <stdlib.h>
#include<stdio.h>
void removeComments(FILE* fptr)
{   
    
    char next=fgetc(fptr);
    char c=next;
    while (c != EOF) 
    { 
        c = next; 
        next=fgetc(fptr);
        if(next==EOF){
            break;
        }
        if(c=='*' && next=='*') //comment starting
        {

            printf("Comment Start Found\n");          
            while(c!=EOF)
            { 
                if(c=='\n')
                {
                    printf('\n');
                    //j++; //to keep the number of lines same
                }   

                
                next = fgetc(fptr);
                if(next==EOF){
                    break;
                }
                if(c=='*' && next=='*') //comment end found
                {   

                    printf("Comment End Found\n");

                    break;
                }    
                // else{
                //     printf("%c%c",c,next);
                // }
                c=next;
            }
            
        }
        else //not a comment
        {
            printf("%c%c",c,next);        
        }
    } 
  
    fclose(fptr); 
    
}
int main(){
    FILE* fp=fopen("t2.txt","rb");
    removeComments(fp);
}