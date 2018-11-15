/*
FileName: thrwordcnt.c
Student Name: Tarun Sudhams
Student Number: 3035253876
Development Platform: MACOSX 10.14 with gcc compiler and Visual Studio Code(tested on Ubuntu 16.04) 
Compilation: gcc thrwordcnt.c -o thrwordcnt (Fix this later)
Execution: ./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]
Remarks: 
*/

/*
REDUCERS THREADS- Number of worker threads
MAPPER THREADS - Will always be 1 in our case as we do not need multiple mapper threads (Need to figure a way to always keep the mapper threads to 1 or see if it is different?)
This means that Mapper Pool will be of the same size as the reducer pool
Number of Buffers or Pool Size will remain the same as from the input argument
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <assert.h>

#define INC_BUFF_SIZE 20 // think what to do with this one

/*
*** DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/

typedef struct readDataFromFile  //rddata
{
    long start; // indicating start of the file
    long end; //indicating end of the file
    char *fileName; //name of the file 
    struct pairBuffer *pointerToPairBuffer; // pointer to the pair buffer
} readDataFromFile;

typedef struct wordBuffer //varbuf
{
    int size; // size of the word buffer
    int count;  // number of words in the buffer
    struct pairDirectory *pointerToDirectory; // directory of pairs       
} wordBuffer;

typedef struct pairBuffer //buffer
{
    int count; // number of entries in the pairBuffer
    int maxSize; // the maximum size of the pairBuffer
    struct wordPair *pointerToDirectory;
    pthread mutex_t mutex;
} pairBuffer;

typedef struct wordPair //pair
{
    char wordName[16];
    int count;
} wordPair;

typedef struct sharedBuffer
{
    struct pairBuffer *buffer1;
    struct wordBuffer *buffer2;
} sharedBuffer;


/*
*** END OF DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/


/*
*** START OF LIST OF FUNCTIONS BEING USED IN THE PROGRAM*** 
*/


/*
*** END OF LIST OF FUNCTIONS BEING USED IN THE PROGRAM*** 
*/
















/*
 *** INITIALIZING GLOBAL VARIABLES REQUIRED TO RUN THE PROGRAM ***
 */

int 



int main(int argc, char* argv[])
{

    /*
    *** START OF HANDLING COMMANDLINE ARGUMENT ERRORS
    */

    if(argc != 4)
    {
        fprintf(stderr, "Usage: ./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]\nz");
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[1] < 1 || aoti(argv[1] > 15)))
    {
        fprintf(stderr, "The number of worker threads must be between 1 to 15\n");
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[2] < 1 || argv[2] > 10))
    {
        fprintf(stderr, "The number of buffers in task pool must be between 1 to 10");
        exit(EXIT_FAILURE);
    }

    /*
    END OF HANDLING COMMANDLINE ARGUMENT ERRORS ***
    */


}
