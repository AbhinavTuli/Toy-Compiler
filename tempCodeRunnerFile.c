int main(){

    FILE* fp = fopen("grammar.txt","r");

    readGrammerTextFile(fp);

    // printf("Total Non-Terminals : %d\n",numNT);
    // printf("Total Terminals : %d\n\n",numT);

    // printAllGrammar();
    // printAllTerminals();
    // printAllNonTerminals();

    printf("\nComputing Firsts And Follows : \n\n");
    computeFirstAndFollow();


    printf("\nFirst Sets : \n\n");
    printAllFirstSets();

    printf("\n\nFollow Sets : \n\n");
    printAllFollowSets();
}
