#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"symbolTable.h"

variableTable* initializeVarTable()
{
	variableTable* t;
	t = (variableTable *)malloc(sizeof(variableTable));
	t->table = (variableTableEntry *)malloc(sizeof(variableTableEntry)*VARTABLESIZE);
	t->size = VARTABLESIZE;

	t->count = 1;
	t->parent = NULL;
	t->child = NULL;
	t->next = NULL;

	for(int i = 0; i<t->size;i++)
		t->table[i].isEmpty = true;

	return t;
}

void printVarTable(variableTable* ptr)
{
	int len = ptr->size;
	for(int i = 0; i<len;i++)
	{
		if(ptr->table[i].isEmpty)
			printf("index=%d, EMPTY\n",i);
		else
		{
			printf("index=%d, var name is %s\n",i,ptr->table[i].key);
		}
	}
}

int hash1(char * str) {
    int hash = 401;
    while (*str != '\0') {
        hash = ((hash << 4) + (int)(*str)) % MAX_TABLE;
        str++;
    }

    return hash;
}

void insertInVarTable(variableTable *ptr, char* var, bool isArray, int tag)
{
	int tableSize = ptr->size;
	int index = hash1(var)%tableSize;
	//printf("%d\n",index);
	//printf("%d    %d\n",index,ptr->table[index].isEmpty);
	if(ptr->table[index].isEmpty)
	{
		ptr->table[index].isEmpty = false;
		ptr->table[index].key = var;
		ptr->table[index].isArray = isArray;
		ptr->table[index].tag = tag;
	}
	else
	{
		index = (index+1)%tableSize;
		while(!ptr->table[index].isEmpty)
			index = (index+1)%tableSize;

		ptr->table[index].isEmpty = false;
		ptr->table[index].key = var;
		ptr->table[index].isArray = isArray;
		ptr->table[index].tag = tag;
	}
}

bool searchInVarTable(variableTable *ptr, char* var)
{
	int tableSize = ptr->size;
	int index = hash1(var)%tableSize;

	if(ptr->table[index].isEmpty)
		return false;

	int i = 0;
	while(i<tableSize)
	{
		if(strcmp(ptr->table[index].key,var) == 0)
			return true;

		index = (index+1)%tableSize;
		i++;

		if(ptr->table[index].isEmpty)
			return false;
	}

	return false;
}

variableTableEntry retrieveVarTable(variableTable *ptr, char* var)
{
	int tableSize = ptr->size;
	int index = hash1(var)%tableSize;

	int i = 0;
	while(i<tableSize)
	{
		if(strcmp(ptr->table[index].key,var) == 0)
			return ptr->table[index];

		index = (index+1)%tableSize;
		i++;
	}
}

void deleteVarTable(variableTable *ptr)
{
	free(ptr);
}

functionTable* initializeFunTable()
{
	functionTable* t;
	t = (functionTable *)malloc(sizeof(functionTable));

	t->table = (functionTableEntry *)malloc(sizeof(functionTableEntry)*FUNCTABLESIZE);
	t->size = FUNCTABLESIZE;

	for(int i = 0; i<FUNCTABLESIZE;i++)
		t->table[i].isEmpty = true;

	return t;
}

void printFunTable(functionTable* ptr)
{
	int len = ptr->size;

	for(int i = 0; i<len; i++)
	{
		if(ptr->table[i].isEmpty)
			printf("Index %d - EMPTY\n",i);
		else
			printf("Index %d - name = %s\n",i,ptr->table[i].key);
	}
}

void insertInFunTable(functionTable *ptr, char* func, parameter* inputL, parameter* outputL)
{
	int len = ptr->size;
	int index = hash1(func)%len;
	//printf("%d \n",index);

	while(!ptr->table[index].isEmpty)
		index = (index+1)%len;

	ptr->table[index].isEmpty = false;
	ptr->table[index].key = func;
	ptr->table[index].inputList = inputL;
	ptr->table[index].outputList = outputL;
	// Have to add the sizes of the input and output lists here
	ptr->table[index].localVarTable = initializeVarTable();
}

bool searchInFunTable(functionTable *ptr, char* func)
{
	int len = ptr->size;
	int index = hash1(func)%len;
	//printf("%d \n",index);
	int i = 0;
	while(i<len && !ptr->table[index].isEmpty)
	{
		if(strcmp(func,ptr->table[index].key)==0)
			return true;

		i++;
		index = (index+1)%len;
	}

	return false;
}

functionTableEntry retrieveFunTable(functionTable *ptr, char* func)
{
	int len = ptr->size;
	int index = hash1(func)%len;

	int i = 0;
	while(i<len)
	{
		if(strcmp(ptr->table[index].key,func) == 0)
			return ptr->table[index];

		index = (index+1)%len;
		i++;
	}
}

void deleteFunTable(functionTable *ptr)
{
	free(ptr);
}

/*
void main()
{
	
	variableTable* newTable = initializeVarTable();
	char* temp = "TANMAY";
	insertInVarTable(newTable,"TANMAY",false,1);
	insertInVarTable(newTable,"TaANMAY",false,1);
	insertInVarTable(newTable,"TdNMAY",false,1);
	insertInVarTable(newTable,"TbNMAY",false,1);
	insertInVarTable(newTable,"TxNfssMAY",false,1);
	insertInVarTable(newTable,"TANMsdagfaAY",false,1);
	insertInVarTable(newTable,"TANMAdsafdsafY",false,1);
	insertInVarTable(newTable,"TAdsfadsNMAY",false,1);
	printVarTable(newTable);

	printf("%d %d %d %d\n",searchInVarTable(newTable,"TANMAY"),
	searchInVarTable(newTable,"TxNfssMAY"),
	searchInVarTable(newTable,"fds"),
	searchInVarTable(newTable,"TAdsfadsNMAY"));

	printf("TRIAL %d \n",searchInVarTable(newTable,"TANMAdsafdsafY"));

	variableTableEntry v1 = retrieveVarTable(newTable,"TANMAY");
	printf("%d %d \n",v1.isArray,v1.tag);

	deleteVarTable(newTable);

	functionTable* nT = initializeFunTable();
	insertInFunTable(nT,"f1",NULL,NULL);
	insertInFunTable(nT,"f2",NULL,NULL);
	insertInFunTable(nT,"f3",NULL,NULL);
	insertInFunTable(nT,"f4",NULL,NULL);

	printf(" %d  %d  %d\n",searchInFunTable(nT,"f2"),searchInFunTable(nT,"sfa"),searchInFunTable(nT,"f9"));


	printFunTable(nT);
} 

*/ 