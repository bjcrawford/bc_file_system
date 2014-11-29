/**
 * @file bc_file_struct.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
*/

#include "bc_file_struct.h"

BC_FILE *allocateBC_File()
{
	BC_FILE *fp = malloc(sizeof(*fp));
	if(fp)
	{
		fp->used = (size_t) calloc(sizeof(size_t), 1);
		fp->write = (size_t) calloc(sizeof(size_t), 1);
		fp->hidden = (size_t) calloc(sizeof(size_t), 1);
		fp->subDir = (size_t) calloc(sizeof(size_t), 1);
		fp->fileName = calloc(sizeof(char*), 13);
		fp->fileExt = calloc(sizeof(char*), 4);
		fp->createDate = (size_t) calloc(sizeof(size_t), 1);
		fp->modifyDate = (size_t) calloc(sizeof(size_t), 1);
		fp->filePosition = (size_t) calloc(sizeof(size_t), 1);
		fp->fileSize = (size_t) calloc(sizeof(size_t), 1);
		fp->startClusterAddr = (size_t) calloc(sizeof(size_t), 1);
		fp->startLoc = (size_t) calloc(sizeof(size_t), 1);
		fp->currentClusterAddr = (size_t) calloc(sizeof(size_t), 1);
		fp->currentLoc = (size_t) calloc(sizeof(size_t), 1);
		fp->dirClusterAddr = (size_t) calloc(sizeof(size_t), 1);
		fp->dirEntryAddr = (size_t) calloc(sizeof(size_t), 1);
	}

	return fp;
}

void bc_rewind(BC_FILE *file)
{
	file->filePosition = 0;
	file->currentLoc = file->startLoc;
	file->currentClusterAddr = file->startClusterAddr;
}