/**
 * @file bc_fat_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file holds all of the operations that can be performed on
 *     the file allocation table sectors on the virtual drive. Each 
 *     file allocation table entry will consist of 4 bytes. This will
 *     allow for 128 entries per sector. Each file allocation table
 *     entry can hold one of three types of values:
 *     
 *       - The address of the next cluster in the chain
 *       - The end of chain identifier 0xfffffff
 *       - The empty cluster identifier 0x0
 *       
 *     File allocation table entry addresses refer to the cluster 
 *     number of the cluster the entry represents. The cluster numbers
 *     are defined as follows:
 *     
 *       -   0: The boot sector/cluster
 *       -   1: The first cluster of the file allocation table
 *       -   2: The second cluster of the file allocation table
 *         ...
 *       -   n: The last cluster of the file allocation table
 *              Note: n = (((drive size / sector size) * entry size) / sector size) + 1
 *       - n+1: The root directory sector
 *       - n+2: The first sector of the data region
 *         ...
 *       -   x: The last sector of the data region
 *              Note: x = (drive size / sector size) - 1
 *  
 *  This program was written for use in Linux.
*/

#include "bc_fat_operations.h"

/**
 * Initializes the file allocation table sectors of a virtual drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void initFATSectors(FILE **virDrive)
{
	setFATEntry(virDrive, getFirstClusterOfRootDir(virDrive), 0xffffffff);
}

/**
 * Returns the value of the given file allocation table entry. This will
 * return one of three types of entries:
 *   - The address of the next cluster in the chain
 *   - The end of chain identifier 0xfffffff
 *   - The empty cluster identifier 0x0
 *
 * @param  virDrive  A pointer to the file pointer of the virtual drive
 * @param  entryAddr The address of the entry to check
 * @return           The value of the entry
 */
size_t getFATEntry(FILE **virDrive, size_t entryAddr)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += (entryAddr - getFirstClusterOfRootDir(virDrive)) * 4;

	return readNum(virDrive, loc, 4);
}
/**
 * Returns the offset (in bytes) from the beginning of the virtual drive
 * to the start of the first file allocation table sector.
 *
 * @param  virDrive A pointer to the file pointer of the virtual drive
 * @return          The offset of first file allocation table sector in bytes
 */
size_t getFATStartLocation(FILE **virDrive) 
{
	size_t loc = 0;
	/* Skip over boot sector */
	loc += getBytesPerSector(virDrive) * getNumberOfReservedSectors(virDrive);

	return loc;
}

/**
 * Sets the value of a given file allocation table entry. The value must
 * be one of the following types:
 *   - The address of the next cluster in the chain
 *   - The end of chain identifier 0xffffffff
 *   - The empty cluster identifier 0x0
 *
 * @param virDrive  A pointer to the file pointer of the virtual drive
 * @param entryAddr The address of the entry to be modified
 * @param value     The value for the entry
 */
void setFATEntry(FILE **virDrive, size_t entryAddr, size_t value)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += (entryAddr - getFirstClusterOfRootDir(virDrive)) * 4;
	writeNum(virDrive, loc, 4, value);
}

/**
 * Adds a cluster to the end of the given cluster chain. The cluster 
 * entry address that is passed in must be the ending cluster of the 
 * chain.
 *
 * @param  virDrive  A pointer to the file pointer of the virtual drive
 * @param  entryAddr The address of the file allocation table entry 
 *                   that is being extended
 * @return           The new end of chain cluster entry address
 */
size_t addClusterToChain(FILE **virDrive, size_t entryAddr)
{
	size_t next = getNextFreeCluster(virDrive);
	setFATEntry(virDrive, entryAddr, next);
	setFATEntry(virDrive, next, 0xffffffff);
	findAndSetNextFreeCluster(virDrive);

	return next;
}

/**
 * Locates the next free cluster using the file allocation table and
 * stores the address in the boot sector of the drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void findAndSetNextFreeCluster(FILE **virDrive)
{
	size_t entryAddr = getFirstClusterOfRootDir(virDrive) + 1;
	size_t entriesPerSector = getBytesPerSector(virDrive) / 4;
	size_t maxEntries = getNumberOfSectorsPerFAT(virDrive) * entriesPerSector; 
	while(getFATEntry(virDrive, entryAddr) != 0x0)
	{
		if(entryAddr < maxEntries)
		{
			/* FAT is full, handle error */
		}
		entryAddr++;
	} 
	setNextFreeCluster(virDrive, entryAddr);
}