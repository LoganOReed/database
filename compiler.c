/*
*	Compiles user input from cmd line into readable statements for the program
*/

#include "compiler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "inputbuffer.h"
#include "table.h"

MetaCommandResult doMetaCommand(InputBuffer* inputBuffer, Table* table){
	if(strcmp(inputBuffer->buffer, ".exit") == 0){
		closeInputBuffer(inputBuffer);
		freeTable(table);
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

ExecuteResult executeInsert(Statement* statement, Table* table){
	if(table->numRows >= TABLE_MAX_ROWS){
		return EXECUTE_TABLE_FULL;
	}

	Row* rowToInsert = &(statement->rowToInsert);

	serializeRow(rowToInsert, rowSlot(table, table->numRows));
	table->numRows += 1;

	return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement* statement, Table* table){
	Row row;
	for(uint32_t i = 0; i < table->numRows; i++){
		deserializeRow(rowSlot(table, i), &row);
		printRow(&row);
	}
	return EXECUTE_SUCCESS;
}

ExecuteResult executeStatement(Statement* statement, Table* table){
	switch(statement->type){
		case (STATEMENT_INSERT):
			return executeInsert(statement, table);
		case (STATEMENT_SELECT):
			return executeSelect(statement, table);
	}
}


