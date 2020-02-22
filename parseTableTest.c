#include<string.h>
#include<stdio.h>
#include "parser.h"



void createParseTable(struct ntfirstFollow firstFollowSets[MAX_NON_TERMINALS], struct ntRules grammar[MAX_TERMINALS]) //, int[][] Table)
{
    for(int i=0; i<MAX_NON_TERMINALS; i++)
    {
        for(int j=0; j<MAX_TERMINALS; j++)
        {
            Table[i][j] = -1; //default
        }
    }

    //populating the table now
    
    for(int i=0; i<<MAX_NON_TERMINALS; i++)       //for each production
    {
        //rule from NT n
        
        struct ntRules rule = grammar[i];
        
        char heads[rule.numRules][max(NTSIZE, TSIZE)];     //rule for each NT can have multiple heads too
        int tag[rule.numRules];                            //whether terminal or non terminal
        
        for(int k=0; k<rule.numRules; k++)                  //update the names of the heads and their tags 
        {
            strcpy(heads[k], rule.heads->tnt);
            tag[k] = rule.heads->tag; 
        }
        
        char nonT[NTSIZE];
        strcpy(nonT,rule.nt);

        //for every rule n -> alpha
        //head is alpha in every rule 
        for(int k=0; k<rule.numRules; k++)
        {
            int indexNT = getIndexOfNonTerminal(nonT);
            int headIndex;
            if(tag[k] == 0)              //non terminal
            {
                headIndex = getIndexOfNonTerminal(heads[k]);
                
                //for every terminal in first(alpha)
                for(int j=0; j<firstFollowSets[headIndex].numFirsts; j++)
                {
                    int indexT = getIndexofTerminal(firstFollowSets[headIndex].firsts[j]);
                    Table[indexNT][indexT] = i;
                }
            }
            else                          //terminal
            {
                if(strcmp(heads[k],"ε")==0) //if the terminal is ε
                {
                    //for each b (terminal) in follow(n)
                    for(int j=0; j<firstFollowSets[headIndex].numFollows; j++)
                    {
                        int indexT = getIndexOfTerminal(firstFollowSets[headIndex].follows[j]);
                        Table[indexNT][indexT] = i;
                    }


                }
                else
                {
                    headIndex = getIndexOfTerminal(heads[k]);
                    Table[indexNT][headIndex] = i;        //only one terminal in FIRST() because it itself is a terminal
                }     
            } 
            
        }
    }
 }