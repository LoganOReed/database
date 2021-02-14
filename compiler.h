#ifndef COMPILER_H
#define COMPILER_H

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputbuffer.h"

typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
	STATEMENT_INSERT,
	STATEMENT_SELECT
} StatementType;

typedef struct{
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
	StatementType type;
	Row rowToInsert;	//only used by insert statement
} Statement;

MetaCommandResult doMetaCommand(InputBuffer*);
PrepareResult prepareStatement(InputBuffer*, Statement*);
void executeStatement(Statement*);

#endif