#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "inputbuffer.h"

//defining constant sizes
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
	PREPARE_SUCCESS,
	PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR
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