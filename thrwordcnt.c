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
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define MAXIMUM_SIZE 200


/*
*** DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/

typedef struct printResults //rddata
{
    unsigned int counter; // counter of the keyword
    char currentWord[MAXIMUM_SIZE]; // storing the keyword in concern
} printResults;

/*
*** END OF DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/

/*
*** START OF LIST OF FUNCTIONS BEING USED IN THE PROGRAM*** 
*/

char * convertToLowerCase*(char * input);
unsigned int keyWordSearch(char * keyword);





/*
*** END OF LIST OF FUNCTIONS BEING USED IN THE PROGRAM*** 
*/


/*
*** START OF LIST OF GLOBAL VARIABLES BEING USED IN THE PROGRAM*** 
*** This includes mutex locks and the condition variables as well ***
*/

char *filename; // store the name of the keyword file
char ** sharedBuffer;
struct printResults  * pointerIntermediatry;
struct printResults * pointerforResults;
bool flagForTermination;
int resultCounter = 0;
int lineCounter;
char temporaryKeyWordStorage[MAXIMUM_SIZE];
int bufferCounter = 0;
int wordCounter = 0;
int sharedBufferSize;
int workerThreads;

//Condition variables and mutex locks declared below
pthread_mutex_t sharedBufferLock;
pthread_mutex_t resultsPoolLock;
pthread_cond_t sharedBufferNotFull;
pthread_cond_t sharedBufferNotEmpty;
pthread_t *pointerToThreads;
/*
*** END OF LIST OF GLOBAL VARIABLES BEING USED IN THE PROGRAM*** 
*/





































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
