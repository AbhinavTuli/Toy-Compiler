

 //functions

int hash(char * str);
void insertEntry(char* c,term te);
int findEntry(char* c);
struct node* retNode(char *c);
int printHashtable();
void initializeHashtable();
const char* getLexeme(term t);
void populateKeywordTable();
FILE *getStream(FILE *fp);
token* retTokenINT(int value,term t);
token* retTokenREAL(float value,term t);
token* retTokenBOOL(bool value,term t);
token* retTokenSTR(char value[],term t);
token* checkASSIGNOP();
token* checkEQ();
token* checkGT();
token* checkLT();
token* checkRANGEOP();
int isAlpha(char c);
int isDigit(char c);
void removeComments(char *testcaseFile, char *cleanFile);
void getNextToken();
