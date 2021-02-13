#ifndef INPUT_BUFFER
#define INPUT_BUFFER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* buffer;
	size_t bufferLength;
	ssize_t inputLength;
} InputBuffer;

InputBuffer* newInputBuffer();
void printPrompt();
void readInput(InputBuffer*);
void closeInputBuffer(InputBuffer*);

#endif