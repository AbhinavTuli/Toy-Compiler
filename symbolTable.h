#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "parserDef.h"
#include "lexerDef.h"
#include "symbolTableDef.h"

variableTable* initializeVarTable();

void printVarTable(variableTable* ptr);

int hash1(char * str);

void insertInVarTable(variableTable *ptr, char* var, bool isArray, int tag, int nestingLevel);

void updateArrayVarStatic(variableTable *ptr, char* var, int lB, int uB);

void updateArrayVarDynamic(variableTable *ptr, char* var, char* lB, char* uB);

bool searchInVarTable(variableTable *ptr, char* var);

variableTableEntry retrieveVarTable(variableTable *ptr, char* var);

void deleteVarTable(variableTable *ptr);

functionTable* initializeFunTable();

void printFunTable(functionTable* ptr);

void insertInFunTable(functionTable *ptr, char* func, parameter* inputL, parameter* outputL);

bool searchInFunTable(functionTable *ptr, char* func);

void deleteFromFuncTable(functionTable *ptr, char* func);

functionTableEntry retrieveFunTable(functionTable *ptr, char* func);

void deleteFunTable(functionTable *ptr);

#endif