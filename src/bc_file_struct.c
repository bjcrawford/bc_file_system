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
 *     This file contains an implementation of a custom file pointer. The 
 *     properties contained within this struct hold all of the properties 
 *     of the file's directory entry and additionally includes some properties
 *     for keeping a record of the position of the pointer within the file.
 *     The record keeping properties are described below:
 *     
 *        - filePosition: holds the offset from the beginning of the file
 *                        to the position of the file's pointer.
 *                        
 *        - startClusterAddr: holds the address of the file's starting 
 *                            cluster
 *        
 *        - startLoc: holds the offset from the beginning of the drive
 *                    to the first location of the starting cluster of the 
 *                    file
 *
 *        - currentClusterAddr: holds the address of the cluster where the
 *                              file's pointer is currently located
 *
 *        - currentLoc: holds the offset from the beginning of the drive
 *                      to the location of the file pointer
 *
 *        - dirClusterAddr: holds the address of the directory cluster
 *                          in which the file's directory entry is 
 *                          located
 *
 *        - dirEntryAddr: holds the address of the file's directory entry 
 *                        within the directory cluster in which the file's
 *                        directory entry is located 
 *     
*/

#include "bc_file_struct.h"

void rewindBC_File(BC_FILE *file)
{
	file->filePosition = 0;
	file->currentLoc = file->startLoc;
	file->currentClusterAddr = file->startClusterAddr;
}

void destroyBC_File(BC_FILE *file)
{
	if(file)
	{	
		free(file);
		file = NULL;
	}
}
