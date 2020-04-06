struct ASTNode{
    char type[50];
    char value[50];
    struct ASTNode* child;
    struct ASTNode* next;
} astNode;