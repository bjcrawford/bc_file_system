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

void bc_rewind(BC_FILE *file)
{
	file->filePosition = 0;
	file->currentLoc = file->startLoc;
	file->currentClusterAddr = file->startClusterAddr;
}

void freeBC_File(BC_FILE *fp)
{
	if(fp)
	{
		free(fp->fileName);
		free(fp->fileExt);
	}
}