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
	int allempty = 0;
	printf("index \tvarname \twidth \tis_array \tstatic/dynamic \trangevars \ttype \tnesting_level\tOffset\n\n");
	for(int i = 0; i<len;i++)
	{
		if(ptr->table[i].isEmpty)
			allempty++;
		else
		{
			if(ptr->table[i].isInput)
				printf("%d\t%s(input) \t%d\t",i,ptr->table[i].key,ptr->table[i].width);	
			else
				printf("%d\t%s       \t%d\t",i,ptr->table[i].key,ptr->table[i].width);
			if(ptr->table[i].isArray)
			{
				printf("YES\t\t");
				if(ptr->table[i].isArrayStatic)
					printf("STATIC    \t");
				else
					printf("DYNAMIC   \t");

				if(ptr->table[i].isArrayStatic)
					printf("[%d,%d]\t",ptr->table[i].lowerBound,ptr->table[i].upperBound);
				else
					printf("[%s,%s]\t",ptr->table[i].lowerBoundID,ptr->table[i].upperBoundID);

				if(ptr->table[i].tag==1)
					printf("\tInt\t");
				if(ptr->table[i].tag==2)
					printf("\tReal\t");
				if(ptr->table[i].tag==3)
					printf("\tBool\t");

				printf("\t%d\t",ptr->table[i].nestingLevel);
			}
			else
			{
				printf("NO\t\t----\t\t----\t");

				if(ptr->table[i].tag==1)
					printf("\tInt\t");
				if(ptr->table[i].tag==2)
					printf("\tReal\t");
				if(ptr->table[i].tag==3)
					printf("\tBool\t");

				printf("\t%d\t",ptr->table[i].nestingLevel);
			}
			printf("%d\n",ptr->table[i].offset);
		}
	}

	if(allempty == len)
		printf("THIS VAR TABLE IS EMPTY\n");
}

int hash1(char * str) {
    int hash = 401;
    while (*str != '\0') {
        hash = ((hash << 4) + (int)(*str)) % MAX_TABLE;
        str++;
    }

    return hash;
}

void insertInVarTable(variableTable *ptr, char* var, bool isArray, int tag, int nestingLevel)
{
	int tableSize = ptr->size;
	int index = hash1(var)%tableSize;
	//printf("%d\n",index);
	//printf("%d    %d\n",index,ptr->table[index].isEmpty);
	if(ptr->table[index].isEmpty)
	{
		ptr->table[index].isEmpty = false;
		strcpy(ptr->table[index].key,var);
		ptr->table[index].isArray = isArray;
		ptr->table[index].tag = tag;
		ptr->table[index].nestingLevel = nestingLevel;
		ptr->table[index].isInput = false;

		if(tag==1) // integer
			ptr->table[index].width = 2;
		else if(tag==2) // real
			ptr->table[index].width = 4;
		else if(tag==3) // boolean
			ptr->table[index].width = 1;				
	}
	else
	{
		index = (index+1)%tableSize;
		while(!ptr->table[index].isEmpty)
			index = (index+1)%tableSize;

		ptr->table[index].isEmpty = false;
		strcpy(ptr->table[index].key,var);
		ptr->table[index].isArray = isArray;
		ptr->table[index].tag = tag;
		ptr->table[index].nestingLevel = nestingLevel;
		ptr->table[index].isInput = false;

		if(tag==1)
			ptr->table[index].width = 2;
		else if(tag==2)
			ptr->table[index].width = 4;
		else if(tag==3)
			ptr->table[index].width = 1;
	}
}

void updateArrayVarStatic(variableTable *ptr, char* var, int lB, int uB)
{
	int len = ptr->size;
	int index = hash1(var)%len;

	while(strcmp(ptr->table[index].key,var) != 0)
		index = (index + 1)%len;

	ptr->table[index].isArrayStatic = true;
	//ptr->table[index].rangeVariables = rangeVariables;

		//printf("%d ---- %d\n",lB,uB);
		ptr->table[index].lowerBound = lB;
		ptr->table[index].upperBound = uB;

		ptr->table[index].width = ptr->table[index].width*(uB-lB+1) + 1;
}

