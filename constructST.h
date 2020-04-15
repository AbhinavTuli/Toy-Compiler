typedef union
{
    int i;  // static
    char s[40];	// dynamic - id
} arr_index;

struct expNode{
    // Expression Node
    int tag;    //  0 for Op, 1 for NUM, 2 for RNUM, 3 for boolConstt, 4 for ID,5 for array
    char name[40];    // This will have Op or ID or arrayID

    bool isDynamic; // tag for union arr_index
    arr_index index;

    struct expNode* next;
}expNode;