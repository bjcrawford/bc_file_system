/**
 * @file bc_fat_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *  This program was written for use in Linux.
*/

#ifndef BC_DIR_OPERATIONS
#define BC_DIR_OPERATIONS

void createDirectoryEntry(FILE **virDrive, size_t dirCluster,
	                     char attr, char *name, char *ext);
size_t getFirstFreeEntry(FILE **virDrive, size_t dirCluster);

#endif