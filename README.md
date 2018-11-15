# Producer Consumer Interaction

## Introduction 
Counting word frequency - We want to count the frequencies of the selected keywords in a document, and the task is to write a multithreaded program to generate the resulting statistics after scanning through the document. We create multiple threads in the process; each thread is working as a worker to search and count the frequency of a keyword.

We are going to structure our solution in the form of producer/consumer interaction, such that the master (main) thread works as the producer that places keywords to a task pool, while the worker threads work as the consumers that obtain a keyword from the task pool and process it.

We are going to implement the control logic of the multithreaded program in the form of bounded- buffer interaction and use semaphores or (mutex locks and condition variables) to synchronize and coordinate between multiple threads.

## Workflow

To apply the bounded-buffer model to this application, we use the master thread to work as a producer and all worker threads act as the consumers. The multithreaded program only creates a fixed number (T) of worker threads regardless of the number of keywords. Producer communicates with all workers (consumers) via a task pool (the bounded-buffer), which is implemented as an array of C strings in this application. The array has N entries; each is for storing a keyword. The parameters T and N are runtime parameters obtained from the command line arguments. We will assume the number of worker threads is between 1 and 15 and the size of the bounded-buffer is between 1 to 10.

The master thread (producer) reads in keywords from the keyword file and places them one by one to the task pool. If the task pool becomes full, i.e., no empty entries, the producer should be blocked waiting for empty entries in the task pool. This process would continue until all the keywords have been placed to the task pool.

The consumer would continuously check whether tasks (i.e., keywords) are available in the task pool. If the task pool is empty, consumers should be blocked waiting for new tasks to arrive. If there is a task in the task pool, one of the waiting consumers will obtain the task and remove it from the task pool.

To have better control on the output, we would like to ask all worker threads not to output the counting results directly to the screen (stdout); instead, they should return the keyword counts to master thread, and master thread will output all results only after it has waited for the termination of all worker threads.

## How to run it ?

First, compile the c file with the following command: 

```bash
gcc thrwordcnt.c -o thrwordcnt
```
Then run the executable with the following arguments:
```bash
./thrwordcnt [number of workers] [number of buffers] [target plaintext file] [keyword file]
```

*Number of Workers* -> Number of child threads we want to engage <br>
*Number of Buffers* -> Size of the Task Pool <br>
*Target Plain Text File* -> The file you want to count the word frequency in <br>
*Keyword File* -> The file in which the words you want to search are stored <br>