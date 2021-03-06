/*
FileName: thrwordcnt.c
Student Name: Tarun Sudhams
Student Number: 3035253876
Development Platform: MACOSX 10.14 with gcc compiler and Visual Studio Code(tested on Ubuntu 16.04) 
Compilation: gcc -pthread thrwordcnt.c -o thrwordcnt (Ubuntu 16.04)
Execution: ./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]
Remarks: 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define MAXIMUM_SIZE 116

/*
***DECLARING DATA STRUCTURES REQUIRED FOR OUR PROGRAM *** 
*/

typedef struct printResults 
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

bool checkIfSharedBufferEmpty(); //checks if the buffer is empty
char * convertToLowerCase(char * input); // converts a given input char * into all lower case char * 
unsigned int keyWordSearch(char * keyword); //implements 
void * workerThreadExecution(void * arg); //implements the workerThread Logic

/*
************************************************************
*/

/*
***LIST OF GLOBAL VARIABLES BEING USED IN THE PROGRAM*** 
*** This includes mutex locks and the condition variables as well ***
*/

char *filename; // store the name of the keyword file
char ** sharedBuffer; // the buffer to store words to searched
struct printResults * pointerforResults;
int resultCounter = 0; 
int lineCounter; // for counting lines while working with test file
char temporaryKeyWordStorage[MAXIMUM_SIZE]; // temporary storage for the keyword
int bufferCounter = 0; //counter for buffer operations
int wordCounter = 0;
int sharedBufferSize; // defines the size of the 
int workerThreads; // number of work threads used in the program

//Condition variables and mutex locks declared below
pthread_mutex_t sharedBufferLock; //buffer mutex lock
pthread_mutex_t resultsPoolLock; //result pool mutex lock
pthread_cond_t sharedBufferNotFull; // buffer not full condition variable
pthread_cond_t sharedBufferNotEmpty; // buffer not empty condition variable
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


/*
************************************************************
*/


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
    int tasksCompletedCounter = 0; // total number of tasks completed the thread has completed
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // 
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    //following the same logic for producer as mentioned in the assignment description
    while(true) 
    {
        printf("Worker(%li) : Start up. Wait for task!\n", (uintptr_t) arg);
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
        printf("Worker(%li) : Search for keyword '%s'\n", (uintptr_t) arg, temporaryKeyWordStorage);
        pointerforResults[resultCounter].counter = keyWordSearch(temporaryKeyWordStorage); 
        strcpy(pointerforResults[resultCounter].currentWord, temporaryKeyWordStorage);
        resultCounter++;
        pthread_cond_signal(&sharedBufferNotFull);
        pthread_mutex_unlock(&sharedBufferLock);
        tasksCompletedCounter++;
    }

    pthread_exit((void*)(uintptr_t)tasksCompletedCounter);
}

