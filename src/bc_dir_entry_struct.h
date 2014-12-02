/**
 * @file bc_dir_entry_struct.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the struct which represents the properties
 *     contained within a directory entry. For a more detailed explanation
 *     of the properties see the documentation in bc_dir_operations.c
 *     
 *     This program was written for use in Linux.
*/

#ifndef BC_DIR_ENTRY_STRUCT
#define BC_DIR_ENTRY_STRUCT

#include "bc_header.h"

typedef struct 
{
	char attr;
	char fileName[FILE_NAME_MAX + 1];
	char fileExt[FILE_EXT_SIZE + 1];
	u_int createDate;
	u_int modifiedDate;
	u_int startCluster;
	u_int fileSize;

} DirEntry;

#endif