#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BinarySearchTreeAPI.h"
#include "rules.h"

void addRule(Tree* theTree);
void removeRule(Tree* theTree);
void modifyRule(Tree* theTree);
void discuss(Tree* theTree);

int main(int argc, char ** argv) {
    
    FILE* fp;
    
    if(argc >= 2) {
        
        fp =  fopen(argv[1], "r");
        
    }else {
        
        printf("ERROR: Missing input file.\n");
        exit(1);
    }
    
    if(fp == NULL) {
        
        printf("ERROR: Received a NULL file.\n");
        exit(1);
    }
 
    Tree* theTree = createBinTree(&compare, &delete);
    
    char line[100];
    char key[50];
    char sentence[100];
    double uRate = 0.5;
    double sRate = uRate;
    double lRate = 0.05;
    int occur = 0;
    
    int curLine = 0;
    
    Rule* rule;
    
    while(fgets(line,100,fp) != NULL) {
        
        if(curLine == 0) {
            
            sscanf(line,"%s", key);
            ++curLine;
            
            for(int l = 0; l < strlen(key); l++)
                key[l] = tolower(key[l]);
        
        }else if(curLine == 1) {
            
            sscanf(line,"%[^\n]%*c", sentence);
            
            rule = createRule(key, sentence, uRate, sRate, lRate, occur);
            
            addToTree(theTree, rule);
            
            curLine = 0;
        }
        
    }
    
    fclose(fp);
    
    int exitFlag = 0;
    char input[10];
    
    printf("\nWelcome!\n");
    
    while(exitFlag == 0) {
        
        printf("\nMenu Options:\n");
        printf("1. Add a new rule.\n");
        printf("2. Remove a rule.\n");
        printf("3. Display rules.\n");
        printf("4. Modify rule rating.\n");
        printf("5. Discuss.\n");
        printf("9. Quit.\n");
        printf("Please choose a menu option by its corresponding number: \n");
        
        fgets(input, 10, stdin);
        printf("\n");
        
        if(strncmp(input, "1", 1) == 0) {
            
            addRule(theTree);
            
        }else if(strncmp(input, "2", 1) == 0) {
            
            removeRule(theTree);
            
        }else if(strncmp(input, "3", 1) == 0) {
            
            printInOrder(theTree, print);
       
        }else if(strncmp(input, "4", 1) == 0) {
            
            modifyRule(theTree);
        
        }else if(strncmp(input, "5", 1) == 0) {
            
            discuss(theTree);
       
        }else if(strncmp(input, "9", 1) == 0) {
            
            printf("\nGoodbye!\n");
            exitFlag = 1;
            
        }else {
            
            printf("ERROR: Input must be interger values 1-5, or 9 to quit.\n");
        }
    }
    
    destroyBinTree(theTree);
    
    return 0;
}

void addRule(Tree* theTree) {
    
    char key[50];
    char sentence[100];
    double uRate;
    double lRate = 0.05;
    int occur = 0;
    int check = 0;
    
    printf("*Add Rule*\n\n");
    
    printf("Please enter a keyword for the new rule: \n");
    scanf("%s", key);
    getchar();
    
    for(int l = 0; l < strlen(key); l++)
        key[l] = tolower(key[l]);
    
    printf("Please enter a sentence for the new rule: \n");
    fgets(sentence, 100, stdin);
   
    while(check != 1) {
    
        printf("Please enter user importance of the new rule (0.0 to 1.0): \n");
        scanf("%lf", &uRate);
        getchar();
        
        if(uRate < 0.0 || uRate > 1.0) {
            
            printf("\nRating must be between 0.0 and 1.0\n");
            
        }else {
            
            check = 1;
        }
    }
    
    Rule* rule = createRule(key, sentence, uRate, uRate, lRate, occur);
    
    addToTree(theTree, rule);
    
    printf("Rule added succesfully!\n");
    
}

void removeRule(Tree* theTree) {
    
    char key[50];
    
    printf("*Remove Rule*\n\n");
    
    printf("Please enter the keyword of the rule to be removed: \n");
    scanf("%s", key);
    getchar();
    
    for(int l = 0; l < strlen(key); l++)
        key[l] = tolower(key[l]);
    
    Rule* tempRule = createRule(key, "TEST", 0, 0, 0, 0);
    
    TreeNode* toDel = findInTree(theTree, tempRule);
    free(tempRule);
    
    if(toDel != NULL) {
        
        removeFromTree(theTree, toDel);
        printf("\nRule was succesfully removed!\n");
    
    }else {
        
        printf("\nA rule associated with the keyword does not exist.\n");
    }
}

