#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputbuffer.h"



int main(int argc, char* argv[]){
	InputBuffer* inputBuffer = newInputBuffer();
	while(true){
		printPrompt();
		readInput(inputBuffer);

		if(strcmp(inputBuffer->buffer, ".exit") == 0){
			closeInputBuffer(inputBuffer);
			exit(EXIT_SUCCESS);
		}
		else{
			printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
		}
	}
}