/*
*	Data Storage structure and helper functions/constants
*	-Pages store a fixed number of Rows
*	-Rows store serialized id, username, and email
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
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

//Opens db file
//creates pager
//connects file descriptor to pager
//initializes pager
Pager* pagerOpen(const char* filename){
	//creates a file (if none exist) and returns a file descriptor (basically address)
	int fd = open(filename,
		O_RDWR | 		//Read/Write mode
			O_CREAT,	//Create file if it does not exist
		S_IWUSR | 		//User write permission
			S_IRUSR		//User read permission
		);

	//file check
	if(fd == -1){
		printf("Unable to open file.\n");
		exit(EXIT_FAILURE);
	}

	//off_t is for file sizes
	//lseek finds fd for end to find difference
	off_t fileLength = lseek(fd, 0, SEEK_END);

	//construct pager with above values
	Pager* pager = malloc(sizeof(Pager));
	pager->fileDescriptor = fd;
	pager->fileLength = fileLength;

	//initialize pages to NULL
	for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++){
		pager->pages[i] = NULL;
	}

	return pager;
}

//opens connection to database file
Table* dbOpen(const char* filename){
	Pager* pager = pagerOpen(filename);
	uint32_t numRows = pager->fileLength / ROW_SIZE;

	Table* table = malloc(sizeof(Table));
	table->pager = pager;
	table->numRows = numRows;

	return table;
}

//flushes cache to disk
//closes db file
//frees memory for pager and table data structures
void dbClose(Table* table){
	Pager* pager = table->pager;
	uint32_t numFullPages = table->numRows / ROWS_PER_PAGE;

	for(uint32_t i = 0; i < numFullPages; i++){
		if(pager->pages[i] == NULL){
			continue;
		}
		pagerFlush(pager, i, PAGE_SIZE);
		free(pager->pages[i]);				//frees memory from pager pages
		pager->pages[i] = NULL;				//fixes dangling pointer
	}

	//There may be a partial page to write to end of file
	//This wont be needed when we shift over to B-Tree
	uint32_t numAdditionalRows = table->numRows % ROWS_PER_PAGE;
	if(numAdditionalRows > 0){
		uint32_t pageNum = numFullPages;
		if (pager->pages[pageNum] != NULL){
			pagerFlush(pager, pageNum, numAdditionalRows*ROW_SIZE);
			free(pager->pages[pageNum]);
			pager->pages[pageNum] = NULL;
		}
	}

	int result = close(pager->fileDescriptor);
	if(result == -1){
		printf("Error closing db file.\n");
		exit(EXIT_FAILURE);
	}
	for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++){
		void* page = pager->pages[i];
		if(page){
			free(page);
			pager->pages[i] = NULL;
		}
	}
	free(pager);
	free(table);
}

void pagerFlush(Pager* pager, uint32_t pageNum, uint32_t size){
	if(pager->pages[pageNum] == NULL){
		printf("Tried to flush null page\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);
	if(offset == -1){
		printf("Error seeking: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ssize_t bytesWritten = 
		write(pager->fileDescriptor, pager->pages[pageNum], size);

	if(bytesWritten == -1){
		printf("Error writing: %d\n", errno);
		exit(EXIT_FAILURE);
	}
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

//fetches page(returns pointer to it) and handles a cache miss
//We save pages sequentially (Page 0 at 0, Page 1 at offset 4096, etc)
//If requested page lies outside of bounds, we create a blank page by allocating and returning memory
void* getPage(Pager* pager, uint32_t pageNum){
	//check if we're within bounds
	if(pageNum > TABLE_MAX_PAGES){
		printf("Tried to fetch page number out of bounds. %d > %d\n", pageNum, TABLE_MAX_PAGES);
		exit(EXIT_FAILURE);
	}

	if(pager->pages[pageNum] == NULL){
		//Cache miss. Allocate memory and load from file.
		void* page = malloc(PAGE_SIZE);
		uint32_t numPages = pager->fileLength / PAGE_SIZE;

		//Account for the partial page we could have at the end of the file
		if(pager->fileLength % PAGE_SIZE){
			numPages += 1;
		}

		if(pageNum <= numPages){
			lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);		//sets the offset of the file descriptor to be pageNum * PAGE_SIZE
			ssize_t bytesRead = read(pager->fileDescriptor, page, PAGE_SIZE);	//reads PAGE_SIZE number of bytes into page
			if(bytesRead == -1){
				printf("Error reading file: %d\n", errno);
				exit(EXIT_FAILURE);
			}
		}

		pager->pages[pageNum] = page;
	}
	return pager->pages[pageNum];
}

//returns pointer to row at rowNum
void* rowSlot(Table* table, uint32_t rowNum){
	uint32_t pageNum = rowNum / ROWS_PER_PAGE;
	void* page = getPage(table->pager, pageNum);
	uint32_t rowOffset = rowNum % ROWS_PER_PAGE;	//how many rows into the page
	uint32_t byteOffset = rowOffset * ROW_SIZE;		//rowOffset in # of bytes instead of rows
	return page + byteOffset;	//pointer to page shifted by byteOffset
}