void updateArrayVarDynamic(variableTable *ptr, char* var, char* lB, char* uB)
{
	int len = ptr->size;
	int index = hash1(var)%len;

	while(strcmp(ptr->table[index].key,var) != 0)
		index = (index + 1)%len;

	ptr->table[index].isArrayStatic = false;
	//ptr->table[index].rangeVariables = rangeVariables;

		//printf("%d ---- %d\n",lB,uB);
		strcpy(ptr->table[index].lowerBoundID,lB);
		strcpy(ptr->table[index].upperBoundID,uB);

		ptr->table[index].width = 1;
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

variableTableEntry* retrieveVarTable(variableTable *ptr, char* var)
{
	int tableSize = ptr->size;
	int index = hash1(var)%tableSize;

	int i = 0;
	while(i<tableSize)
	{
		if(strcmp(ptr->table[index].key,var) == 0)
			return &(ptr->table[index]);

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
	{
		t->table[i].isEmpty = true;
		//t->table[i].inputList = NULL;
		//t->table[i].outputList = NULL;
	}

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
	//printf("COUNT count%d \n",index);

	while(!ptr->table[index].isEmpty)
		index = (index+1)%len;
	//printf("COUNT count%d \n",index);
	ptr->table[index].isEmpty = false;
	strcpy(ptr->table[index].key,func);
	ptr->table[index].inputList = inputL;
	ptr->table[index].outputList = outputL;
	// Have to add the sizes of the input and output lists here
	ptr->table[index].localVarTable = initializeVarTable();
	strcpy((ptr->table[index].localVarTable)->funcName,func);
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

functionTableEntry* retrieveFunTable(functionTable *ptr, char* func)
{
	int len = ptr->size;
	int index = hash1(func)%len;

	int i = 0;
	while(i<len)
	{
		if(strcmp(ptr->table[index].key,func) == 0)
		{
			functionTableEntry* f = &(ptr->table[index]);
			return f;
		}

		index = (index+1)%len;
		i++;
	}
}

void deleteFromFuncTable(functionTable *ptr, char* func)
{
	int len = ptr->size;
	int index = hash1(func)%len;

	int i = 0;
	while(i<len)
	{
		if(strcmp(ptr->table[index].key,func) == 0)
		{
			ptr->table[index].isEmpty = true;
			return;
		}

		index = (index+1)%len;
		i++;
	}
}
void deleteFunTable(functionTable *ptr)
{
	free(ptr);
}

parameter* initializeParameter(char* varID, bool isArray, int tag)
{
	parameter* temp = (parameter *)malloc(sizeof(parameter));
	strcpy(temp->key, varID);
	temp->isArray = isArray;
	temp->tag = tag;
	temp->next = NULL;

	return temp;
}

void updateParameterArrayStatic(parameter* p, int low, int high)
{
	p->isArrayStatic = true;
	p->lowerBound = low;
	p->upperBound = high;
}

void updateParameterArrayDynamic(parameter* p, char* low, char* high)
{
	p->isArrayStatic = false;
	strcpy(p->lowerBoundID,low);
	strcpy(p->upperBoundID,high);
}

void addParametertoList(parameter* head, parameter* node)
{
	if(head==NULL)
	{
		head = (parameter *)malloc(sizeof(parameter));
		strcpy(head->key,node->key);
		head->isArray = node->isArray;
		head->tag = node->tag;
		printf("%s\n",head->key);	
		return;
	}

	parameter* traverse = head;
	while(traverse->next != NULL)
		traverse = traverse->next;

	traverse->next = node;
	return;
}

void printParameterList(parameter* head)
{
	if(head==NULL)
	{
		printf("LIST IS EMPTY\n");
		return;
	}

	int i = 1;
	while(head != NULL)
	{
		printf("Element %d, Var Name is %s, it is of type ",i,head->key);
		if(head->isArray)
		{
			printf("array[");
			
			//printf(" TEST %d TEST ",head->tag);
			if(head->tag == 1)
				printf("integer], ");
			if(head->tag == 2)
				printf("real], ");
			if(head->tag == 3)
				printf("boolean], ");

			if(head->isArrayStatic)
				printf("STATIC, range is [%d,%d]\n",head->lowerBound,head->upperBound);
			else
				printf("DYNAMIC, range is [%s,%s]\n",head->lowerBoundID,head->upperBoundID);
		}
		else
		{
			if(head->tag == 1)
				printf("integer\n");
			else if(head->tag == 2)
				printf("real\n");
			else if(head->tag == 3)
				printf("boolean\n");
		}

		i++;
		head = head->next;
	}
}

void printAllTables(functionTable *ptr, variableTable* driver)
{
	int len = ptr->size;

	for(int i = 0; i<len; i++)
	{
		if(!ptr->table[i].isEmpty)
		{
			printf("\nMODULE name is %s\n\n",ptr->table[i].key);
			printParameterList(ptr->table[i].inputList);
			printParameterList(ptr->table[i].outputList);

			variableTable* temp = ptr->table[i].localVarTable;
			while(temp!=NULL)
			{
				variableTable* traverse = temp;
				
				while(traverse!=NULL)
				{
					printf("LOCAL VAR TABLE of %s, start = %d, end = %d\n",temp->funcName,temp->scopeStart,temp->scopeEnd);
					printVarTable(traverse);
					traverse = traverse->next;
				}

				temp = temp->child;
			}

			parameter* listI = ptr->table[i].inputList;
			while(listI!=NULL)
			{
				if(listI->isRedifined)
				{
					printf(" %s (input) \n",listI->key);
				}

				listI = listI->next;
			}
		}
	}

	printf(" DRIVER MODULE \n");
	variableTable* temp = driver;
		
		while(temp!=NULL)
		{
			variableTable* traverse = temp;
				
			while(traverse!=NULL)
			{
				printf("LOCAL VAR TABLE of %s, start = %d, end = %d\n",temp->funcName,temp->scopeStart,temp->scopeEnd);
				printVarTable(traverse);
				traverse = traverse->next;
			}

			temp = temp->child;
		}
}

bool searchNested(variableTable* ptr, char* varname)
{
	while(ptr!=NULL)
	{
		if(searchInVarTable(ptr,varname))
			return true;

		ptr = ptr->parent;
	}

	return false;
}

variableTableEntry* searchNestedRetrieve(variableTable* ptr, char* varname)
{
	while(ptr!=NULL)
	{
		if(searchInVarTable(ptr,varname))
			return retrieveVarTable(ptr,varname);

		ptr = ptr->parent;
	}

	return NULL;
}
void updateDefineBool(functionTable* ptr, char* funcName, bool isDefined)
{
	functionTableEntry* ftemp = retrieveFunTable(ptr,funcName);
	ftemp->isDefined = isDefined;
}

void updateOffset(variableTable* ptr, char* varname, int offset)
{
	variableTableEntry* vtemp = retrieveVarTable(ptr,varname);
	vtemp->offset = offset;
}

void updateIsInput(variableTable* ptr, char* varname, bool isInput)
{
	variableTableEntry* vtemp = retrieveVarTable(ptr,varname);
	vtemp->isInput = isInput;

	if(vtemp->isArray)
		vtemp->width = 5;
}

int retrieveWidth(variableTable* ptr, char* varname)
{
	variableTableEntry* vtemp = retrieveVarTable(ptr,varname);
	return vtemp->width;
}

void printWidth(functionTable* ptr)
{
	int len = ptr->size;
	printf(" Function Name \t Activation Width \n");
	for(int i = 0; i<len; i++)
	{
		if(!ptr->table[i].isEmpty)
		{
			printf(" %s\t\t",ptr->table[i].key);
			int width = 0;

			variableTable* temp = ptr->table[i].localVarTable;
			//printVarTable(temp);
			while(temp!=NULL)
			{
				variableTable* traverse = temp;
				
				while(traverse!=NULL)
				{
					int len1 = traverse->size;
					for(int j = 0; j<len1; j++)
					{
						if(!traverse->table[j].isEmpty)
						{
							//printf("%d\n",traverse->table[j].width);
							width = width + traverse->table[j].width;
						}
					}
					traverse = traverse->next;
				}

				temp = temp->child;
			}

			parameter* ptemp = ptr->table[i].inputList;
			while(ptemp!=NULL)
			{
				if(ptemp->isRedifined)
				{
					//printf("  %s %d \n", ptr->table[i].key,width);
					width = width + ptemp->width;
				}

				ptemp = ptemp->next;
			}

			printf(" %d \n", width);
		}
	}
}
/*
void main()
{
	functionTable* t = initializeFunTable();
	insertInFunTable(t,"f1",NULL,NULL);
	insertInFunTable(t,"f2",NULL,NULL);
	insertInFunTable(t,"f3",NULL,NULL);
	insertInFunTable(t,"f4",NULL,NULL);
	printFunTable(t);

	//printParameterList(retrieveFunTable(t,"f1").outputList);

	parameter* n1 = initializeParameter("v1",false,1);
	//printf("%s , %d \n",n1->key,n1->tag);
	parameter* n2 = initializeParameter("v2",false,2);
	parameter* n3 = initializeParameter("v3",false,3);
	parameter* n4 = initializeParameter("v4",true,1);
	updateParameterArrayStatic(n4,5,10);
	parameter* n5 = initializeParameter("v5",true,2);
	updateParameterArrayStatic(n5,11,17);
	parameter* n6 = initializeParameter("v6",true,3);
	char a1[20] = "a";
	char a2[20] = "b";
	updateParameterArrayDynamic(n6,a1,a2);
	
	functionTableEntry* ftemp = retrieveFunTable(t,"f1");
	//printf("%s\n",ftemp->key);
	if(ftemp->outputList == NULL)
		ftemp->outputList = n1;
	addParametertoList(ftemp->outputList,n2);
	addParametertoList(ftemp->outputList,n3);
	addParametertoList(ftemp->outputList,n4);
	addParametertoList(ftemp->outputList,n5);
	addParametertoList(ftemp->outputList,n6);
	//addParametertoList(retrieveFunTable(t,"f1").outputList,n2);
	//addParametertoList(retrieveFunTable(t,"f1").outputList,n3);
	//addParametertoList(retrieveFunTable(t,"f1").outputList,n4);
	//addParametertoList(retrieveFunTable(t,"f1").outputList,n5);
	//addParametertoList(retrieveFunTable(t,"f1").outputList,n6);
	//printf("%s\n",ftemp->outputList->key);
	printParameterList(ftemp->outputList);
}
*/ 
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

/*
void main()
{
	
	variableTable* newTable = initializeVarTable();
	insertInVarTable(newTable,"var1int",false,1,0);
	insertInVarTable(newTable,"var2float",false,2,0);
	insertInVarTable(newTable,"var3bool",false,3,0);

	insertInVarTable(newTable,"arr1int",true,1,0);
	updateArrayVar(newTable,"arr1int",true,false,3,5);
	insertInVarTable(newTable,"arr2float",true,2,0);
	updateArrayVar(newTable,"arr2float",true,false,6,10);
	insertInVarTable(newTable,"arr3bool",true,3,0);
	updateArrayVar(newTable,"arr3bool",true,true,-1,-1);

	printVarTable(newTable);
} */