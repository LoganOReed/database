#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdint.h>
#include "compiler.h"

//shorthand for defining size of data types
#define sizeOfAttribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//fixed size of a page
#define TABLE_MAX_PAGES 100


//compact representation of a row
const uint32_t ID_SIZE = sizeOfAttribute(Row, id);
const uint32_t USERNAME_SIZE = sizeOfAttribute(Row, username);
const uint32_t EMAIL_SIZE = sizeOfAttribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

//constant values of pages
const uint32_t PAGE_SIZE = 4096;	//convention for size of pages of operating systems
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct{
	uint32_t numRows;
	void* pages[TABLE_MAX_PAGES];	//array of pointers to pages
} Table;

void serializeRow(Row* source, void* destination);
void deserializeRow(void* source, Row* destination);

//where to read/write memory for a particular row
void* rowSlot(Table* table, uint32_t rowNum);

#endif