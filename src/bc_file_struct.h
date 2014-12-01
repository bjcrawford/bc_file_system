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
	u_int used;
	u_int write;
	u_int hidden;
	u_int subDir;
	char *fileName;
	char *fileExt;
	u_int createDate;
	u_int modifyDate;
	u_int filePosition;
	u_int fileSize;
	u_int startClusterAddr;
	u_int startLoc;
	u_int currentClusterAddr;
	u_int currentLoc;
	u_int dirClusterAddr;
	u_int dirEntryAddr;

} BC_FILE;

void bc_rewind(BC_FILE*);
void freeBC_File(BC_FILE*);

#endif