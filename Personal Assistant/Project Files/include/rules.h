#ifndef rules_h
#define rules_h

#include <stdio.h>

typedef struct rule {
    
    char keyword[50];
    char textResponse[100];
    double userRating;
    double systemRating;
    double learningRate;
    int occurences;
    
} Rule;

Rule* createRule(char key[50], char text[100], double usrRating, double sysRating, double learnRate, int occur);

double getUR(void* data);

double getSR(void* data);

double getLR(void* data);

int getOccur(void* data);

char *getResponse(void* data);

void setUR(void* data, double newUR);

void setOccur(void* data, int newO);

void setSR(void* data, double newSR);

int compare(const void* a, const void* b);

void delete(void* data);


void print(void* data);


#endif /* rules_h */
