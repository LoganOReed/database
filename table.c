/*
*	Data Storage structure and helper functions/constants
*	-Pages store a fixed number of Rows
*	-Rows store serialized id, username, and email
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "table.h"

//constant definitions
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

//constructor
Table* newTable(){
	Table* table = malloc(sizeof(Table));
	table->numRows = 0;
	for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++){
		table->pages[i] = NULL;	//creates flag so we know when we want to create a new page
	}
	return table;
}

//destructor
void freeTable(Table* table){
	for(size_t i = 0; table->pages[i]; i++){	//loops through all pages that aren't null
		free(table->pages[i]);
	}
	free(table);	//deallocates memory
}

void printRow(Row* row){
	printf("(%d, %s, %s)\n", row->id,row->username,row->email);
}

//Functions to Serialize and Deserialize rows to store them in pages.
void serializeRow(Row* source, void* destination){
	//copies 3rd arg number of bytes from 2nd arg to 1st
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserializeRow(void* source, Row* destination){
	//copies 3rd arg number of bytes from 2nd arg to 1st
	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* rowSlot(Table* table, uint32_t rowNum){
	uint32_t pageNum = rowNum / ROWS_PER_PAGE;
	void* page = table->pages[pageNum];
	if(page == NULL){
		//Allocate memory if we are trying to access a page that doesn't exist
		table->pages[pageNum] = malloc(PAGE_SIZE);
		page = table->pages[pageNum];
	}
	uint32_t rowOffset = rowNum % ROWS_PER_PAGE;	//how many rows into the page
	uint32_t byteOffset = rowOffset * ROW_SIZE;		//rowOffset in # of bytes instead of rows
	return page + byteOffset;	//pointer to page shifted by byteOffset
}