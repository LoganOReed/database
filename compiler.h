#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "inputbuffer.h"
#include "table.h"

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
	EXECUTE_SUCCESS,
	EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
	STATEMENT_INSERT,
	STATEMENT_SELECT
} StatementType;

typedef struct {
	StatementType type;
	Row rowToInsert;	//only used by insert statement
} Statement;

MetaCommandResult doMetaCommand(InputBuffer*, Table*);
PrepareResult prepareStatement(InputBuffer*, Statement*);
ExecuteResult executeInsert(Statement*, Table*);
ExecuteResult executeSelect(Statement*, Table*);
ExecuteResult executeStatement(Statement*, Table*);

#endif