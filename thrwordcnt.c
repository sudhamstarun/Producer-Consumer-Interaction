/*
FileName: thrwordcnt.c
Student Name: Tarun Sudhams
Student Number: 3035253876
Development Platform: MACOSX 10.14 with gcc compiler and Visual Studio Code(tested on Ubuntu 16.04) 
Compilation: gcc thrwordcnt.c -o thrwordcnt (Fix this later)
Execution: ./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]
Remarks: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

typedef node // creating a singly-linked list in order to store all the target keywords and navigate through them
{
    char *keyword;
    struct node *nextKeyWord;
} Node;

typedef
{
    
}

/*
 *** INITIALIZING GLOBAL VARIABLES REQUIRED TO RUN THE PROGRAM ***
 */

