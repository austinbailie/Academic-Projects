/*
 Student Name: Austin Bailie
 Student Number: 0963370
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"


bool customSearch(const void* first,const void* second) {
    
    Individual* firstIndi = (Individual*)first;
    Individual* secondIndi = (Individual*)second;
    
    char* firstStr = malloc(sizeof(char) * (strlen(firstIndi->givenName) + strlen(firstIndi->surname) + 2));
    char* secondStr = malloc(sizeof(char) * (strlen(secondIndi->givenName) + strlen(secondIndi->surname) + 2));
    
    
    strcpy(firstStr, firstIndi->surname);
    strcat(firstStr, ",");
    strcat(firstStr, firstIndi->givenName);
 
    strcpy(secondStr, secondIndi->surname);
    strcat(secondStr, ",");
    strcat(secondStr, secondIndi->givenName);
    
    for(int i = 0; i < strlen(firstStr); i++) {
        
        firstStr[i] = tolower(firstStr[i]);
    }
    
    for(int j = 0; j < strlen(secondStr); j++) {
        
        secondStr[j] = tolower(secondStr[j]);
    }
   
    if(strcmp(firstStr, secondStr) == 0) {
		
		free(firstStr);
		free(secondStr);
        return true;
    }
    
    free(firstStr);
	free(secondStr);
    
    return false;
}


void deleteDummy(void* toBeDeleted) {
    
}


void deleteLink(void* toBeDeleted) {
    
    free(toBeDeleted);
}

Family* createFamily(FILE* fp, char* famRef, List* linkerList) {
    
    Family* tempFam = malloc(sizeof(Family));
    tempFam->wife = NULL;
    tempFam->husband = NULL;
    
    tempFam->otherFields = initializeList(&printField, &deleteField, &compareFields);
    tempFam->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
    tempFam->children = initializeList(&printIndividual, &deleteDummy, &compareIndividuals);
    
    char level;
    char tag[20];
    char value[100];
    char line[260];
    
    char eventArray[12][4] = {"ANUL", "CENS", "DIV", "DIVF", "ENGA", "MARR", "MARB",
        "MARC", "MARL", "MARS", "EVEN"};
    
    int otherFlag = 0;

    while(fgets(line, 250, fp) != NULL) {
        
        strcpy(value, " ");
        
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
       
        for(int i = 0; i < 12; i++) {
            
            char levelTwo;
            char tagTwo[20];
            char valueTwo[100];
            
            if(strncmp(eventArray[i], tag, strlen(tag) - 1) == 0) {
                
                otherFlag = 1;
                
                Event* tempEvent = malloc(sizeof(Event));
                tempEvent->date = NULL;
                tempEvent->place = NULL;
                
                tempEvent->otherFields = initializeList(&printField, &deleteField, &compareFields);
                strcpy(tempEvent->type, tag);
                
                while(fgets(line, 250, fp) != NULL) {
                    
                    strcpy(valueTwo, " ");
                    
                    sscanf(line, "%c %s %[^\n]%*c", &levelTwo, tagTwo, valueTwo);
                   
                    if(strcmp("DATE", tagTwo) == 0 && levelTwo == '2') {
                        
                        tempEvent->date = malloc(sizeof(char) * (strlen(valueTwo) + 1));
                        strcpy(tempEvent->date, valueTwo);
                        
                    }else if(strcmp("PLAC", tagTwo) == 0 && levelTwo == '2') {
                        
                        tempEvent->place = malloc(sizeof(char) * (strlen(valueTwo) + 1));
                        strcpy(tempEvent->place, valueTwo);
                        
                    }else if(levelTwo == '2') {
                        
                        Field* tempField = malloc(sizeof(Field));
                        tempField->tag = malloc(sizeof(char) * strlen(tagTwo) + 2);
                        tempField->value = malloc(sizeof(char) * strlen(valueTwo) + 2);
                        strcpy(tempField->tag, tagTwo);
                        strcpy(tempField->value, valueTwo);
                        insertBack(&tempEvent->otherFields, tempField);
                        
                    }else if(levelTwo == '1' || levelTwo == '0') {
                        
                        fseek(fp, -(sizeof(char) * strlen(line)), SEEK_CUR);
                        break;
                    }
                }
                
                if(tempEvent->date == NULL) {
                   
                    tempEvent->date = (char*)malloc(sizeof(char) + 1);
                    strcpy(tempEvent->date, "");
                }
                
                if(tempEvent->place == NULL) {
                    
                    tempEvent->place = (char*)malloc(sizeof(char) + 1);
                    strcpy(tempEvent->place, "");
                }
                
                insertBack(&tempFam->events, tempEvent);
                break;
            }
        }
        
        if(strcmp("HUSB", tag) == 0) {
            
            ListIterator iter = createIterator(*linkerList);
        
            Linker* data;
            
            while( (data = (Linker*)nextElement(&iter)) != NULL) {
                
                if(strncmp(data->ID, value, strlen(value) - 1) == 0) {
                    
                    insertBack(&data->indiPtr->families, tempFam);
                    tempFam->husband = data->indiPtr;
                    break;
                }
            }
            
        }else if(strcmp("WIFE", tag) == 0) {
            
            ListIterator iter = createIterator(*linkerList);
          
            Linker* data;
            
            while( (data = (Linker*)nextElement(&iter)) != NULL) {
                
                if(strncmp(data->ID, value, strlen(value) - 1) == 0) {
                    
                    insertBack(&data->indiPtr->families, tempFam);
                    tempFam->wife = data->indiPtr;
                    break;
                }
            }
            
            
        }else if(strcmp("CHIL", tag) == 0) {
            
            ListIterator iter = createIterator(*linkerList);
         
            Linker* data;
            
            while( (data = (Linker*)nextElement(&iter)) != NULL) {
                
                if(strncmp(data->ID, value, strlen(value) - 1) == 0) {
                    
                    insertBack(&data->indiPtr->families, tempFam);
                    insertBack(&tempFam->children, data->indiPtr);
                    break;
                }
            }
            
        }else if(level != '0' && otherFlag != 1) {
            
            Field* tempField = malloc(sizeof(Field));
            tempField->tag = malloc(sizeof(char) * strlen(tag) + 2);
            tempField->value = malloc(sizeof(char) * strlen(value) + 2);
            strcpy(tempField->tag, tag);
            strcpy(tempField->value, value);
            
            insertBack(&tempFam->otherFields, tempField);
            
        }else if(level == '0' && strcmp(tag, famRef) != 0) {
            
            fseek(fp, -(sizeof(char) * strlen(line)), SEEK_CUR);
            break;
        }
        
    }
    
    return tempFam;
}

Individual* createIndividual(FILE* fp, char* indiRef, List* linkerList) {
    
    Individual* tempIndi = malloc(sizeof(Individual));
    tempIndi->givenName = NULL;
    tempIndi->surname = NULL;
    
    tempIndi->otherFields = initializeList(&printField, &deleteField, &compareFields);
    tempIndi->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
    tempIndi->families = initializeList(&printFamily, &deleteDummy, &compareFamilies);
    
    Linker* tempLink = malloc(sizeof(Linker));
    
    strcpy(tempLink->ID, indiRef);
    tempLink->indiPtr = tempIndi;
    
    insertBack(linkerList, tempLink);
    
    char level;
    char tag[20];
    char value[100];
    char line[260];
    
    char eventArray[23][4] = {"ADOP", "BIRT", "BAPM", "BARM", "BASM", "BLES", "BURI", "CENS", "CHR", "CHRA",
        "CONF", "CREM", "DEAT", "EMIG", "FCOM", "GRAD", "IMMI", "NATU", "ORDN",
        "RETI", "PROB", "WILL", "EVEN"};
    
    int otherFlag = 0;
  
    while(fgets(line, 250, fp) != NULL) {
        
        strcpy(value, " ");
        
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        for(int i = 0; i < 23; i++) {
            
            
            if(strncmp(eventArray[i], tag, strlen(tag) - 1) == 0) {
                
                otherFlag = 1;
                
                Event* tempEvent = malloc(sizeof(Event));
                tempEvent->date = NULL;
                tempEvent->place = NULL;
                
                tempEvent->otherFields = initializeList(&printField, &deleteField, &compareFields);
                strcpy(tempEvent->type, tag);
                
                while(fgets(line, 250, fp) != NULL) {
                    
                    strcpy(value, " ");
                    
                    sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
                  
                    if(strcmp("DATE", tag) == 0 && level == '2') {
                        
                        tempEvent->date = malloc(sizeof(char) * (strlen(value) + 1));
                        strcpy(tempEvent->date, value);
                        
                    }else if(strcmp("PLAC", tag) == 0 && level == '2') {
                        
                        tempEvent->place = malloc(sizeof(char) * (strlen(value) + 1));
                        strcpy(tempEvent->place, value);
                        
                    }else if(level == '2') {
                        
                        Field* tempField = malloc(sizeof(Field));
                        tempField->tag = malloc(sizeof(char) * strlen(tag) + 1);
                        tempField->value = malloc(sizeof(char) * strlen(value) + 1);
                        strcpy(tempField->tag, tag);
                        strcpy(tempField->value, value);
                        insertBack(&tempEvent->otherFields, tempField);
                        
                    }else if(level == '1' || level == '0') {
                        
                        fseek(fp, -(sizeof(char) * strlen(line)), SEEK_CUR);
                        break;
                    }
                }
                
                if(tempEvent->date == NULL) {
                    
                    tempEvent->date = (char*)malloc(sizeof(char) + 2);
                    strcpy(tempEvent->date, " ");
                }
                
                if(tempEvent->place == NULL) {
                    
                    tempEvent->place = (char*)malloc(sizeof(char) + 2);
                    strcpy(tempEvent->place, " ");
                }
                
                insertBack(&tempIndi->events, tempEvent);
                break;
            }
        }
        
        if(strcmp("NAME", tag) == 0) {
            
            char* pch = strtok(value, " ");
            
            if(pch[0] == '/') {

                tempIndi->givenName = malloc(sizeof(char) * 2);
                strcpy(tempIndi->givenName, " ");
                tempIndi->surname = malloc(sizeof(char) * (strlen(pch)+1));
                char* str = strtok(pch, "/");
                strcpy(tempIndi->surname, str);

            }else {
            
                tempIndi->givenName = malloc(sizeof(char) * (strlen(pch)+1));
                strcpy(tempIndi->givenName, pch);
            
                pch = strtok(NULL, " /");
               
                if(pch != NULL) {
                
                    tempIndi->surname = malloc(sizeof(char) * (strlen(pch)+1));
                    strcpy(tempIndi->surname, pch);
                    
                }
           }
            
        }else if(level != '0' && otherFlag != 1) {
            
            if(strcmp("FAMS", tag) != 0 && strcmp("FAMC", tag) != 0) {
            
                Field* tempField = malloc(sizeof(Field));
                tempField->tag = malloc(sizeof(char) * strlen(tag) + 2);
                tempField->value = malloc(sizeof(char) * strlen(value) + 2);
                strcpy(tempField->tag, tag);
                strcpy(tempField->value, value);
                
                insertBack(&tempIndi->otherFields, tempField);
            }
            
        }else if(level == '0' && strcmp(tag, indiRef) != 0) {
            
            fseek(fp, -(sizeof(char) * strlen(line)), SEEK_CUR);
            break;
        }
    }
    
    if(tempIndi->givenName == NULL) {
        
        tempIndi->givenName = (char*)malloc(sizeof(char) + 2);
        strcpy(tempIndi->givenName, " ");
    }
    
    if(tempIndi->surname == NULL) {
        
        tempIndi->surname = (char*)malloc(sizeof(char) + 2);
        strcpy(tempIndi->surname, " ");
    }
    
    return tempIndi;
}

Submitter* createSubmitter(FILE* fp) {
    
    rewind(fp);
    
    Submitter* subRecord = malloc(sizeof(Submitter) + (sizeof(char) * 50));
    
    subRecord->otherFields = initializeList(&printField, &deleteField, &compareFields);
    
    strcpy(subRecord->submitterName, "");
    strcpy(subRecord->address, "");
    
    char level;
    char tag[20];
    char value[100];
    char line[260];
    
    char subRef[100];
 
    while(fgets(line, 250, fp) != NULL) {
        
        strcpy(value, " ");
        
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        if(strcmp("SUBM", tag) == 0) {
            
            strcpy(subRef, value);
            rewind(fp);
            
            while(fgets(line, 250, fp) != NULL) {
                
                strcpy(value, " ");
                
                sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
                
                if(strncmp(subRef, tag, (strlen(subRef) - 1)) == 0) {
                    
                    char levelTwo;
                    char tagTwo[20];
                    char valueTwo[100];
                    char lineTwo[260];
                    
                    while(fgets(lineTwo, 250, fp) != NULL) {
                        
                        strcpy(valueTwo, " ");
                        
                        sscanf(lineTwo, "%c %s %[^\n]%*c", &levelTwo, tagTwo, valueTwo);
                        
                        if(levelTwo == '0') {
							
                            return subRecord;
                        }
                        
                        if(strcmp("NAME", tagTwo) == 0) {
                            
                            strcpy(subRecord->submitterName, valueTwo);
                            
                        }else if(strcmp("ADDR", tagTwo) == 0) {
                            
                            strcpy(subRecord->address, valueTwo);
                        
                        }else if(levelTwo != 0) {
                            
                            Field* tempField = malloc(sizeof(Field));
                            tempField->tag = malloc(sizeof(char) * strlen(tagTwo) + 1);
                            tempField->value = malloc(sizeof(char) * strlen(valueTwo) + 1);
                            strcpy(tempField->tag, tagTwo);
                            strcpy(tempField->value, valueTwo);
                            insertBack(&subRecord->otherFields, tempField);
                            
                        }
                    }
                }
            }
        }
    }
    
    return subRecord;
}

Header* createHeader(FILE* fp) {
   
    Header* tempHeader = malloc(sizeof(Header));
    
    tempHeader->submitter = createSubmitter(fp);
    
    tempHeader->otherFields = initializeList(&printField, &deleteField, &compareFields);
    
    rewind(fp);
    
    char level;
    char tag[20] = "";
    char value[100];
    char line[260];
    char prevTag[20];
    
    while(fgets(line, 250, fp) != NULL) {
        
        strcpy(value, " ");
        strcpy(prevTag, tag);
        
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        if(strcmp("SOUR", tag) == 0) {
            
            strcpy(tempHeader->source, value);
            
        }else if(strcmp("VERS", tag) == 0 && strcmp("GEDC", prevTag) == 0) {
            
            float version = atof(value);
            tempHeader->gedcVersion = version;
            
        }else if(strcmp("CHAR", tag) == 0) {
            
            if(strncmp("ANSEL", value, strlen(value) - 1) == 0) {
                
                tempHeader->encoding = ANSEL;
                
            }else if(strncmp("UTF-8", value, strlen(value) - 1) == 0) {
                
                tempHeader->encoding = UTF8;
                
            }else if(strncmp("UNICODE", value, strlen(value) - 1) == 0) {
                
                tempHeader->encoding = UNICODE;
                
            }else if(strncmp("ASCII", value, strlen(value) - 1) == 0) {
                
                tempHeader->encoding = ASCII;
            }
            
        }else if(level != '0' && strcmp("GEDC", tag) != 0 && strcmp("SUBM", tag) != 0) {
            
            Field* tempField = malloc(sizeof(Field));
            tempField->tag = malloc(sizeof(char) * strlen(tag)+1);
            tempField->value = malloc(sizeof(char) * strlen(value)+1);
            strcpy(tempField->tag, tag);
            strcpy(tempField->value, value);
            insertBack(&tempHeader->otherFields, tempField);
        }
        
        if(level == '0' && strcmp("HEAD", tag) != 0) {
            
            fseek(fp, -(sizeof(char) * strlen(line)), SEEK_CUR);
            return tempHeader;
        }
    }
    
    return tempHeader;
}

GEDCOMerror headerCheck(FILE* fp) {
    
    GEDCOMerror invalidHead = {.type = OK, .line = -1};
    
    char tagArray[20][4] = {"SOUR", "VERS", "NAME", "CORP", "DATA", "DATE", "COPR",
        "DEST", "TIME", "SUBM", "SUBN", "FILE", "GEDC", "FORM",
        "CHAR", "LANG", "PLAC", "NOTE", "CONC", "CONT"};
   
    char level;
    char tag[20] = "";
    char value[100];
    char line[260];
    char prevTag[20];
    
    int validFlag = 0;
    int lineCount = 1;
    int fieldCount = 0;
    
    rewind(fp);
    fgets(line, 250, fp);
    sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
    
    if(level != '0') {
        
        invalidHead.type = INV_HEADER;
        invalidHead.line = 1;
        return invalidHead;
    }

    while(fgets(line, 250, fp) != NULL) {
        
        ++lineCount;
        strcpy(value, "");
        strcpy(prevTag, tag);
        
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        if(level == '0')
            break;
        
        if(level == ' ') {
            
            invalidHead.type = INV_HEADER;
            invalidHead.line = lineCount;
            break;
        }
        
        if(strcmp("SOUR", tag) == 0) {
            
            ++fieldCount;
            
            if(strcmp(value, "") == 0) {
                
                invalidHead.type = INV_HEADER;
                invalidHead.line = lineCount;
                break;
            }
            
        }else if(strcmp("VERS", tag) == 0 && strcmp("GEDC", prevTag) == 0) {
            
             ++fieldCount;
            
            if(strcmp(value, "") == 0) {
                
                invalidHead.type = INV_HEADER;
                invalidHead.line = lineCount;
                break;
            }
            
        }else if(strcmp("CHAR", tag) == 0) {
            
             ++fieldCount;
            
            if(strcmp(value, "") == 0) {
                
                invalidHead.type = INV_HEADER;
                invalidHead.line = lineCount;
                break;
            }
            
        }else if(strcmp("SUBM", tag) == 0) {
            
             ++fieldCount;
            
            if(strcmp(value, "") == 0) {
                
                invalidHead.type = INV_HEADER;
                invalidHead.line = lineCount;
                break;
            }
        }
        
        for(int i = 0; i < 20; i++) {
            
            if(strncmp(tagArray[i], tag, strlen(tag) - 1) == 0) {
                
                validFlag = 1;
                break;
                
            }else {
                
                validFlag = 0;
            }
        }
        
        if(validFlag != 1) {
            
            invalidHead.type = INV_HEADER;
            invalidHead.line = lineCount;
            break;
        }
    }
    
    if(fieldCount != 4 && invalidHead.type == OK) {
        
        invalidHead.type = INV_HEADER;
        invalidHead.line = lineCount;
    }
    
    return invalidHead;
}

GEDCOMerror gedCheck(FILE* fp) {
    
    GEDCOMerror invalidGEDCOM = {.type = INV_GEDCOM, .line = -1};
    GEDCOMerror success = {.type = OK, .line = -1};
    bool validTrail = false;
    bool validSub = false;
    char level;
    char tag[20];
    char value[100];
    char line[260];
  
    rewind(fp);
    fgets(line, 250, fp);
    sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
    
    if(strcmp("HEAD", tag) != 0) {
        
        return invalidGEDCOM;
    }
    
    while(fgets(line, 250, fp) != NULL) {
        
        strcpy(value, "");
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
        
        if(strncmp("SUBM", value, (strlen(value) - 1)) == 0 && strlen(value) > 1)
            validSub = true;
        
        if(strcmp("TRLR", tag) == 0)
            validTrail = true;
        
    }
    
    if(validTrail != true) {
        
        return invalidGEDCOM;
    }
        
    if(validSub != true) {
        
        return invalidGEDCOM;
    }
    
    return success;
}

GEDCOMerror recordCheck(FILE* fp) {
    
    GEDCOMerror invalidRecord = {.type = INV_RECORD, .line = -1};
    GEDCOMerror success = {.type = OK, .line = -1};
    
    char level = '\0';
    char tag[20];
    char value[100];
    char line[260];
    
    
    int lineCount = 0;
    char prevLevel = '\0';
 
    while(fgets(line, 257, fp) != NULL) {
        
        ++lineCount;
        prevLevel = level;
        
        if(strlen(line) > 255) {
            
            invalidRecord.line = lineCount;
            return invalidRecord;
        }
        
        strcpy(value, "");
        sscanf(line, "%c %s %[^\n]%*c", &level, tag, value);
     
        if(atoi(&level) > (atoi(&prevLevel) + 1)) {

            invalidRecord.line = lineCount;
            return invalidRecord;
            
        }else if(atoi(&level) > 2) {
			
			invalidRecord.line = lineCount;
            return invalidRecord;
		}
    }
    
    return success;
}

////// Assignment 2 ///////////////

GEDCOMerror writeHeader(FILE* fp, const GEDCOMobject* obj) {
    
    GEDCOMerror writeSuccess = {.type = OK, .line = -1};
    GEDCOMerror writeFail = {.type = WRITE_ERROR, .line = -1};
    
    if(fp == NULL)
        return writeFail;
    
    if(obj == NULL)
        return writeFail;
    
    fputs("0 HEAD\n", fp);
    
    fprintf(fp, "1 SOUR %s\n", obj->header->source);
    fprintf(fp, "1 GEDC\n2 VERS %.1f\n", obj->header->gedcVersion);
    
    fputs("2 FORM LINEAGE-LINKED\n", fp);
    
    switch(obj->header->encoding) {
            
        case ANSEL:
            
            fputs("1 CHAR ANSEL\n", fp);
            break;
            
        case UTF8:
            
            fputs("1 CHAR UTF-8\n", fp);
            break;
            
        case UNICODE:
            
            fputs("1 CHAR UNICODE\n", fp);
            break;
            
        case ASCII:
            
            fputs("1 CHAR ASCII\n", fp);
            break;
            
        default:
            
            fputs("1 CHAR *ERROR*\n", fp);
            break;
    }
    
    fputs("1 SUBM @SUB1@\n", fp);
    
    return writeSuccess;
}

GEDCOMerror writeSubmitter(FILE* fp, const GEDCOMobject* obj) {
    
    GEDCOMerror writeSuccess = {.type = OK, .line = -1};
    GEDCOMerror writeFail = {.type = WRITE_ERROR, .line = -1};
    
    if(fp == NULL)
        return writeFail;
    
    if(obj == NULL)
        return writeFail;
    
    fputs("0 @SUB1@ SUBM\n", fp);
    
    fprintf(fp, "1 NAME %s\n", obj->submitter->submitterName);
    
    if(strlen(obj->submitter->address) > 0) {
        
        fprintf(fp, "1 ADDR %s\n", obj->submitter->address);
    }
    
    return writeSuccess;
}

GEDCOMerror writeIndividual(FILE* fp, const GEDCOMobject* obj) {
    
    GEDCOMerror writeSuccess = {.type = OK, .line = -1};
    GEDCOMerror writeFail = {.type = WRITE_ERROR, .line = -1};
    
    if(fp == NULL)
        return writeFail;
    
    if(obj == NULL)
        return writeFail;
    
    if(getLength(obj->individuals) != 0) {
        
        Individual* indi;
        ListIterator iter = createIterator(obj->individuals);
        
        while( (indi = (Individual*)nextElement(&iter)) != NULL) {
            
            char* ptrAddr = formatAddr((void*)indi);
            fprintf(fp, "0 @I%s@ INDI\n", ptrAddr);
            free(ptrAddr);
           
            if(strncmp(indi->givenName, " ", strlen(indi->givenName)-1) == 0) {
            
                fputs("1 NAME  ", fp);
            
            }else {
                
                fprintf(fp, "1 NAME %s ", indi->givenName);
            }
            
            if(strncmp(indi->surname, " ", strlen(indi->surname)-1) == 0) {
               
                fputs("//\n", fp);
                
            }else {
                
                fprintf(fp, "/%s/\n", indi->surname);
            }
            
            if(getLength(indi->otherFields) != 0) {
            
                Field* field;
                ListIterator iterFld = createIterator(indi->otherFields);
                
                while( (field = (Field*)nextElement(&iterFld)) != NULL) {
                    
                    if(strcmp("GIVN", field->tag) == 0) {
                        
                        fprintf(fp, "2 GIVN %s\n", field->value);
                        
                    }else if(strcmp("SURN", field->tag) == 0) {
                        
                        fprintf(fp, "2 SURN %s\n", field->value);
                    
                    }else {
                        
                        fprintf(fp, "1 %s %s\n", field->tag, field->value);
                    }
                }
            }
            
            if(getLength(indi->events) != 0) {
                
                Event* event;
                ListIterator iterEvt = createIterator(indi->events);
                
                while( (event = (Event*)nextElement(&iterEvt)) != NULL) {
                    
                    fprintf(fp, "1 %s\n", event->type);
                    
                    if(strncmp(event->date, " ", strlen(event->date)-1) != 0)
                        fprintf(fp, "2 DATE %s\n", event->date);
                    
                    if(strncmp(event->place, " ", strlen(event->place)-1) != 0)
                        fprintf(fp, "2 PLAC %s\n", event->place);
                    
                }
            }
            
            if(getLength(indi->families) != 0) {
                
                Family* family;
                ListIterator iterFam = createIterator(indi->families);
                
                while( (family = (Family*)nextElement(&iterFam)) != NULL) {
                    
                    if(family->husband == indi || family->wife == indi) {
                        
                        ptrAddr = formatAddr((void*)family);
                        fprintf(fp, "1 FAMS @F%s@\n", ptrAddr);
                        free(ptrAddr);
                        
                    }else {
                        
                        ptrAddr = formatAddr((void*)family);
                        fprintf(fp, "1 FAMC @F%s@\n", ptrAddr);
                        free(ptrAddr);
                    }
                }
            }
        }
    }
    
    return writeSuccess;
}

GEDCOMerror writeFamily(FILE* fp, const GEDCOMobject* obj) {
    
    GEDCOMerror writeSuccess = {.type = OK, .line = -1};
    GEDCOMerror writeFail = {.type = WRITE_ERROR, .line = -1};
    
    if(fp == NULL)
        return writeFail;
    
    if(obj == NULL)
        return writeFail;
    
    if(getLength(obj->families) != 0) {
        
        Family* family;
        ListIterator iterFam = createIterator(obj->families);
        
        while( (family = (Family*)nextElement(&iterFam)) != NULL) {
        
            char* ptrAddr = formatAddr((void*)family);
            fprintf(fp, "0 @F%s@ FAM\n", ptrAddr);
            free(ptrAddr);
            
            if(family->husband != NULL) {
                
                ptrAddr = formatAddr((void*)family->husband);
                fprintf(fp, "1 HUSB @I%s@\n", ptrAddr);
                free(ptrAddr);
            }
            
            if(family->wife != NULL) {
                
                ptrAddr = formatAddr((void*)family->wife);
                fprintf(fp, "1 WIFE @I%s@\n", ptrAddr);
                free(ptrAddr);
            }
            
            if(getLength(family->events) != 0) {
                
                Event* event;
                ListIterator iterEvt = createIterator(family->events);
                
                while( (event = (Event*)nextElement(&iterEvt)) != NULL) {
                    
                    fprintf(fp, "1 %s\n", event->type);
                    
                    if(strcmp(event->date, "") != 0)
                        fprintf(fp, "2 DATE %s\n", event->date);
                    
                    if(strcmp(event->place, "") != 0)
                        fprintf(fp, "2 PLAC %s\n", event->place);
                    
                }
            }
            
            if(getLength(family->children) != 0) {
                
                Individual* indi;
                ListIterator iterIndi = createIterator(family->children);
                
                while( (indi = (Individual*)nextElement(&iterIndi)) != NULL) {
                    
                    ptrAddr = formatAddr((void*)indi);
                    fprintf(fp, "1 CHIL @I%s@\n", ptrAddr);
                    free(ptrAddr);
                }
            }
        }
    }
    
    return writeSuccess;
}

char* formatAddr(void* data) {
    
    char temp[30];
    char* ptrAddr = malloc(sizeof(char) * 5);
    
    sprintf(temp, "%p", data);
    strcpy(ptrAddr, &temp[strlen(temp)-4]);
    
    for(int i = 0; i < strlen(ptrAddr); i++) {
        
        ptrAddr[i] = toupper(ptrAddr[i]);
    }
    
    return ptrAddr;
}

int sortGen(const void* first, const void* second) {
    
    Individual* I1 = (Individual*)first;
    Individual* I2 = (Individual*)second;
    
    if(strncmp(I1->surname, " ", strlen(I1->surname)-1) == 0 || strncmp(I2->surname, " ", strlen(I2->surname)-1) == 0 ) {
        
        return 1;
        
    }else if(strcmp(I1->surname, I2->surname) == 0) {
        
        return strcmp(I1->givenName, I2->givenName);
    }
    
    return strcmp(I1->surname, I2->surname);
}

char* createFileSum(char* filename) {
    
    GEDCOMobject* obj = NULL;
    
    GEDCOMerror error;
    
    error = createGEDCOM(filename, &obj);
    
    if(error.type != OK)
		return calloc(1, sizeof(char));
    
    if(obj == NULL)
        return calloc(1, sizeof(char));
    
    char* string = malloc(sizeof(char) * 200);
    
    int indNum = getLength(obj->individuals);
    int famNum = getLength(obj->families);
    char encod[10];
    
    switch(obj->header->encoding) {
            
        case ANSEL:
            
            strcpy(encod, "ANSEL");
            break;
            
        case UTF8:
            
            strcpy(encod, "UTF8");
            break;
            
        case UNICODE:
            
            strcpy(encod, "UNICODE");
            break;
            
        case ASCII:
            
            strcpy(encod, "ASCII");
            break;
            
        default:
            
            strcpy(encod, "ERROR");
            break;
    }
    
    sprintf(string, "{\"source\":\"%s\",\"GEDCvers\":\"%.1f\",\"encoding\":\"%s\",\"subName\":\"%s\",\"subAddr\":\"%s\",\"numInd\":\"%d\",\"numFam\":\"%d\"}",
            obj->header->source, obj->header->gedcVersion, encod, obj->submitter->submitterName, obj->submitter->address, indNum, famNum);
    
    deleteGEDCOM(obj);
    
    return string;
    
}

char* populateGEDview(char* filename) {
    
    GEDCOMobject* obj = NULL;
    
    GEDCOMerror error;
    
    error = createGEDCOM(filename, &obj);
    
    if(error.type != OK)
		return calloc(1, sizeof(char));
    
    if(obj == NULL)
        return calloc(1, sizeof(char));
    
    if(getLength(obj->individuals) == 0)
        return calloc(1, sizeof(char));
    
    
    char* string = malloc(sizeof(char) * 50);
    
    strcpy(string, "[");
    
    Individual* indi;
    ListIterator iter = createIterator(obj->individuals);
    
    while( (indi = (Individual*)nextElement(&iter)) != NULL) {
        
        char gender[5] = "?";
        int famSize = 0;
        
        char* JSONstr = indToJSON(indi);
        
        Field* field;
        ListIterator iterFld = createIterator(indi->otherFields);
        
        while( (field = (Field*)nextElement(&iterFld)) != NULL) {
         
            if(strcmp(field->tag, "SEX") == 0)
                strcpy(gender, field->value);
                
        }
        
        if(getLength(indi->families) > 0) {
        
            Family* family;
            ListIterator iterFam = createIterator(indi->families);
            
            while( (family = (Family*)nextElement(&iterFam)) != NULL) {
                
                if(family->wife == indi || family->husband == indi) {
                
                    if(family->wife != NULL)
                        ++famSize;
                    
                    if(family->husband != NULL)
                        ++famSize;
                    
                    famSize += getLength(family->children);
            
                }else {
                    
                    ++famSize;
                }
            }
            
        }else {
            
            ++famSize;
        }
        
        char* bonus = malloc(sizeof(char) * 100);
        
        sprintf(bonus, ",\"gender\":\"%s\",\"famSize\":\"%d\"}", gender, famSize);
        
        JSONstr[strlen(JSONstr)-1] = 0;
        
        string = realloc(string, sizeof(char) * (strlen(string) + strlen(JSONstr) + strlen(bonus) + 5));
        
        strcat(string, JSONstr);
        strcat(string, bonus);
        
        if(iter.current != NULL)
            strcat(string, ",");
        
        free(bonus);
        free(JSONstr);
        
    }
    
    strcat(string, "]");
    
    deleteGEDCOM(obj);
    
    return string;
}

char* addJSONind(char* indStr, char* filename) {
    
    GEDCOMobject* obj = NULL;
    
    GEDCOMerror error;
    
    error = createGEDCOM(filename, &obj);
    
    if(error.type != OK)
		return calloc(1, sizeof(char));
    
    if(obj == NULL)
        return calloc(1, sizeof(char));
    
    Individual* indi = JSONtoInd(indStr);
    
    addIndividual(obj, indi);
    
    error = writeGEDCOM(filename, obj);
    
    char* returnStr = malloc(sizeof(char) * 50);
    
    if(error.type == OK) {
        
        sprintf(returnStr, "Added a new individual, %s %s, to file ", indi->givenName, indi->surname);
        deleteGEDCOM(obj);
        return returnStr;
        
    }
        
    strcpy(returnStr, "FAIL");
    deleteGEDCOM(obj);
    
    return returnStr;
}

char* getJSONdesc(char* filename, char* indStr, int max) {
    
    Individual* indi = JSONtoInd(indStr);
    
    if(indi == NULL)
        return calloc(1, sizeof(char));
    
    GEDCOMobject* obj = NULL;
    
    GEDCOMerror error;
    
    error = createGEDCOM(filename, &obj);
    
    if(error.type != OK)
		return calloc(1, sizeof(char));
    
    if(obj == NULL)
        return calloc(1, sizeof(char));
    
    Individual* indiRef = findPerson(obj, customSearch, indi);
    
    List theList = getDescendantListN(obj, indiRef, max);
    
    char* string = gListToJSON(theList);
    
    clearList(&theList);
    deleteGEDCOM(obj);
    
    return string;
}

char* getJSONancs(char* filename, char* indStr, int max) {
    
    Individual* indi = JSONtoInd(indStr);
    
    if(indi == NULL)
        return calloc(1, sizeof(char));
    
    GEDCOMobject* obj = NULL;
    
    GEDCOMerror error;
    
    error = createGEDCOM(filename, &obj);
    
    if(error.type != OK)
		return calloc(1, sizeof(char));
    
    if(obj == NULL)
        return calloc(1, sizeof(char));
    
    Individual* indiRef = findPerson(obj, customSearch, indi);
    
    List theList = getAncestorListN(obj, indiRef, max);
    
    char* string = gListToJSON(theList);
    
    clearList(&theList);
    deleteGEDCOM(obj);
    
    return string;
}