int main(int argc, char* argv[])
{
    /*
    *** DECLARING VARIABLES REQUIRED IN INT MAIN AND INITLIZATION OF CONDITION VARIABLES
        AND MUTEX LOCKS***
    */

    FILE *fp;
    int randomIntegerOne;
    char temporaryKeyWord[MAXIMUM_SIZE];

    pthread_mutex_init(&sharedBufferLock, NULL);
    pthread_mutex_init(&resultsPoolLock, NULL);
    pthread_cond_init(&sharedBufferNotEmpty, NULL);
    pthread_cond_init(&sharedBufferNotFull, NULL);

    /*
    ************************************************************
    */

    workerThreads = (int) strtol (argv[1], (char**)NULL, 10);
    sharedBufferSize = (int) strtol (argv[2], (char**)NULL, 10);
    filename = strdup(argv[3]);

    /*
    *** START OF HANDLING COMMANDLINE ARGUMENT ERRORS ***
    */

    if(argc < 5)
    {
        fprintf(stderr, "Usage: ./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]\nz");
        exit(EXIT_FAILURE);
    }

    if(workerThreads < 1 || workerThreads > 15)
    {
        fprintf(stderr, "The number of worker threads must be between 1 to 15\n");
        exit(EXIT_FAILURE);
    }

    if(sharedBufferSize < 1 || sharedBufferSize > 10)
    {
        fprintf(stderr, "The number of buffers in task pool must be between 1 to 10");
        exit(EXIT_FAILURE);
    }

    /*
    ************************************************************
    */


    /*
    *** MAIN LOGIC STARTS HERE ***
    */
       
    fp = fopen(argv[4], "r");
    fscanf(fp, "%d", &lineCounter);
    pointerforResults = (struct printResults *) malloc (lineCounter * sizeof(struct printResults)); // array to maintain the final results
    sharedBuffer = malloc(8*lineCounter * sizeof(char*));

    // initialize the sharedBuffer array
    for(randomIntegerOne = 0; randomIntegerOne < sharedBufferSize; randomIntegerOne++)
    {
        sharedBuffer[randomIntegerOne] = malloc((MAXIMUM_SIZE) * sizeof(char *));
    }


    //setting all the values in sharedBuffer to NULL
    for(randomIntegerOne = 0; randomIntegerOne < sharedBufferSize; randomIntegerOne++)
    {
        sharedBuffer[randomIntegerOne] = NULL;
    }


    //initialize thread array
    pointerToThreads = malloc(workerThreads * sizeof(pthread_t));

    //create threads
    for(randomIntegerOne = 0; randomIntegerOne < workerThreads; randomIntegerOne++)
    {
        pthread_create(&pointerToThreads[randomIntegerOne], NULL, workerThreadExecution, (void*) (uintptr_t) randomIntegerOne);
    }

    //following the same logic for consumer as mentioned in the assignment description
    while(wordCounter < lineCounter) 
    {
        fscanf(fp, "%s", temporaryKeyWord);
        pthread_mutex_lock(&sharedBufferLock);

        while(bufferCounter == sharedBufferSize)
        {
            pthread_cond_wait(&sharedBufferNotFull, &sharedBufferLock);
        }

        sharedBuffer[bufferCounter] = strdup(temporaryKeyWord);
        wordCounter++;
        bufferCounter++;
        pthread_cond_signal(&sharedBufferNotEmpty);
        pthread_mutex_unlock(&sharedBufferLock);
    }

    //following the same logic to terminate the threads running after finishing the tasks
    while(true)
    {
        if(checkIfSharedBufferEmpty() == true)
        {
            for(randomIntegerOne = 0; randomIntegerOne < workerThreads; randomIntegerOne++)
            {
                pthread_mutex_lock(&sharedBufferLock);

                while(bufferCounter == sharedBufferSize)
                {
                    pthread_cond_wait(&sharedBufferNotFull, &sharedBufferLock);
                }

                sharedBuffer[bufferCounter] = "__XX__";
                bufferCounter++;
                pthread_cond_signal(&sharedBufferNotEmpty);
                pthread_mutex_unlock(&sharedBufferLock);
            }
            break;
        }
    }

    int taskPool[workerThreads]; // array to store number of tasks conducted by each thread

    //waits for the threads to terminate and collect the number of tasks they finished
    for(randomIntegerOne = 0; randomIntegerOne < workerThreads; randomIntegerOne++)
    {
        pthread_join(pointerToThreads[randomIntegerOne], (void**) &taskPool[randomIntegerOne]);
    }

    /*
    *** PRINTING OUTPUT RESULTS AS REQUESTED BY THE ASSIGNMENT DESCRIPTION ***

    */
    for(randomIntegerOne = 0; randomIntegerOne < workerThreads; randomIntegerOne++)
    {
        printf("Worker thread %d has terminated and completed %d tasks.\n", randomIntegerOne, taskPool[randomIntegerOne]);
    }

    for(randomIntegerOne = 0; randomIntegerOne < lineCounter; randomIntegerOne++)
    {
        printf("%s: %d\n", pointerforResults[randomIntegerOne].currentWord, pointerforResults[randomIntegerOne].counter);
    }

    /*
    ************************************************************
    */

    //destorying condtion variables and mutex locks

    pthread_mutex_destroy(&sharedBufferLock);
    pthread_mutex_destroy(&resultsPoolLock);
    pthread_cond_destroy(&sharedBufferNotEmpty);
    pthread_cond_destroy(&sharedBufferNotFull);
    free(sharedBuffer);
    free(pointerToThreads);
    fclose(fp);
    free(filename);
    free(pointerforResults);
    return 0;
}
