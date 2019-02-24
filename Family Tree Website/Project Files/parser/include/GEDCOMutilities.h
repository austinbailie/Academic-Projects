/*
 Student Name: Austin Bailie
 Student Number: 0963370
 */

#ifndef GEDCOMutilities_h
#define GEDCOMutilities_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

typedef struct {
    
    char ID[10];
    Individual* indiPtr;
    
} Linker;

Header* createHeader(FILE* fp);

Submitter* createSubmitter(FILE* fp);

Individual* createIndividual(FILE* fp, char* indiRef, List* linkerList);

Family* createFamily(FILE* fp, char* famRef, List* linkerList);

void deleteLink(void* toBeDeleted);

void deleteDummy(void* toBeDeleted);

GEDCOMerror headerCheck(FILE* fp);

GEDCOMerror gedCheck(FILE* fp);

GEDCOMerror recordCheck(FILE* fp);

bool customSearch(const void* first,const void* second);


GEDCOMerror writeHeader(FILE* fp, const GEDCOMobject* obj);

GEDCOMerror writeSubmitter(FILE* fp, const GEDCOMobject* obj);

GEDCOMerror writeIndividual(FILE* fp, const GEDCOMobject* obj);

GEDCOMerror writeFamily(FILE* fp, const GEDCOMobject* obj);

int sortGen(const void* first, const void* second);

char* formatAddr(void* data);

char* createFileSum(char* filename);

char* populateGEDview(char* filename);

char* getJSONdesc(char* filename, char* indStr, int max);

char* getJSONancs(char* filename, char* indStr, int max);

#endif /* GEDCOMutilities_h */
