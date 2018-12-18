#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEBUG 1
#define NUM_ARGS 4
#define MAX_ARG_LEN 255

void initArray(char *[], int);
void promptArgs(char *[]);
int scanLine(char *, int);
void printStringArray(char *[], int);
int splitArg(char *, char *[]);
void freeArray(char *[], int);
void execute(char *, char *[]);
void printDelimiter(char *[], int);
void poke(const char *);

int main(void) {
    //initialize the array of arg pointers
    char *args[NUM_ARGS];
    promptArgs(args);
}

/****************************************************************************
*Prompts user for command line arguments and stores them in the args variable
*as char arrays.
*
*@params
*-**args: Pointer to character array storing the command line arguments.
****************************************************************************/
void promptArgs(char *args[]) {
    //Prompt for 3 commands
    for(int i = 0; i < NUM_ARGS - 1; i++) {
        printf("mash-%d>", i + 1);
        scanLine(args[i], MAX_ARG_LEN);
    }
    //prompt for file
    printf("file>");
    scanLine(args[NUM_ARGS - 1], MAX_ARG_LEN);
    //Concatenate the input file to the end of each command
    char *temp;
    for(int i = 0; i < NUM_ARGS - 1; i++) {
        if(i == 0) {
            temp = malloc(sizeof(char) * (strlen(args[i]) + strlen(args[NUM_ARGS - 1]) + 1));
        }
        else {
            temp = realloc(temp, (sizeof(char) * (strlen(args[i]) + strlen(args[NUM_ARGS - 1]) + 1))); //<<<Segfault occurs here
        }
        strcpy(temp, args[i]);
        strcat(temp, args[NUM_ARGS - 1]);
        //free(args[i]);
        args[i] = temp;
    }
}

/****************************************************************************
*Scans a line of input and stores it in the given char array. Skips new line
*char. Memory is allocated to the given string.
*
*@params
*-*string: Destination of the captured input string.
*-len: The length of the destination char array.
****************************************************************************/
int scanLine(char *string, int len) {
    char curr;
    char temp[MAX_ARG_LEN];
    int i = 0;
    while((curr = getchar()) != '\n' && i < len) {
        temp[i++] = curr;
    }
    temp[++i] = '\0';
    string = malloc(i * sizeof(char));
    strcpy(string, temp);
    return i;
}
