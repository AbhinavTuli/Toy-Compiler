/*
    Group Number            -        26
    Abhinav Tuli            -   2017A7PS0048P
    Kushagra Raina          -   2017A7PS0161P
    Tanmay Moghe            -   2017A7PS0184P
    Amratanshu Shrivastava  -   2017A7PS0224P
    Rohit Bohra             -   2017A7PS0225P
*/
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