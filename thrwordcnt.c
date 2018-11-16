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
***DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/

typedef struct printResults //rddata
{
    unsigned int counter; // counter of the keyword
    char currentWord[MAXIMUM_SIZE]; // storing the keyword in concern
} printResults;

/*
************************************************************
*/

/*
***LIST OF FUNCTIONS BEING USED IN THE PROGRAM*** 
*/

bool checkIfSharedBufferEmpty();
char * convertToLowerCase(char * input);
unsigned int keyWordSearch(char * keyword);
void * workerThreadExecution(void * arg);

/*
************************************************************
*/

/*
***LIST OF GLOBAL VARIABLES BEING USED IN THE PROGRAM*** 
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
************************************************************
*/

/*
    ***DEFINING THE FUNCTIONS REQUIRED TO RUN THE PROGRAM***
*/

bool checkIfSharedBufferEmpty()
{
    int randomIntegerOne;

    for(randomIntegerOne = 0; randomIntegerOne < sharedBufferSize; randomIntegerOne++)
    {
        if(sharedBuffer[randomIntegerOne] != NULL)
        {
            return false;
        }
    }

    return true;
}

// ADAPTED FROM wordcnt.c file provided with the assignment question
//pre: input is a C string to be converted to lower case
//post: return the input C string after converting all the characters to lower case
char * convertToLowerCase(char * input)
{
    unsigned int randomIntegerOne;

    for(randomIntegerOne = 0; randomIntegerOne < strlen(input); randomIntegerOne++)
    {
        input[randomIntegerOne] = tolower(input[randomIntegerOne]);
    }

    return input;
}

// ADAPTED FROM wordcnt.c file provided with the assignment question
//pre: the keyword to search for
//post: the frequency of occurrence of the keyword in the file
unsigned int keyWordSearch(char *keyword)
{
    int counter = 0;
    FILE *f;
    char * pos;
    char input[MAXIMUM_SIZE];
    char * word = strdup(keyword); // clone the keyword

    convertToLowerCase(word); // convert the word to lowercase
    f = fopen(filename, "r"); // open the file stream

    while(fscanf(f, "%s", input) != EOF) //if not EOF
    {
        convertToLowerCase(input); // change the input to lower case
        if(strcmp(input, word) == 0) // perfect match
        {
            counter++;
        }

        else
        {
            pos = strstr(input, word); // search for substring 
            if(pos != NULL)//keyword is a substring of this string 
            { // if the charactter before keyword in this input string 
                //is an alphabet, skip the checking 
                if((pos - input > 0) && (isalpha(*(pos-1))))
                {
                    continue;
                }
                //if the character after keyword in this input string
				//is an alphabet, skip the checking
                if(((pos - input+strlen(word) < strlen(input))) && (isalpha(*(pos+strlen(word)))))
                {
                    continue;
                }
                //Well, we count this keyword as the characters before 
				//and after the keyword in this input string are non-alphabets
                counter++;
            }

        }
    }

    fclose(f);
    free(word);

    return counter;
}

void * workerThreadExecution(void *arg)
{
    char charArrayForCheck[MAXIMUM_SIZE]; 
    int tasksCompletedCounter = 0; // total number of tasks completed the thread has completed
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while(1)
    {
        printf("Worker(%d) : Start up. Wait for task!\n");
        pthread_mutex_lock(&sharedBufferLock);

        while(bufferCounter == 0)
        {
            pthread_cond_wait(&sharedBufferNotEmpty,&sharedBufferLock);
        }

        bufferCounter = bufferCounter - 1;
        strcpy(temporaryKeyWordStorage, sharedBuffer[bufferCounter]);

        if(strcmp(temporaryKeyWordStorage, "__XX__") == 0)
        {
            pthread_cond_signal(&sharedBufferNotFull);
            pthread_mutex_unlock(&sharedBufferLock);
            break;
        }

        sharedBuffer[bufferCounter] = NULL;
        printf("Worker(%d) : Search for keyword '%s'\n", (int)arg, temporaryKeyWordStorage);
        pointerforResults[resultCounter].counter = keyWordSearch(temporaryKeyWordStorage);
        strcpy(pointerforResults[resultCounter].currentWord, temporaryKeyWordStorage);
        resultCounter++;
        pthread_cond_signal(&sharedBufferNotFull);
        pthread_mutex_lock(&sharedBufferLock);
        tasksCompletedCounter++;
    }

    pthread_exit((void*) tasksCompletedCounter);
}

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

    if(atoi(argv[1]) < 1 || atoi(argv[1]) > 15)
    {
        fprintf(stderr, "The number of worker threads must be between 1 to 15\n");
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[2]) < 1 || atoi(argv[2]) > 10)
    {
        fprintf(stderr, "The number of buffers in task pool must be between 1 to 10");
        exit(EXIT_FAILURE);
    }

    /*
    END OF HANDLING COMMANDLINE ARGUMENT ERRORS ***
    */


}
