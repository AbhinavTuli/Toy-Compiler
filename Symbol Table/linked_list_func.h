//


typedef struct _nodeFunc {													
	functionTableEntry e;									// a single function, all its details, its key, arguments,etce
	struct _nodeFunc *next;									// next function in order of scope? or nested function ? 
} nodeFunc;

typedef struct _linkedListFunc {							  
	nodeFunc *head;											// contains pointer to a single nodefunc, which
	int size;												// size of given list
} linkedListFunc;

linkedListFunc createListFunc();                                                    //                                                                   

linkedListFunc destroyListFunc(linkedListFunc l);                                   //  delete all elements, free stuff

linkedListFunc insertInListFunc(linkedListFunc l, functionTableEntry e);            //  do malloc inside for node, makes usage clean
 
functionTableEntry * searchInListFunc(linkedListFunc l, char *value);
 
void printListFunc(linkedListFunc l);                                               //  for testing

#endif