#include "parser.h"
// Hash Function for strings
// unsigned long hash(unsigned char *str)
//     {
//         unsigned long hash = 5381;
//         int c;
//         while (c = *str++)
//             hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//             return hash;
//     }
int Table[MAX_NON_TERMINALS][TERMINALS]; //stores the rule number



void ComputeFirstAndFollow(grammar G,FirstandFollow F)
{
                            // if first is terminal, return first, else recursively go through non terminals

}

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
    
    int whichRule = 0;
    
    for(int i=0; i<MAX_NON_TERMINALS; i++)       //for each production
    {
        //rule from NT n
        
        
        struct ntRules rule = grammar[i];
        
        char heads[rule.numRules][NTSIZE];     //rule for each NT can have multiple heads too
        int tag[rule.numRules];                            //whether terminal or non terminal
        
        for(int k=0; k<rule.numRules; k++)                  //update the names of the heads and their tags 
        {
            strcpy(heads[k], rule.heads[k].tnt);
            tag[k] = rule.heads[k].tag; 
        }
        
        char nonT[NTSIZE];
        strcpy(nonT,rule.nt);

        //for every rule n -> alpha
        //head is alpha in every rule 
        for(int k=0; k<rule.numRules; k++)
        {
            int indexNT = getIndexOfNonTerminal(nonT);
            int headIndex;

            //DEBUG stmt printf("\n%d\n",whichRule+k);


            if(tag[k] == 0)              //non terminal
            {
                headIndex = getIndexOfNonTerminal(heads[k]);
                
                //for every terminal in first(alpha)
                for(int j=0; j<firstFollowSets[headIndex].numFirsts; j++)
                {
                    int indexT = getIndexOfTerminal(firstFollowSets[headIndex].firsts[j]);
                    Table[indexNT][indexT] = whichRule + k;

                    
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
                        Table[indexNT][indexT] = whichRule + k;
                    }


                }
                else
                {
                    headIndex = getIndexOfTerminal(heads[k]);
                    Table[indexNT][headIndex] = whichRule + k;        //only one terminal in FIRST() because it itself is a terminal
                }     
            } 
            
        }

        whichRule += rule.numRules;
    }
 }


struct treeNode




void parseInputSourceCode(char *testcaseFile, table T)
{

}

printParseTree(parseTree PT, char *outfile)
{

}