#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rules.h"

Rule* createRule(char key[50], char text[100], double usrRating, double sysRating, double learnRate, int occur) {
    
    Rule* r = (Rule*)malloc(sizeof(Rule));
    
    strcpy(r->keyword, key);
    strcpy(r->textResponse, text);
    r->userRating = usrRating;
    r->systemRating = sysRating;
    r->learningRate = learnRate;
    r->occurences = occur;
    
    return r;
}

double getUR(void* data) {
    
    Rule* rle = (Rule*)data;
    
    return rle->userRating;
}

double getSR(void* data) {
    
    Rule* rle = (Rule*)data;
    
    return rle->systemRating;
}

double getLR(void* data) {
    
    Rule* rle = (Rule*)data;
    
    return rle->learningRate;
}

int getOccur(void* data) {
    
    Rule* rle = (Rule*)data;
    
    return rle->occurences;
}

char *getResponse(void* data) {
    
    Rule* rle = (Rule*)data;
    
    return rle->textResponse;
}

void setUR(void* data, double newUR) {
    
    Rule* rle = (Rule*)data;
    
    rle->userRating = newUR;
}

void setSR(void* data, double newSR) {
    
    Rule* rle = (Rule*)data;
    
    rle->systemRating = newSR;
}

void setOccur(void* data, int newO) {
    
    Rule* rle = (Rule*)data;
    
    rle->occurences = newO;
}


int compare(const void* a, const void* b) {
    
    Rule* rule1 = (Rule*)a;
    Rule* rule2 = (Rule*)b;
    
    return strcmp(rule1->keyword, rule2->keyword);
}

void delete(void* data) {
    
}

void print(void* data) {
 
    Rule* rule = (Rule*)data;
    
    printf("%s     \tRating: %.2lf \tSystem: %.2lf \tOccurences: %d\n", rule->keyword, rule->userRating, rule->systemRating, rule->occurences);
}
