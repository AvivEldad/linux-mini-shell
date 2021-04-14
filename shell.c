* linux mini shell
 * The program present a shell. The user insert command of his choice, and the program parses the string command to a process.
 * (the program does not support cd command).
 * For each command the program also prints the sched file of the process.
 * In the end, the program prints statistic about the commands.*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include<sys/wait.h>

#define LEN 511
#define TRUE 1
#define FALSE 0


int wordsNum(const char sentence[]);
void parseSentence(const char[], char **command);
void freeMemory(char **command , int arraySize);
void printPrompt();
char** buildArray(int arraySize, const char sentence[]);


int main(){

    char sentence[LEN];
    int arraySize, numOfCommands = 0, sumLen = 0, done = FALSE;
    float average;

    while (done == FALSE ) {
        printPrompt();
        if (fgets(sentence, LEN, stdin) != NULL) {
            numOfCommands++;
            sumLen += (int) strlen(sentence) - 1;
            if (strcmp(sentence, "done\n") != 0) {
                arraySize = wordsNum(sentence);
                if(arraySize != 0){
                    char **command = buildArray(arraySize, sentence);
                    if((strcmp(command[0], "done") == 0) && arraySize == 1){
                        freeMemory(command, arraySize);
                        done = TRUE;
                        break;
                    }
                    else if (strcmp(command[0], "cd") == 0) {
                        printf("command not supported (Yet)\n");
                    }
                    else {
                        pid_t id;
                        id = fork();
                        if (id < 0) {
                            perror("ERR");
                            freeMemory(command, arraySize);
                            exit(1);
                        }
                        else if(id == 0) {//son process
                            pid_t sid;
                            sid = fork();
                            if(sid < 0) {
                                perror("ERR");
                                freeMemory(command, arraySize);
                                exit(1);
                            }
                            else if(sid == 0){//grandson process
                                char buf [20];
                                sprintf(buf,"/proc/%d/sched", getppid());
                                char* catCommand [] = {"cat" ,buf, NULL};
                                if(execvp(catCommand[0], catCommand) == -1){
                                    perror("ERR");
                                    exit(1);
                                }
                            }
                            else{
                                wait(NULL);
                                if(execvp(command[0],command) == -1){
                                    perror("command not found\n");
                                    freeMemory(command, arraySize);
                                    exit(1);
                                }
                            }
                        }
                        else {
                            wait(NULL);
                        }
                    }
                    freeMemory(command, arraySize);
                }
            }
            else{
                done = TRUE;
            }
        }
    }
    average = (float)sumLen / (float)numOfCommands;
    printf("Num of commands: %d\n", numOfCommands);
    printf("Total length of all commands: %d\n", sumLen);
    printf("Average length of all commands: %f\n", average);
    printf("See you next time !\n");
    return  0;
}


//The function print a prompt line with the user name and current directory
void printPrompt(){
    struct passwd *p;
    uid_t uid = getuid();
    char dir[512];
    p = getpwuid(uid);
    if((p != NULL) && (getcwd(dir, sizeof(dir)) != NULL)){
        printf("%s@%s>", p->pw_name, dir);
    }
    else if ((p == NULL) && (getcwd(dir, sizeof(dir)) != NULL)){
        printf("null@%s>",  dir);
    }
    else if ((p != NULL) && (getcwd(dir, sizeof(dir)) == NULL))
        printf("%s@null>",  p->pw_name);
}


//The function count the words in sentence, and return the number
int wordsNum(const char sentence[]) {

    int i, counter = 0;

    for (i = 1; sentence[i] != '\0'; i++) {
        if (sentence[i] == ' ' || sentence[i] == '\n') {
            if (sentence[i - 1] != ' ') {
                counter++;
            }
        }
    }
    return counter;
}


//The function take a string, and for each word in it, it allocate a place in a string pointer array, that present a command
void parseSentence(const char sentence[], char **command){

    char copyWord[LEN];
    int letter = 0, i, j =0;

    for (i = 0; sentence[i] != '\0'; i++) {
        if (sentence[i] != ' ' && sentence[i] != '\n') {
            copyWord[letter] = sentence[i];
            letter++;
        } else if ((sentence[i] == ' ' || sentence[i] == '\n') && letter > 0) {
            copyWord[letter] = '\0';
            command[j] = (char*) malloc(strlen(copyWord) + 1);
            if (command[j] == NULL) {
                fprintf(stderr,"Error! could not allocate memory");
                freeMemory(command, j);
                exit(1);
            }
            strcpy(command[j], copyWord);
            j++;
            letter = 0;
        }
    }
    command[j] = NULL;
}


//The function free an allocated memory
void freeMemory(char **command, int arraySize){
    for(int k = 0; k < arraySize; k++){
        free(command[k]);
    }
    free(command);
}


//The function build a 2d array, that represent the command from the user
char** buildArray(int arraySize, const char sentence[]) {
    char ** command = malloc((arraySize + 1)  * (sizeof(char*)));
    if (command == NULL) {
        fprintf(stderr,"malloc failed");
        exit(1);
    }
    parseSentence(sentence, command);
    return command;
}
