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

//reads input using strtok instead of sscanf to fix mem bugs
PrepareResult prepareInsert(InputBuffer* inputBuffer, Statement* statement){
	statement->type = STATEMENT_INSERT;

	//This reads input into mem without having to worry about sscanf
	char* keyword = strtok(input_buffer->buffer, " ");
	//NULL after first call because that tells the function to continue at last delimiter
	char* idString = strtok(NULL, " ");
	char* username = strtok(NULL, " ");
	char* email = strtok(NULL, " ");

	if(idString == NULL || username == NULL || email == NULL){
		return PREPARE_SYNTAX_ERROR;
	}

	int id = atoi(idString);

	//check id
	if(id < 0){
		return PREPARE_NEGATIVE_ID;
	}

	//check bounds
	if(strlen(username) > COLUMN_USERNAME_SIZE){
		return PREPARE_STRING_TOO_LONG;
	}
	if(strlen(email) > COLUMN_EMAIL_SIZE){
		return PREPARE_STRING_TOO_LONG;
	}

	//store temp var into rowToInsert
	statement->rowToInsert.id = id;
	strcpy(statement->rowToInsert.username, username);	//I have no idea why we don't just use assignment
	strcpy(statement->rowToInsert.email, email);

	return PREPARE_SUCCESS
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement){
	if (strncmp(inputBuffer->buffer, "insert", 6) == 0){
		return prepareInsert(inputBuffer, statement);
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


