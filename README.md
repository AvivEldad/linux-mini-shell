linux simple mini shell

==Description==
The program present a shell. The user insert command of his choice, and the program parses the string command to a process.
(the program does not support cd command).
In the end, the program prints statistic about the commands.
For each command the program also prints the sched file of the process.



==functions==:
five main functions:

1.printPrompt - The function print a prompt line with the user name and current directory


2.numOfWords - The function recive the sentence (string), counts the number of words in it, and return the number.
            input: char sentence[]: the string from the user
            return: int value: number in the sentence
            

3.parseSentence  - The function take a string, and for each word in it, it allocate a place in a string pointer array, that present a command
                the it's length.
             input: char sentence[]: the string from the user
                    char **command: the array that presenting command
             
4.freeMemory - The function free an allocated memory of a given array
                input: char **command: an array with allocated cells 
                       int arraySize: the size of the array to free 
                


5. buildArray - The function build a 2d array, that represent the command from the user
                input: int arraySize: the size of the wanted array.
                       char sentence[]: the string that enter in the array
                return: char ** value: the allocated array     


==Program Files==
shell.c


==How to compile?==

compile: gcc shell.c -o shell
run: ./shell


==Input:==
A string from the user that represent a command(for example "echo hello world")


==Output:==
The program execute the command to the screen (for ex2b its also prints the sched file of the command).
In the end, the program also prints statistic and a sentence in the end:
Num of commands
Total length of all commands
Average length of all commands
See you next time !


