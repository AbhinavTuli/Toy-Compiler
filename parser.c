
// Hash Function for strings
// unsigned long hash(unsigned char *str)
//     {
//         unsigned long hash = 5381;
//         int c;
//         while (c = *str++)
//             hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//             return hash;
//     }
int Table[NON_TERMINALS][TERMINALS]; //stores the rule number



void ComputeFirstAndFollow(grammar G,FirstandFollow F)
{
                            // if first is terminal, return first, else recursively go through non terminals

}

void createParseTable(struct ntfirstFollow firstFollowSets[NON_TERMINALS], struct ntRules grammar[NON_TERMINALS]) //, int[][] Table)
{
    for(int i=0; i<NON_TERMINALS; i++)
    {
        for(int j=0; j<TERMINALS; j++)
        {
            Table[i][j] = -1; //default
        }
    }

    //populating the table now
    
    for(int i=0; i<NON_TERMINAL; i++)       //for each production
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
            int indexNT = getIndexNT(nonT);
            int headIndex;
            if(tag[k] == 0)              //non terminal
            {
                headIndex = getIndexNT(heads[k]);
                
                //for every terminal in first(alpha)
                for(int j=0; j<firstFollowSets[headIndex].numFirsts; j++)
                {
                    int indexT = getIndexT(firstFollowSets[headIndex].firsts[j]);
                    Table[indexNT][indexT] = i;
                }
            }
            else                          //terminal
            {
                headIndex = getIndexT(heads[k]);
                Table[indexNT][headIndex] = i;        //only one terminal in FIRST() because it itself is a terminal
                
            }
            
            
            
            

        }
        
        

        





        // char firstset[TERMINALS][TSIZE];
        // for(int p=0; p<firstFollowSets[i].numFirsts ;p++)
        // {
        //     strcpy(firstset[p] , firstFollowSets[i].firsts[p]);
        // }

        // char followset[TERMINALS][TSIZE];
        // for(int p=0; p<firstFollowSets[i].numFollows ;p++)
        // {
        //     strcpy(firstset[p] , firstFollowSets[i].follows[p]);
        // }
    }
    



}

parseInputSourceCode(char *testcaseFile, table T)
{

}

printParseTree(parseTree PT, char *outfile)
{

}