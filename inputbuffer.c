#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputbuffer.h"

//basically constructor for InputBuffer
InputBuffer* newInputBuffer() {
	//allocates memory for input buffer
	InputBuffer* inputBuffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	inputBuffer->buffer = NULL;
	inputBuffer->bufferLength = 0;
	inputBuffer->inputLength = 0;

	return inputBuffer;
}

void printPrompt() { printf("db > "); }

void readInput(InputBuffer* inputBuffer){
	ssize_t bytesRead = getline(&(inputBuffer->buffer), &(inputBuffer->bufferLength), stdin);

	if(bytesRead <= 0){
		printf("Error reading input. \n");
		exit(EXIT_FAILURE);
	}

	//Ignore Trailing New Line
	inputBuffer->inputLength = bytesRead - 1;
	inputBuffer->buffer[bytesRead - 1] = 0;
}

void closeInputBuffer(InputBuffer* inputBuffer){
	//deallocate memory
	free(inputBuffer->buffer);
	free(inputBuffer);
}