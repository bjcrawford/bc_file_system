/**
 * @file bc_dir_entry_struct.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#ifndef BC_DIR_ENTRY_STRUCT
#define BC_DIR_ENTRY_STRUCT

typedef unsigned int u_int;

typedef struct 
{
	char attr;
	char fileName[12];
	char fileExt[3];
	u_int createDate;
	u_int modifiedDate;
	u_int startCluster;
	u_int fileSize;

} DirEntry;

#endif