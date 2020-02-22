#define NTSIZE 	35
#define TSIZE	20
#define BUFF_SIZE 150
#define MAX_NON_TERMINALS 100
#define MAX_TERMINALS 100
#define MAX_FIRST 15
#define MAX_FOLLOW 15


char buffer[BUFF_SIZE];

char temp[10][TSIZE]; // Contains Firsts
int tempSize = 0;

char temp2[10][TSIZE];  // Contains Follows
int tempSize2 = 0;

char nonterminals[MAX_NON_TERMINALS][NTSIZE];
char terminals[MAX_TERMINALS][TSIZE];
int rhsNT[15];

int rhsNTSize = 0;
int numNT = 0;
int numT = 0;

int currentFirstFollowCompute = 0;

struct ruleToken* CurrentRuleTokenFF;

struct ruleToken{
    int tag; // Non-terminal(0) or terminal(1)
    char tnt[NTSIZE];
    struct ruleToken* next;
};

struct ntRules{
    char nt[NTSIZE];
    int numRules;
    struct ruleToken heads[10];
};

struct ntfirstFollow{
    char nt[NTSIZE];

    char firsts[MAX_FIRST][TSIZE]; // firsts consists only of terminals
    int numFirsts;

    char follows[MAX_FOLLOW][TSIZE]; // follows consists only of terminals
    int numFollows;
};

struct ntRules grammar[MAX_NON_TERMINALS];
int grammarLength = 0;

struct ntfirstFollow FirstFollowSets[MAX_NON_TERMINALS];


// Functions

void ComputeFirstAndFollow();

void computeRecursiveFirst(int index,int j);

void computeRecursiveFollow(int index,int j);

int getIndexOfNonTerminal(char* nt);

int getIndexOfTerminal(char *bufferToken);

void addFirsts(int i);

void addFollows(int i);

void addRuleToGrammer(char* rule);

void printRuleGrammarStruct(int i);

void printAllFirstSets();

void printAllTerminals();

void printAllNonTerminals();

void findRuleNumbersForRHS_NonTerminals(char* nt);

bool checkIfTokenAlreadyPresent(struct ruleToken* CurrentRuleToken);