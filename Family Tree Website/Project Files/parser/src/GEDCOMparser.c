/*
 Student Name: Austin Bailie
 Student Number: 0963370
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"


GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj) {

    FILE* filePointer;
    
    GEDCOMerror fileError = {.type = INV_FILE, .line = -1};
    GEDCOMerror parseSuccess = {.type = OK, .line = -1};
    GEDCOMerror error;

    char* dot;
    
    if(fileName == NULL) {
        
        return fileError;
    }
    
    if((dot = strrchr(fileName, '.')) != NULL) {
        
        if(strcmp(dot, ".ged") != 0) {
            
            return fileError;
        }
    }
    
    filePointer = fopen(fileName, "r");
    
    if(filePointer == NULL) {
        
        return fileError;
    }
    
    error = recordCheck(filePointer);
    
    if(error.type != OK)
        return error;
   
    rewind(filePointer);
    error = gedCheck(filePointer);
    
    if(error.type != OK)
        return error;
    
    char level;
    char tag[20];
    char value[100];
    char line[260];
    
    rewind(filePointer);
   
    Header* objHeader = NULL;
    
    List indiList = initializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    List famList = initializeList(&printFamily, &deleteFamily, &compareFamilies); 
    
    List* linkerList = malloc(sizeof(List));
    linkerList->head = NULL;
    linkerList->tail = NULL; 
    *linkerList = initializeList(&printFamily, &deleteLink, &compareFamilies); 
    
    while(fgets(line, 250, filePointer) != NULL) {
        
        strcpy(value, "");
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        if(strcmp("HEAD", tag) == 0) {
            
            GEDCOMerror error = headerCheck(filePointer);
            if(error.type != OK) {
                
                return error;
            }
            
            objHeader = createHeader(filePointer);
            
        }else if(strncmp("INDI", value, strlen(value) - 1) == 0 && level == '0') {
            
            insertBack(&indiList, createIndividual(filePointer, tag, linkerList));
       
        }else if(strncmp("FAM", value, strlen(value) - 1) == 0 && level == '0') {
          
			insertBack(&famList, createFamily(filePointer, tag, linkerList));
        }
    }
    
    fclose(filePointer);
    
    *obj = malloc(sizeof(GEDCOMobject));
    
    (*obj)->header = objHeader;
    
    (*obj)->families = famList;
    
    (*obj)->individuals = indiList;
    
    (*obj)->submitter = objHeader->submitter;
    
	clearList(linkerList);
	free(linkerList);
	  
    return parseSuccess;
}


void deleteGEDCOM(GEDCOMobject* obj) {
    
    clearList(&obj->header->otherFields);
    
    free(obj->header->submitter);
    
    free(obj->header);
    
    
    ListIterator iterFam = createIterator(obj->families);
    
    Family* dataFam;
    
    while( (dataFam = (Family*)nextElement(&iterFam)) != NULL) {
 
        clearList(&dataFam->events);
 
        clearList(&dataFam->children);
  
        clearList(&dataFam->otherFields);
    }
    
    
    ListIterator iter = createIterator(obj->individuals);
    
    Individual* data;
    
    while( (data = (Individual*)nextElement(&iter)) != NULL) {
        
        clearList(&data->events);
  
        clearList(&data->families);
  
        clearList(&data->otherFields);
    }
    
    
    clearList(&obj->families);
    
    clearList(&obj->individuals);
    
    free(obj);
}

List getDescendants(const GEDCOMobject* familyRecord, const Individual* person) {
    
    List decList;
    decList = initializeList(&printIndividual, &deleteDummy, &compareIndividuals);
    bool valid = false;
    
    if(person == NULL)
        return decList;
    
    if(familyRecord == NULL)
        return decList;
    
    ListIterator iter;
    iter = createIterator(familyRecord->families);
    
    Family* famData;
    
    while( (famData = (Family*)nextElement(&iter)) != NULL) {
    
        if(famData->wife == person) {
            valid = true;
            break;
            
        }else if(famData->husband == person) {
            valid = true;
            break;
        }
    }
    
    if(valid != true)
        return decList;
    
    ListIterator iterIndi = createIterator(famData->children);
    
    Individual* indiData;
    
    while( (indiData = (Individual*)nextElement(&iterIndi)) != NULL) {
   
		insertBack(&decList, indiData);   
    }
    
    
    ListIterator iterOne = createIterator(decList);
    
    Individual* indivi;

    while( (indivi = (Individual*)nextElement(&iterOne)) != NULL) {

        if(indivi->families.head != NULL) {

            Family* family;

            ListIterator iterTwo = createIterator(indivi->families);

            while( (family = (Family*)nextElement(&iterTwo)) != NULL) {
 
                if(family->children.head != NULL) {
                
                    Individual* indiTwo;
                    
                    ListIterator iterThree = createIterator(family->children);
                    
                    while( (indiTwo = (Individual*)nextElement(&iterThree)) != NULL) {
                        
                        if(findElement(decList, customSearch, indiTwo) == NULL) {
                            
                            insertBack(&decList, indiTwo);
                        }
                    }
                }
            }
        }
    }
    
    List copiedList = initializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    Individual* toBeCopied;
                    
    ListIterator iterFour = createIterator(decList);
                    
    while( (toBeCopied = (Individual*)nextElement(&iterFour)) != NULL) {
		
		Individual* copy = malloc(sizeof(Individual));
		
		copy->givenName = malloc(sizeof(char) * strlen(toBeCopied->givenName) + 1);
		copy->surname = malloc(sizeof(char) * strlen(toBeCopied->surname) + 1);
		
		strcpy(copy->givenName, toBeCopied->givenName);
		strcpy(copy->surname, toBeCopied->surname);
		
		copy->otherFields = initializeList(&printField, &deleteField, &compareFields);
		copy->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
		copy->families = initializeList(&printFamily, &deleteDummy, &compareFamilies);
		
		insertBack(&copiedList, copy);
	}
    
    clearList(&decList);
  
    return copiedList;
}

Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person) {
    
    if(familyRecord == NULL)
        return NULL;
    
    if(person == NULL)
        return NULL;
    
    Individual* indiMatch = (Individual*)findElement(familyRecord->individuals, compare, person);
      
    return indiMatch;
}

char* printError(GEDCOMerror err) {
    
    char* errorStr = NULL;
    char buffer[50];
    
    switch (err.type) {
      
        case OK:
            
            errorStr = malloc(sizeof(char) * strlen("OK") + 1);
            strcpy(errorStr, "OK");
            
            break;
            
        case INV_FILE:
            
            errorStr = malloc(sizeof(char) * strlen("Invalid File"));
            strcpy(errorStr, "Invalid File");
            
            break;
            
        case INV_GEDCOM:
            
            errorStr = malloc(sizeof(char) * strlen("Invalid GEDCOM"));
            strcpy(errorStr, "Invalid GEDCOM");
            
            break;
            
        case INV_HEADER:
            
            sprintf(buffer, "Invalid Header (Line %d)", err.line);
            errorStr = malloc(sizeof(char) * strlen(buffer));
            strcpy(errorStr, buffer);
            
            break;
            
        case INV_RECORD:
            
            sprintf(buffer, "Invalid Record (Line %d)", err.line);
            errorStr = malloc(sizeof(char) * strlen(buffer));
            strcpy(errorStr, buffer);
            
            break;
            
        case OTHER_ERROR:
            
            sprintf(buffer, "Other Error (Line %d)", err.line);
            errorStr = malloc(sizeof(char) * strlen(buffer));
            strcpy(errorStr, buffer);
            
            break;
            
        default:
            break;
    }
    
    return errorStr;
}

char* printGEDCOM(const GEDCOMobject* obj) {

	if(obj == NULL)
		return NULL;
	
	char* string = malloc(sizeof(char) * (strlen(obj->header->source) + 4));
	char* tmpStr = malloc(sizeof(char) * 10);

	strcpy(string, obj->header->source);
	string = realloc(string, sizeof(char) * (strlen(string) + sizeof(tmpStr) + 1));
	strcat(string, "\n");
	sprintf(tmpStr, "%.2lf\n", obj->header->gedcVersion);
	strcat(string, tmpStr);
	free(tmpStr);
	
	string = realloc(string, sizeof(char) * (strlen(string) + 10));
	
	if(obj->header->encoding == ANSEL)
		strcat(string, "ANSEL\n");
		
	if(obj->header->encoding == UTF8)
		strcat(string, "UTF8\n");
	
	if(obj->header->encoding == UNICODE)
		strcat(string, "UNICODE\n");
		
	if(obj->header->encoding == ASCII)
		strcat(string, "ASCII\n");
		
	if(getLength(obj->header->otherFields) != 0) {
		
		char *fldStr = toString(obj->header->otherFields);
		string = realloc(string, sizeof(char) * (strlen(string) + strlen(fldStr) + 1));
		strcat(string, fldStr);
		free(fldStr);
	}
	
	string = realloc(string, sizeof(char) * (strlen(string) + strlen(obj->submitter->submitterName) + 2));
	strcat(string, obj->submitter->submitterName);
    strcat(string, "\n");
    
    if(strlen(obj->submitter->address) > 0) {

        string = realloc(string, sizeof(char) * (strlen(string) + strlen(obj->submitter->address) + 2));
        strcat(string, obj->submitter->address);
        strcat(string, "\n");
    }
	
    if(getLength(obj->header->submitter->otherFields) != 0) {

        char *fldStr = toString(obj->header->submitter->otherFields);
        string = realloc(string, sizeof(char) * (strlen(string) + strlen(fldStr) + 1));
        strcat(string, fldStr);
        free(fldStr);
    }

     if(getLength(obj->families) != 0) {

        char *famStr = toString(obj->families);
        string = realloc(string, sizeof(char) * (strlen(string) + strlen(famStr) + 1));
        strcat(string, famStr);
        free(famStr);
    }

    if(getLength(obj->individuals) != 0) {

        char *indStr = toString(obj->individuals);
        string = realloc(string, sizeof(char) * (strlen(string) + strlen(indStr) + 1));
        strcat(string, indStr);
        free(indStr);
    }
	
	return string;
}



//// HELPER /////////////////////////////////


void deleteIndividual(void* toBeDeleted) {
    
    Individual* indi = (Individual*)toBeDeleted;
    
    free(indi->givenName);
    free(indi->surname);
    
    free(toBeDeleted);
}

int compareIndividuals(const void* first,const void* second) {
    
    Individual* firstIndi = (Individual*)first;
    Individual* secondIndi = (Individual*)second;
    
    char* firstStr = malloc(sizeof(char) * (strlen(firstIndi->givenName) + strlen(firstIndi->surname) + 2));
    char* secondStr = malloc(sizeof(char) * (strlen(secondIndi->givenName) + strlen(secondIndi->surname) + 2));
    
    strcpy(firstStr, firstIndi->givenName);
    strcat(firstStr, ",");
    strcat(firstStr, firstIndi->surname);
    
    strcpy(secondStr, secondIndi->givenName);
    strcat(secondStr, ",");
    strcat(secondStr, secondIndi->surname);
    
    int value = strcmp(firstStr, secondStr);
    
    free(firstStr);
    free(secondStr);
    
    return value;
}

char* printIndividual(void* toBePrinted) { 
    
    Individual* indi = (Individual*)toBePrinted;
    char* string = malloc((sizeof(char) * (strlen(indi->givenName) + strlen(indi->surname)) + sizeof(", \n") + 2)); 
    
    strcpy(string, indi->givenName);
    strcat(string, "\n");
    strcat(string, indi->surname);
    strcat(string, "\n");
    
    ListIterator iter;

    if(indi->events.head != NULL) {

        iter = createIterator(indi->events);
        Event* event;
            
        while( (event = (Event*)nextElement(&iter)) != NULL) {

			char* eventStr;
            eventStr = printEvent(event);
            string = realloc(string, sizeof(char) * (strlen(string) + strlen(eventStr) + 1));
            strcat(string, eventStr);
            free(eventStr);
        }
    }
    
    if(indi->otherFields.head != NULL) {

        iter = createIterator(indi->otherFields);
        Field* field;

        while( (field = (Field*)nextElement(&iter)) != NULL) {

            char* fieldStr;
            fieldStr = printField(field);
            string = realloc(string, sizeof(char) * (strlen(string) + strlen(fieldStr) + 1));
            strcat(string, fieldStr);
            free(fieldStr);
        }
    }
     
    return string;
}

void deleteFamily(void* toBeDeleted) {
    
    free(toBeDeleted);
}

int compareFamilies(const void* first,const void* second) {
    
    Family* firstFam = (Family*)first;
    Family* secondFam = (Family*)second;
    
    int firstCount = 0;
    int secondCount = 0;
    
    if(firstFam->wife != NULL)
		++firstCount;
		
	if(firstFam->husband != NULL)
		++firstCount;
		
	if(secondFam->wife != NULL)
		++secondCount;
		
	if(secondFam->husband != NULL)
		++secondCount;
	
	firstCount += getLength(firstFam->children);
	
	secondCount += getLength(secondFam->children);
	
	if(firstCount < secondCount)
		return -1;
		
	if(firstCount > secondCount)
		return 1;
		
	if(firstCount == secondCount)
		return 0;
    
    return 0;
}

char* printFamily(void* toBePrinted) {
    
    Family* family = (Family*)toBePrinted;
    
    Individual* indi;
    
    char* string = NULL;
    
    int wifeLen = 0;
    int husbLen = 0;
    
    if(family->wife != NULL)
        wifeLen = strlen(family->wife->givenName) + strlen(family->wife->surname);
    
    if(family->husband != NULL)
        husbLen = strlen(family->husband->givenName) + strlen(family->husband->surname);
    
    string = malloc((sizeof(char) * (wifeLen + husbLen + 50)));
    
    if(family->wife != NULL && family->husband != NULL) {
        
        sprintf(string, "WIFE:\n %s\n %s\n\nHUSB:\n %s\n %s\n\nCHILDREN:\n", family->wife->givenName, family->wife->surname, family->husband->givenName, family->husband->surname);
    
    }else if(family->wife == NULL && family->husband != NULL) {
        
         sprintf(string, "HUSB:\n %s\n %s\n\nCHILDREN:\n", family->husband->givenName, family->husband->surname);
    
    }else if(family->wife != NULL && family->husband == NULL) {
        
        sprintf(string, "WIFE:\n %s\n %s\n\nCHILDREN:\n", family->wife->givenName, family->wife->surname);
    }
    
    
    ListIterator iter;

    iter = createIterator(family->children);
            
    while( (indi = (Individual*)nextElement(&iter)) != NULL) {

		string = realloc(string, (sizeof(char) * (strlen(string) + strlen(indi->givenName) + strlen(indi->surname) + 10)));
		strcat(string, " ");
        strcat(string, indi->givenName);
        strcat(string, "\n ");
        strcat(string, indi->surname);
        strcat(string, " \n\n");
    }
    
    if(family->events.head != NULL) {

        iter = createIterator(family->events);
        Event* event;
            
        while( (event = (Event*)nextElement(&iter)) != NULL) {

			char* eventStr;
            eventStr = printEvent(event);
            string = realloc(string, sizeof(char) * (strlen(string) + strlen(eventStr) + 1));
            strcat(string, eventStr);
            free(eventStr);
        }
    }
    
    if(family->otherFields.head != NULL) {

        iter = createIterator(family->otherFields);
        Field* field;

        while( (field = (Field*)nextElement(&iter)) != NULL) {

            char* fieldStr;
            fieldStr = printField(field);
            string = realloc(string, sizeof(char) * (strlen(string) + strlen(fieldStr) + 1));
            strcat(string, fieldStr);
            free(fieldStr);
        }
    }
    
    return string;
}

void deleteField(void* toBeDeleted) {
	
	Field* fld = (Field*)toBeDeleted;
	
	free(fld->tag);
	free(fld->value);
	
	free(toBeDeleted);
}	

int compareFields(const void* first,const void* second) {
	
	Field* firstFld = (Field*)first;
	Field* secondFld = (Field*)second;
	
	char* firstStr = malloc(sizeof(char) * (strlen(firstFld->tag) + strlen(firstFld->value) + 2));
    char* secondStr = malloc(sizeof(char) * (strlen(secondFld->tag) + strlen(secondFld->value) + 2));
    
    strcpy(firstStr, firstFld->tag);
    strcat(firstStr, " ");
    strcat(firstStr, firstFld->value);
    
    strcpy(secondStr, secondFld->tag);
    strcat(secondStr, " ");
    strcat(secondStr, secondFld->value);
    
    int value = strcmp(firstStr, secondStr);
    
    free(firstStr);
    free(secondStr);
    
    return value;
}

char* printField(void* toBePrinted) {
	
    Field* field = (Field*)toBePrinted;
    char* string = malloc(sizeof(char) * (strlen(field->tag) + strlen(field->value) + 10));
    
    strcpy(string, field->tag);
    strcat(string, ": ");
    strcat(string, field->value);
    strcat(string, "\n");
    
	return string;
}

void deleteEvent(void* toBeDeleted) {
	
	Event* evnt = (Event*)toBeDeleted;
	
	free(evnt->date);
	free(evnt->place);
	free(toBeDeleted);
}

int compareEvents(const void* first,const void* second) {
	
	Event* firstEvent = (Event*)first;
	Event* secondEvent = (Event*)second;
	
	return strcmp(firstEvent->type, secondEvent->type);
}

char* printEvent(void* toBePrinted) {
    
    Event* event = (Event*)toBePrinted;
    char* string = malloc(sizeof(char) * (strlen(event->type) + strlen(event->date) + strlen(event->place) + 10));
    
    strcpy(string, event->type);
    strcat(string, ": ");
    
    if(strlen(event->date) > 0)
         strncat(string, event->date, strlen(event->date)-1);
    
   
    if(strlen(event->place) > 0) {
        
        strcat(string, ", ");
        strcat(string, event->place);
    }
    
    strcat(string, "\n");
    
    ListIterator iter;

    if(event->otherFields.head != NULL) {

        iter = createIterator(event->otherFields);
        Field* field;

        while( (field = (Field*)nextElement(&iter)) != NULL) {

            char* fieldStr;
            fieldStr = printField(field);
            string = realloc(string, sizeof(char) * (strlen(string) + strlen(fieldStr) + 1));
            strcat(string, fieldStr);
            free(fieldStr);
        }
    }
	
	return string;
}

///// Assignment 2 ///////////////////

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj) {
    
    GEDCOMerror writeSuccess = {.type = OK, .line = -1};
    GEDCOMerror writeFail = {.type = WRITE_ERROR, .line = -1};
    GEDCOMerror error;
    
    if(obj == NULL)
        return writeFail;
    
    if(fileName == NULL)
        return writeFail;
    
    FILE* fp;
    
    fp = fopen(fileName, "w");
    
    if(fp == NULL)
        return writeFail;
    
    
    error = writeHeader(fp, obj);
    
    if(error.type != OK)
        return error;
    
    error = writeSubmitter(fp, obj);
    
    if(error.type != OK)
        return error;
    
    error = writeIndividual(fp, obj);
    
    if(error.type != OK)
        return error;
    
    error = writeFamily(fp, obj);
    
    if(error.type != OK)
        return error;
    
    fputs("0 TRLR\n", fp);
    
    fclose(fp);
    
    return writeSuccess;
}

ErrorCode validateGEDCOM(const GEDCOMobject* obj) {

    if(obj == NULL)
        return INV_GEDCOM;
    
    if(obj->header == NULL)
        return INV_GEDCOM;
    
    if(obj->submitter == NULL)
        return INV_GEDCOM;
    
    if(obj->header->submitter == NULL)
        return INV_HEADER;
    
    if(strlen(obj->header->source) == 0)
        return INV_HEADER;
    
    if(getLength(obj->header->otherFields) != 0) {
        
        Field* field;
        ListIterator iterFld = createIterator(obj->header->otherFields);
        
        while( (field = (Field*)nextElement(&iterFld)) != NULL) {
            
            if(strlen(field->value) > 200)
                return INV_RECORD;
            
            if(strlen(field->tag) > 200)
                return INV_RECORD;
            
        }
    }
    
    if(getLength(obj->submitter->otherFields) != 0) {
        
        Field* field;
        ListIterator iterFld = createIterator(obj->submitter->otherFields);
        
        while( (field = (Field*)nextElement(&iterFld)) != NULL) {
            
            if(strlen(field->value) > 200)
                return INV_RECORD;
            
            if(strlen(field->tag) > 200)
                return INV_RECORD;
            
        }
    }
    
    if(strlen(obj->submitter->submitterName) == 0)
        return INV_RECORD;
    
    if(strlen(obj->submitter->address) > 200)
        return INV_RECORD;
    
    
    if(getLength(obj->individuals) != 0) {
        
        Individual* indi;
        ListIterator iterIndi = createIterator(obj->individuals);
        
        while( (indi = (Individual*)nextElement(&iterIndi)) != NULL) {
            
            if(indi == NULL)
                return INV_RECORD;
            
            if(indi->givenName == NULL)
                return INV_RECORD;
            
            if(indi->surname == NULL)
                return INV_RECORD;
            
            if(strlen(indi->givenName) > 200)
                return INV_RECORD;
            
            if(strlen(indi->surname) > 200)
                return INV_RECORD;
            
            if(getLength(indi->otherFields) != 0) {
                
                Field* field;
                ListIterator iterFld = createIterator(indi->otherFields);
                
                while( (field = (Field*)nextElement(&iterFld)) != NULL) {
                    
                    if(strlen(field->value) > 200)
                        return INV_RECORD;
                    
                    if(strlen(field->tag) > 200)
                        return INV_RECORD;
                    
                }
            }
            
            if(getLength(indi->events) != 0) {
                
                Event* event;
                ListIterator iterEvt = createIterator(indi->events);
                
                while( (event = (Event*)nextElement(&iterEvt)) != NULL) {
                    
                    if(strlen(event->type) == 0)
                        return INV_RECORD;
                    
                    if(strlen(event->date) > 200)
                        return INV_RECORD;
                    
                    if(strlen(event->place) > 200)
                        return INV_RECORD;
                    
                    if(getLength(event->otherFields) != 0) {
                        
                        Field* field;
                        ListIterator iterFld = createIterator(event->otherFields);
                        
                        while( (field = (Field*)nextElement(&iterFld)) != NULL) {
                            
                            if(strlen(field->value) > 200)
                                return INV_RECORD;
                            
                            if(strlen(field->tag) > 200)
                                return INV_RECORD;
                            
                        }
                    }
                }
            }
        }
    }
    
    if(getLength(obj->families) != 0) {
        
        Family* family;
        ListIterator iterFam = createIterator(obj->families);
        
        while( (family = (Family*)nextElement(&iterFam)) != NULL) {
            
            if(family == NULL)
                return INV_RECORD;
            
            if(getLength(family->otherFields) != 0) {
                
                Field* field;
                ListIterator iterFld = createIterator(family->otherFields);
                
                while( (field = (Field*)nextElement(&iterFld)) != NULL) {
                    
                    if(strlen(field->value) > 200)
                        return INV_RECORD;
                    
                    if(strlen(field->tag) > 200)
                        return INV_RECORD;
        
                }
            }
            
            if(getLength(family->events) != 0) {
                
                Event* event;
                ListIterator iterEvt = createIterator(family->events);
                
                while( (event = (Event*)nextElement(&iterEvt)) != NULL) {
                    
                    if(strlen(event->type) == 0)
                        return INV_RECORD;
                    
                    if(strlen(event->date) > 200)
                        return INV_RECORD;
                    
                    if(strlen(event->place) > 200)
                        return INV_RECORD;
                    
                    if(getLength(event->otherFields) != 0) {
                        
                        Field* field;
                        ListIterator iterFld = createIterator(event->otherFields);
                        
                        while( (field = (Field*)nextElement(&iterFld)) != NULL) {
                            
                            if(strlen(field->value) > 200)
                                return INV_RECORD;
                            
                            if(strlen(field->tag) > 200)
                                return INV_RECORD;
                            
                        }
                    }
                }
            }
        }
    }
    
    
    return OK;
}

List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen) {
    
    List decList;
    decList = initializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    bool valid = false;
    int genCount = 0;
    
   
    if(person == NULL)
        return decList;
    
    if(familyRecord == NULL)
        return decList;
    
    ListIterator iterFam;
    iterFam = createIterator(familyRecord->families);
    
    Family* famData;
    
    while( (famData = (Family*)nextElement(&iterFam)) != NULL) {
        
        if(famData->wife == person) {
            valid = true;
            break;
            
        }else if(famData->husband == person) {
            valid = true;
            break;
        }
    }
    
    if(valid != true)
        return decList;
    
    if(getLength(famData->children) == 0)
        return decList;
    
    
    ListIterator iterIndi = createIterator(famData->children);
    Individual* indiData;
    
    List* tempList = malloc(sizeof(List));
    *tempList = initializeList(&printIndividual, &deleteDummy, &sortGen);
    
    while( (indiData = (Individual*)nextElement(&iterIndi)) != NULL) {
        
        Individual* indiCopy = indiData;
        
        insertSorted(tempList, indiCopy);
    }
    insertBack(&decList, tempList);
    
    ++genCount;
    
    if(maxGen == genCount)
        return decList;
    
    
    ListIterator iter = createIterator(decList);
                                          
    List* tempGen;
                                          
    while( (tempGen = (List*)nextElement(&iter)) != NULL) {
        
        ++genCount;
        List* genList = malloc(sizeof(List));
        *genList = initializeList(&printIndividual, &deleteDummy, &sortGen);
        
        ListIterator iterOne = createIterator(*tempGen);
        Individual* indivi;
    
        while( (indivi = (Individual*)nextElement(&iterOne)) != NULL) {
            
            if(getLength(indivi->families) > 0) {
                
                Family* family;
                ListIterator iterTwo = createIterator(indivi->families);
                
                while( (family = (Family*)nextElement(&iterTwo)) != NULL) {
                    
                    if(getLength(family->children) > 0) {
                        
                        Individual* indiTwo;
                        ListIterator iterThree = createIterator(family->children);
                        
                        while( (indiTwo = (Individual*)nextElement(&iterThree)) != NULL) {
                            
                            if(findElement(*tempGen, customSearch, indiTwo) == NULL) {
                                
                                Individual* indiCopy = indiTwo;
                                
                                insertSorted(genList, indiCopy);
                            }
                        }
                    }
                }
            }
        }
        
        if(getLength(*genList) > 0) {
            
            insertBack(&decList, genList);
            
        }else {
            
            free(genList);
        }
        
        if(maxGen == genCount)
            break;
        
    }
    
    return decList;
}

List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen) {
    
    List ancList;
    ancList = initializeList(&printGeneration, &deleteGeneration, &compareGenerations);
    bool valid = false;
    int genCount = 0;
    
    if(maxGen < 0)
        return ancList;
    
    if(person == NULL)
        return ancList;
    
    if(familyRecord == NULL)
        return ancList;
    
    if(getLength(familyRecord->families) == 0)
        return ancList;
    
    if(getLength(familyRecord->individuals) == 0)
        return ancList;
    
    
    ListIterator iterFam = createIterator(familyRecord->families);
    Family* famData;
    
    List* tempList = malloc(sizeof(List));
    *tempList = initializeList(&printIndividual, &deleteDummy, &sortGen);
    
    while( (famData = (Family*)nextElement(&iterFam)) != NULL) {
        
        if(findElement(famData->children, customSearch, person) != NULL) {
            
            if(famData->wife != NULL) {
                
                Individual* indiCopy;
                indiCopy = famData->wife;
                
                if(findElement(*tempList, customSearch, indiCopy) == NULL)
                    insertSorted(tempList, indiCopy);
                
                valid = true;
            }
            
            if(famData->husband != NULL) {
                
                Individual* indiCopy;
                indiCopy = famData->husband;
                
                if(findElement(*tempList, customSearch, indiCopy) == NULL)
                    insertSorted(tempList, indiCopy);
                
                valid = true;
            }
            
            ++genCount;
        }
    }
    
    if(getLength(*tempList) > 0) {
        
        insertBack(&ancList, tempList);
    
    }else {
        
        free(tempList);
    }
    
    if(valid != true)
        return ancList;
    
    if(maxGen == genCount)
        return ancList;
    
    
    ListIterator iter = createIterator(ancList);
    
    List* tempGen;
    
    while( (tempGen = (List*)nextElement(&iter)) != NULL) {
        
        ++genCount;
        List* genList = malloc(sizeof(List));
        *genList = initializeList(&printIndividual, &deleteDummy, &sortGen);
        
        ListIterator iterOne = createIterator(*tempGen);
        Individual* indivi;
        
        while( (indivi = (Individual*)nextElement(&iterOne)) != NULL) {
            
            ListIterator iterTwo = createIterator(familyRecord->families);
            Family* family;
            
            while( (family = (Family*)nextElement(&iterTwo)) != NULL) {
                
                if(findElement(family->children, customSearch, indivi) != NULL) {
                    
                    if(family->wife != NULL) {
                        
                        Individual* indiCopy;
                        indiCopy = family->wife;
                        if(findElement(*genList, customSearch, indiCopy) == NULL)
                            insertSorted(genList, indiCopy);
                    }
                    
                    if(family->husband != NULL) {
                        
                        Individual* indiCopy;
                        indiCopy = family->husband;
                        if(findElement(*genList, customSearch, indiCopy) == NULL)
                            insertSorted(genList, indiCopy);
                    }
                    
                }
            }
        }
        
        if(getLength(*genList) > 0) {
            
            insertBack(&ancList, genList);
            
        }else {
			
			free(genList);
		}
        
        if(maxGen == genCount)
            break;
     
        
    }
    
    return ancList;
}


void deleteGeneration(void* toBeDeleted) {
    
    List* genList = (List*)toBeDeleted;
    
    clearList(genList);
    
	free(genList);
}


int compareGenerations(const void* first,const void* second) {

    
    
    return 0;
}


char* printGeneration(void* toBePrinted) {
    
    List* genList = (List*)toBePrinted;
    
    char* string = toString(*genList);
    
    return string;
}

char* indToJSON(const Individual* ind) {
    
    if(ind == NULL)
        return calloc(1, sizeof(char));
    
    char* string = malloc(sizeof(char) * (strlen(ind->givenName) + strlen(ind->surname) + 50));
    
    if(ind->givenName == NULL || strncmp(ind->givenName, " ", strlen(ind->givenName) - 1) == 0) {
        
        sprintf(string, "{\"givenName\":\"\",\"surname\":\"%s\"}", ind->surname);
        
    }else if(ind->surname == NULL || strncmp(ind->surname, " ", strlen(ind->surname) - 1) == 0) {
        
        sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"\"}", ind->givenName);
        
    }else {
        
        sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"%s\"}", ind->givenName, ind->surname);
    }
    
    return string;
}

Individual* JSONtoInd(const char* str) {
    
    if(str == NULL)
        return NULL;
    
    if(strlen(str) == 0)
        return NULL;
    
    Individual* indi = malloc(sizeof(Individual));
    indi->givenName = NULL;
    indi->surname = NULL;
    
    indi->otherFields = initializeList(&printField, &deleteField, &compareFields);
    indi->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
    indi->families = initializeList(&printFamily, &deleteDummy, &compareFamilies);
    
    char* string = malloc(sizeof(char) * strlen(str)+1);
    strcpy(string, str);
    char* pch;

    pch = strtok(string, "\"{,:]}");
    
    if(strcmp("givenName", pch) != 0) {
        
        free(indi);
        return NULL;
    }

    pch = strtok(NULL, "\"{,:]}");

    if(strcmp("surname", pch) == 0) {
    
        indi->givenName = calloc(1, sizeof(char));
        
        pch = strtok(NULL, "\"{,:]}");
        
        if(pch != NULL) {
            
            indi->surname = malloc(sizeof(char) * (strlen(pch) + 1));
            strcpy(indi->surname, pch);
        }
            
    }else {
        
        indi->givenName = malloc(sizeof(char) * (strlen(pch) + 1));
        strcpy(indi->givenName, pch);
        
        pch = strtok(NULL, "\"{,:]}");
        
        if(strcmp("surname", pch) != 0) {
            
            free(indi->givenName);
            free(indi);
            return NULL;
        }
        
        pch = strtok(NULL, "\"{,:]}");
        
        if(pch != NULL) {
            
            indi->surname = malloc(sizeof(char) * (strlen(pch) + 1));
            strcpy(indi->surname, pch);
            
        }else {
            
            indi->surname = calloc(1, sizeof(char));
        }
    }

    free(string);
    
    return indi;
}

GEDCOMobject* JSONtoGEDCOM(const char* str) {
    
    if(str == NULL)
        return NULL;
    
    if(strlen(str) == 0)
        return NULL;
    
    if(str[0] != '{')
        return NULL;
    
    Header* head = malloc(sizeof(Header));
    head->otherFields = initializeList(&printField, &deleteField, &compareFields);
    
    Submitter* subm = malloc(sizeof(Submitter) + (sizeof(char) * 50));
    subm->otherFields = initializeList(&printField, &deleteField, &compareFields);
    
    char* string = malloc(sizeof(char) * strlen(str)+1);
    strcpy(string, str);
    char* pch;
    char array[10][50] = {"", "", "", "", "", "", "", "", "", ""};
    int count = 0;
    
    pch = strtok(string, "\"{,:]}");
    
    while(pch != NULL) {
        
        if(strcmp("gedcVersion", pch) == 0 && strcmp("source", array[count-1]) == 0) {
            
            strcpy(array[count], "");
            ++count;
            
        }else if(strcmp("encoding", pch) == 0 && strcmp("gedcVersion", array[count-1]) == 0) {
            
            strcpy(array[count], "");
            ++count;
            
        }else if(strcmp("subName", pch) == 0 && strcmp("encoding", array[count-1]) == 0) {
            
            strcpy(array[count], "");
            ++count;
            
        }else if(strcmp("subAddress", pch) == 0 && strcmp("subName", array[count-1]) == 0) {
            
            strcpy(array[count], "");
            ++count;
        }
        
        strcpy(array[count], pch);
        ++count;
        pch = strtok(NULL, "\"{,:]}");
    }
    
    for(int i = 0; i < 10; i++) {
        
        if(strcmp("source", array[i]) == 0) {
            
            strcpy(head->source, array[i+1]);

        }else if(strcmp("gedcVersion", array[i]) == 0) {
            
            float version = atof(array[i+1]);
            head->gedcVersion = version;
            
        }else if(strcmp("encoding", array[i]) == 0) {
            
            if(strcmp("ANSEL", array[i+1]) == 0) {
                
                head->encoding = ANSEL;
                
            }else if(strcmp("UTF-8", array[i+1]) == 0) {
                
                head->encoding = UTF8;
                
            }else if(strcmp("UNICODE", array[i+1]) == 0) {
                
                head->encoding = UNICODE;
                
            }else if(strcmp("ASCII", array[i+1]) == 0) {
                
                head->encoding = ASCII;
            }
            
        }else if(strcmp("subName", array[i]) == 0) {
            
            strcpy(subm->submitterName, array[i+1]);
            
        }else if(strcmp("subAddress", array[i]) == 0) {
            
            strcpy(subm->address, array[i+1]);
        }
    }
    
    head->submitter = subm;
    
    GEDCOMobject* obj = malloc(sizeof(GEDCOMobject));
    
    obj->header = head;
    obj->submitter = subm;
    
    obj->individuals = initializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
    
    obj->families = initializeList(&printFamily, &deleteFamily, &compareFamilies);
    
    free(string);
    
    return obj;
}

void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded) {
    
    if(obj == NULL)
        return;
    
    if(toBeAdded == NULL)
        return;
    
    insertBack(&obj->individuals, (void*)toBeAdded);
    
}

char* iListToJSON(List iList) {
    
    if(getLength(iList) == 0) {
        
        char* listStr = malloc(sizeof(char) * 4);
        strcpy(listStr, "[]");
        
        return listStr;
    }
    
    char* listStr = malloc(sizeof(char) * 10);
    char* string = NULL;
    
    strcpy(listStr, "[");
    
    ListIterator iter = createIterator(iList);
    Individual* indi;
    
    while( (indi = (Individual*)nextElement(&iter)) != NULL) {
        
        string = indToJSON(indi);
        listStr = realloc(listStr, sizeof(char) * (strlen(listStr) + strlen(string) + 5));
        strcat(listStr, string);
        
        if(iter.current != NULL)
            strcat(listStr, ",");
            
            
        free(string);
    }
    
    strcat(listStr, "]");
    
    
    return listStr;
}


char* gListToJSON(List gList) {
    
    if(getLength(gList) == 0) {
        
        char* listStr = malloc(sizeof(char) * 4);
        strcpy(listStr, "[]");
        
        return listStr;
    }
    
    char* listStr = malloc(sizeof(char) * 10);
    char* string = NULL;
    
    strcpy(listStr, "[");
    
    ListIterator iter = createIterator(gList);
    List* genList;
    
    while( (genList = (List*)nextElement(&iter)) != NULL) {
        
        string = iListToJSON(*genList);
        listStr = realloc(listStr, sizeof(char) * (strlen(listStr) + strlen(string) + 5));
        strcat(listStr, string);
        
        if(iter.current != NULL)
            strcat(listStr, ",");
            
        free(string);
        
    }
    
    strcat(listStr, "]");
    
    return listStr;
}