void modifyRule(Tree* theTree) {
    
    char key[50];
    double uRate;
    int check = 0;
    
    printf("*Modify Rule*\n\n");
    
    printf("Please enter the keyword of the rule to be modified: \n");
    scanf("%s", key);
    getchar();
    
    for(int l = 0; l < strlen(key); l++)
        key[l] = tolower(key[l]);
    
    Rule* tempRule = createRule(key, "TEST", 0, 0, 0, 0);
    
    TreeNode* ruleNode = findInTree(theTree, tempRule);
    free(tempRule);
    
    if(ruleNode != NULL) {
        
        printf("\nCurrent User Rating: %.2lf\n\n", getUR(ruleNode));
        
        while(check != 1) {
        
            printf("Please enter a new user rating for the rule (0.0 to 1.0): \n");
            scanf("%lf", &uRate);
            getchar();
            
            if(uRate < 0.0 || uRate > 1.0) {
                
                printf("\nRating must be between 0.0 and 1.0\n");
                
            }else {
                
                check = 1;
            }
        }
        
        setUR(ruleNode, uRate);
        printf("\nRule modified succesfully!\n");
    
    }else {
        
        printf("\nA rule associated with the entered keyword does not exist.\n");
    }
}

void discuss(Tree* theTree) {
    
    char userQuestion[100];
    int quit = 0;
    
    printf("*Discussion*\n\n");
    printf("\nPersonal Assistant: Ask away!\n");
    
    while(quit != 1) {
    
        printf("\nUser: ");
        fgets(userQuestion, 100, stdin);
        
        for(int l = 0; l < strlen(userQuestion); l++)
            userQuestion[l] = tolower(userQuestion[l]);
        
        if(strcmp(userQuestion, "quit\n") == 0) {
            
            printf("\nPersonal Assistant: It was nice while it lasted, goodbye!\n");
            break;
        }
            
        
        int length = strlen(userQuestion);
        int wordCount = 0;
        
        for(int i = 0; i < length; i++) {

            if(isspace(userQuestion[i]))
                ++wordCount;
        }
        
        char *token;
        char tokArray[wordCount][20];
        int j = 0;
        
        token = strtok(userQuestion, " ,.!?\n");
        
        while(token != NULL) {
            
            strcpy(tokArray[j], token);
            token = strtok(NULL, " ,.!?\n");
            j++;
        }
        
        double topRating = 0;
        TreeNode* topRule = NULL;

        for(int k = 0; k < j; k++) {

            Rule* tempRule = createRule(tokArray[k], "TEST", 0, 0, 0, 0);

            TreeNode* ruleNode = findInTree(theTree, tempRule);
            free(tempRule);

            if(ruleNode != NULL) {
            
                if((getUR(ruleNode) + getSR(ruleNode)) > topRating) {
                    
                    topRating = (getUR(ruleNode) + getSR(ruleNode));
                    topRule = ruleNode;
                }
            }
        }

        if(topRule != NULL) {

            char input[10];
            int exitFlag = 0;
            double sysRate = 0;
            double lrnRate = 0;
            int occur = 0;
            
            printf("\nPersonal Assistant: %s\n", getResponse(topRule));
            
            occur = getOccur(topRule);
            setOccur(topRule, occur + 1);
            
            sysRate = getSR(topRule);
            lrnRate = getLR(topRule);
            occur = getOccur(topRule);
            
            while(exitFlag != 1) {
            
                printf("\nPersonal Assistant: Was this feedback helpful? 'y' or 'n'\n");
                printf("\nUser: ");
                scanf("%s", input);
                getchar();
                
                if(strcmp(input, "y") == 0) {
                    
                    sysRate = sysRate + ( (sysRate * lrnRate) / occur );
                    
                    if(sysRate > 1)
                        sysRate = 1;
                    
                    setSR(topRule, sysRate);
                    exitFlag = 1;
                    printf("\nPersonal Assistant: Look how fast I am learning! Elon wouldn't be pleased...\n");
                    
                }else if(strcmp(input, "n") == 0) {
                    
                    sysRate = sysRate - ( (sysRate * lrnRate) / occur );
                    
                    if(sysRate < 0)
                        sysRate = 0;
                    
                    setSR(topRule, sysRate);
                    exitFlag = 1;
                    printf("\nPersonal Assistant: Success comes from learning on failure! Atleast that's what humans say, how would I know?\n");
                    
                }else {
                    
                    printf("\nPersonal Assistant: Invalid input, try again!\n");
                }
            }
            
        }else {

            printf("\nPersonal Assistant: I couldn't think of a response!\n");
        }
        
         printf("\nPersonal Assistant: Feel free to ask me another question, or just say 'quit' to leave.\n");
    }
}




