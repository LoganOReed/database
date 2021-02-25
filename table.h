#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//fixed size of a page
#define TABLE_MAX_PAGES 100

//defining constant sizes
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct{
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE+1];	//Additional space deals with null terminator
	char email[COLUMN_EMAIL_SIZE+1];
} Row;

//macro for defining size of data types
#define sizeOfAttribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//compact representation of a row
const uint32_t ID_SIZE;
const uint32_t USERNAME_SIZE;
const uint32_t EMAIL_SIZE;
const uint32_t ID_OFFSET;
const uint32_t USERNAME_OFFSET;
const uint32_t EMAIL_OFFSET;
const uint32_t ROW_SIZE;

//constant values of pages
const uint32_t PAGE_SIZE;	//convention for size of pages of operating systems
const uint32_t ROWS_PER_PAGE;
const uint32_t TABLE_MAX_ROWS;

typedef struct{
	uint32_t numRows;
	void* pages[TABLE_MAX_PAGES];	//array of pointers to pages
} Table;


Table* newTable();			//constructor
void freeTable(Table*);		//destructor

void serializeRow(Row* source, void* destination);
void deserializeRow(void* source, Row* destination);

//where to read/write memory for a particular row
void* rowSlot(Table* table, uint32_t rowNum);

//prints the row
void printRow(Row*);

#endif