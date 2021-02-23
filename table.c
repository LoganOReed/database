/*
*	Data Storage structure and helper functions/constants
*	-Pages store a fixed number of Rows
*	-Rows store serialized id, username, and email
*/

#include <stdlib.h>
#include <stdint.h>
#include "compiler.h"
#include "table.h"

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