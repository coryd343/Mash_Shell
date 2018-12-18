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
    //initialize temp files
    //char *temp[] = {" > com1output.txt", " > com2output.txt", " > com3output.txt"};

    //initialize the array of arg pointers
    char *args[NUM_ARGS];
    //initArray(args, NUM_ARGS);
    promptArgs(args);
    for(int i = 0; i < 4; i++) {
        printf("args[%d]: %s|\n", i, args[i]);
    }
    
    /*for(int i = 0; i < NUM_ARGS - 1; i++) {
        args[i] = realloc(args[i], sizeof(char) * (strlen(args[i]) + strlen(temp[i]) + 1));
        strcat(args[i], temp[i]);
    }*/
    //printStringArray(args, NUM_ARGS);
    
    //Tokenize each command
    char *com1[MAX_ARG_LEN], *com2[MAX_ARG_LEN], *com3[MAX_ARG_LEN];
    int count1, count2, count3;
    initArray(com1, MAX_ARG_LEN);
    initArray(com2, MAX_ARG_LEN);
    initArray(com3, MAX_ARG_LEN);
    count1 = splitArg(args[0], com1);
    count2 = splitArg(args[1], com2);
    count3 = splitArg(args[2], com3);
    printStringArray(com1, count1);
    //printStringArray(com2, count2);
    //printStringArray(com3, count3);
    
    //Fork & Execute Commands
    //execute(com1[0], com1);
    int p1 = fork();
    if(p1 == 0) {
        //execute com1
        //poke("fork 1");
        printDelimiter(args, 0);
        execute(com1[0], com1);
    }
    if(p1 > 0) {
        int p2 = fork();
        if(p2 == 0) {
            //execute com2
            poke("fork 2");
            printDelimiter(args, 1);
            execute(com2[0], com2);
        }
        if(p2 > 0) {
            int p3 = fork();
            if(p3 == 0) {
                //execute com3
                poke("fork 3");
                printDelimiter(args, 2);
                execute(com3[0], com3);
            }
            if(p3 > 0) {
                int wait3 = wait(NULL);
                poke("All processes finished!");
            }
        }
    }
    
    poke("finish!");
    return 0;
}

/****************************************************************************
*Allocates MAX_ARG_LEN bytes of memory to each element of an array.
*
*@params
*-*args[]: Pointer to character arrary storing the command line arguments.
****************************************************************************/
void initArray(char *args[], int elements) {
    for(int i = 0; i < elements; i++) {
        args[i] = calloc(MAX_ARG_LEN, sizeof(char));
    }
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
    char temp[MAX_ARG_LEN];
    for(int i = 0; i < NUM_ARGS - 1; i++) {
        printf("mash-%d>", i + 1);
        scanf(" %[^\n]", temp);
        args[i] = malloc(sizeof(char) * strlen(temp));
        strcpy(args[i], temp);
    }
    //prompt for file
    printf("file>");
    scanf(" %[^\n]", temp);
    args[NUM_ARGS - 1] = malloc(sizeof(char) * strlen(temp));
    strcpy(args[NUM_ARGS - 1], temp);
    
    //Concatenate the input file to the end of each command
    for(int i = 0; i < NUM_ARGS - 1; i++) {
        args[i] = realloc(args[i], (sizeof(char) * (strlen(args[i]) + strlen(args[NUM_ARGS - 1]) + 2)));
        strcat(args[i], " ");
        strcat(args[i], args[NUM_ARGS - 1]);
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

/****************************************************************************
*Prints every element of an array of char pointers
*
*@params
*-*in[]: The array of char pointers.
*-len: The number of elements in the array.
****************************************************************************/
void printStringArray(char *in[], int len) {
    for(int i = 0; i < len; i++) {
        printf("[%s]", in[i]);
    }
    printf("\n");
}

/****************************************************************************
*Takes a string and splits it into tokens separated by whitespace.
*
*@params
*-*arg: Source character array containing the string to split.
*-*argArray[]: Destination array of the tokens from arg.
*
*@return: The number of tokens the string was split into.
****************************************************************************/
int splitArg(char *arg, char *argArray[]) {    
    int i = 0;  
    char *temp = strtok(arg, " ");    
    while(temp != NULL) {
        argArray[i++] = temp;
        temp = strtok(NULL, " ");
    }
    /*poke("After while loop");
    argArray[i++] = fileName;
    poke("After file name added");
    char redirect[1] = ">";
    char *temp2 = strcat(redirect, tempFile);
    printf("i: %d, temp2: %s, strlen(temp2): %ld", i, temp2, strlen(temp2));
    //argArray[i++] = temp2;
    poke(argArray[i]);*/
    argArray[i++] = NULL; //End with null terminator.
    return i;
}

/****************************************************************************
*Deallocates the memory assigned to the args.
*
*@params
*-*array[]: Pointer to character arrary storing the command line arguments.
*-len: The length of the args array.
****************************************************************************/
void freeArray(char *array[], int len) {
    for(int i = 0; i < len; i++) {
        free(array[i]);
    }
}

/***************************************************************************
*Executes a command line command with all its arguments.
*-*command[]: The command line command with all its arguments.
***************************************************************************/
void execute(char *file, char *command[]) {
    execvp(file, command);
}

/***************************************************************************
*Prints a separator of 80 chars. Displays the name of the input command.
*-*args[]: The collection of arguments prompted from user.
*-comNum: The index of the command in args to print.
***************************************************************************/
void printDelimiter(char *args[], int comNum) {
    printf("-----CMD %d: %s", comNum + 1, args[comNum]);
    int numChars = strlen(args[comNum]) + 12; //Length of command + preceding chars
    for(int i = numChars; i < 80; i++)
        printf("-");
    printf("\n");
}

/****************************************************************************
*Prints a line of text for debugging purposes.
****************************************************************************/
void poke(const char *text) {
    if(DEBUG) {
        printf("%s\n", text);
    }
}

