/*
*	Driver for the cmd line
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputbuffer.h"
#include "compiler.h"

//https://cstack.github.io/db_tutorial/
//this is the website I am using as a guide


int main(int argc, char* argv[]){
	Table* table = newTable();
	InputBuffer* inputBuffer = newInputBuffer();
	while(true){
		printPrompt();
		readInput(inputBuffer);

		if(inputBuffer->buffer[0] == '.'){
			switch(doMetaCommand(inputBuffer, table)){
				case (META_COMMAND_SUCCESS):
					continue;
				case (META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s'\n", inputBuffer->buffer);
					continue;
			}
		}

		Statement statement;
		switch (prepareStatement(inputBuffer, &statement)){
			case (PREPARE_SUCCESS):
				break;
			case (PREPARE_SYNTAX_ERROR):
				printf("Syntax error. Could not parse statement.\n");
				continue;
			case (PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword at start of '%s'.\n", 
					inputBuffer->buffer);
				continue;
		}
		switch (executeStatement(&statement, table)){
			case (EXECUTE_SUCCESS):
				printf("Executed.\n");
				break;
			case (EXECUTE_TABLE_FULL):
				printf("Error: Table Full.\n");
				break;
		}
	}
}