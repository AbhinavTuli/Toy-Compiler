#include "table_func.h"
#include <stdio.h>
#include <stdlib.h>
// #include "hash_function.h"

functionTable createFT(int size) {
	functionTable ft;
	ft.size=size;
	ft.total=0;
	ft.bins=(linkedListFunc*)malloc(sizeof(linkedListFunc)*size);
	for(int i=0; i<size; ++i) {
		ft.bins[i]=createListFunc();
	}
	return ft;
}

functionTable destroyFT(functionTable ft) {
	for(int i=0; i<ft.size; ++i) {
		ft.bins[i]=destroyListFunc(ft.bins[i]); }
	free(ft.bins);
	ft.bins=NULL;
	ft.size=0;
	ft.total=0;
	return ft;
}

functionTableEntry * searchInFT(functionTable ft, char *key) {
	int i = hash(key, ft.size);
	return searchInListFunc(ft.bins[i], key);
}

functionTable insertInFT(functionTable ft, functionTableEntry e) {
	if(searchInFT(ft, e.key)!= NULL) //do nothing if already present
		return ft;
	int bin= hash(e.key,ft.size);
	ft.total++;
	ft.bins[bin]=insertInListFunc(ft.bins[bin], e);
	return ft;
}

void printFT(functionTable ft) {
	fprintf(stderr, "Printing Hash Table\n");
	for(int i=0; i<ft.size; ++i) {
		if(ft.bins[i].size==0)
			continue;
		fprintf(stderr, "Bin %d\n",i);
		printListFunc(ft.bins[i]);
	}
}
