#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputbuffer.h"
#include "compiler.h"

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer){
	if(strcmp(inputBuffer->buffer, ".exit") == 0){
		exit(EXIT_SUCCESS);
	} else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement){
	if (strncmp(inputBuffer->buffer, "insert", 6) == 0){
		statement->type = STATEMENT_INSERT;
		int argsAssigned = sscanf(
			inputBuffer->buffer,
			"insert %d %s %s",
			&(statement->rowToInsert.id),
			statement->rowToInsert.username,
			statement->rowToInsert.email);
		if(argsAssigned < 3){
			return PREPARE_SYNTAX_ERROR;
		}
		return PREPARE_SUCCESS;
	}
	if (strcmp(inputBuffer->buffer, "select") == 0){
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}

void executeStatement(Statement* statement){
	switch(statement->type){
		case (STATEMENT_INSERT):
			printf("This is where there would be an insert...\n");
			break;
		case (STATEMENT_SELECT):
			printf("This is where there would be a select...\n");
			break;
	}
}
