
#include<stdio.h>

const int hash = 499;                                                   // works for upto 499 distinct identifiers or function names

int hash_function(char *str) {
    
    unsigned long int h = 4201;
    int c;
	while ( c = *str++) 
    {
    	h = h + (h*c)^7 ;
	}
	h%=hash;
	if (h < 0) {
		h += hash;
	}
	return h;
}

int main()                                                              // testing hash
{
    printf("%d\n",hash_function("moghe"));\
    printf("%d\n",hash_function("bohra"));
    printf("%d\n",hash_function("tuli"));
    printf("%d\n",hash_function("amratanshu"));
    printf("%d\n",hash_function("raina"));
}