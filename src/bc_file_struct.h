/**
 * @file bc_file_struct.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
*/

#ifndef BC_FILE_STRUCT
#define BC_FILE_STRUCT

#include <stdlib.h>

typedef struct
{
	size_t used;
	size_t write;
	size_t hidden;
	size_t subDir;
	char *fileName;
	char *fileExt;
	size_t createDate;
	size_t modifyDate;
	size_t filePosition;
	size_t fileSize;
	size_t startClusterAddr;
	size_t startLoc;
	size_t currentClusterAddr;
	size_t currentLoc;
	size_t dirClusterAddr;
	size_t dirEntryAddr;

} BC_FILE;

BC_FILE *allocateBC_File();
void bc_rewind(BC_FILE*);

#endif