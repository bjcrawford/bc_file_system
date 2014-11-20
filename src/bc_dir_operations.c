/**
 * @file bc_dir_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     The file holds all of the operations which can be performed on
 *     the directory sectors of the virtual drive. Each directory 
 *     entry will consist of 32 bytes. This will allow for 16 entries
 *     per cluster. The layout of the data contained in each directory 
 *     entry is as follows:
 *
 *        (bytes) | value 
 *       ---------------------------------------------------------------
 *        (0)     | A byte holding the attributes of the file/folder:
 *                    Bit 0: 0 for unused entry, 1 for used entry
 *                    Bit 1: 0 for R/O, 1 for R/W
 *                    Bit 2: 1 for hidden file/folder
 *                    Bit 3: 1 for system file
 *                    Bit 4: 1 for subdirectory
 *                    The remaining bits are unused
 *        (1-17)  | The file/folder name (name: 14, extension: 3)
 *        (18-21) | Creation date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (22-25) | Last modified date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (26-27) | First cluster of file (empty file: 0)
 *        (28-31) | The file size in bytes
 *  
 *  This program was written for use in Linux.
*/

#include "bc_dir_operations.h"

/**
 * Creates a directory entry in the given directory cluster
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The starting cluster of the directory
 * @param  attr       The attributes of the file/folder (one byte)
 *                      Bit 0: 0 for unused entry, 1 for used entry
 *                      Bit 1: 0 for R/O, 1 for R/W
 *                      Bit 2: 1 for hidden file/folder
 *                      Bit 3: 1 for system file
 *                      Bit 4: 1 for subdirectory
 *                      The remaining bits are unused
 * @param  name       The name of the file/folder (maximum 14 characters)
 * @param  ext        The extension of the file (maximum 3 characters)          
 */
void createDirectoryEntry(FILE **virDrive, size_t dirCluster,
	                     char attr, char *name, char *ext)
{

}
/**
 * Returns the starting location of the first free directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The starting cluster of the directory
 * @return            The starting location of the first free directory entry
 */
size_t getFirstFreeEntry(FILE **virDrive, size_t dirCluster)
{
	int found = 0;
	size_t loc = 0;
	size_t dataStartLoc;
	size_t entry = 0;
	size_t bytesPerSector = getBytesPerSector(virDrive);

	/* Skip over boot sector */
	loc += bytesPerSector;
	/* Skip over FAT */
	loc += bytesPerSector * getNumberOfSectorsPerFAT(virDrive);
	/* Record data start location */
	dataStartLoc = loc;
	/* Skip to directory cluster */
	loc += bytesPerSector * dirCluster;

	while(!found)
	{
		if(entry > 15)
		{
			size_t nextCluster = getFATEntry(virDrive, dirCluster);
			if(nextCluster != 0xffffffff && nextCluster != 0)
			{
				loc = dataStartLoc + nextCluster * bytesPerSector;
			}
			else if(nextCluster == 0xffffffff)
			{
				/* Add cluster to directory chain */
			}
			else
			{
				/* Something is wrong */
			}
		}
	}

}

