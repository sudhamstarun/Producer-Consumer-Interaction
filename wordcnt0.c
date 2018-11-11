/************************************************************
* COMP3230A Principles of Operating Systems
* -----------------------------------------------------------
* Filename: wordcnt0.c
* Author: Anthony Tam
* version: 1.0
* -----------------------------------------------------------
* Description:
* Implement this keyword searching and counting task in C
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN	116 // max len for each word?

//align to 8-byte boundary
struct output {
	char keyword[MAXLEN];
	unsigned int count;
};

char * target; 	//store the filename of keyword file

//pre: input is a C string to be converted to lower case
//post: return the input C string after converting all the characters to lower case
char * lower(char * input){
	unsigned int i;	
	for (i = 0; i < strlen(input); ++i)
		input[i] = tolower(input[i]);
	return input;
}

//pre: the keyword to search for
//post: the frequency of occurrence of the keyword in the file
unsigned int search(char * keyword){
	int count = 0;
	FILE * f;
	char * pos;
	char input[MAXLEN];
	char * word = strdup(keyword);		//clone the keyword	

	lower(word);	//convert the word to lower case
	f = fopen(target, "r");		//open the file stream

    while (fscanf(f, "%s", input) != EOF){  //if not EOF
        lower(input);	//change the input to lower case
        if (strcmp(input, word) == 0)	//perfect match
			count++;
		else {
			pos = strstr(input, word); //search for substring
			if (pos != NULL) { //keyword is a substring of this string
				//if the character before keyword in this input string
				//is an alphabet, skip the checking
				if ((pos - input > 0) && (isalpha(*(pos-1)))) continue; 
				//if the character after keyword in this input string
				//is an alphabet, skip the checking
				if (((pos-input+strlen(word) < strlen(input))) 
					&& (isalpha(*(pos+strlen(word))))) continue;
				//Well, we count this keyword as the characters before 
				//and after the keyword in this input string are non-alphabets
				count++;  
			}
		}
    }
    fclose(f);
	free(word);
	
    return count;
}

int main(int argc, char * argv[]){
	if (argc < 3){
		printf("Usage: ./wordcnt0 [target plaintext file] [keyword file]\n");
		return 0;
	}

	int line, i;
	char word[MAXLEN];
	struct output * result;
	FILE * k;


	//set the target file name
	target = strdup(argv[1]);

	k = fopen(argv[2], "r");
	fscanf(k, "%d", &line);		//read in the number of keywords
	
	//allocate space to store the results
	result = (struct output *) malloc(line * sizeof(struct output));	
	
	//perform the search
	for (i = 0; i < line; ++i) {
		fscanf(k, "%s", word);
		result[i].count = search(word);
		strcpy(result[i].keyword, word);
	}
	fclose(k);
	
	//output the result to stdout
	for (i = 0; i <line; ++i)
		printf("%s : %d\n", result[i].keyword, result[i].count);	
	
	free(target);
	free(result);
	return 0;
}
