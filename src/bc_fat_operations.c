/**
 * @file bc_fat_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include "bc_fat_operations.h"

size_t getFATEntry(FILE **virDrive, size_t entryNum)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += entryNum * 4;

	return readNum(virDrive, loc, 4);
}

size_t getFATStartLocation(FILE **virDrive) 
{
	size_t loc = 0;
	/* Skip over boot sector */
	loc += getBytesPerSector(virDrive);

	return loc;
}

void setFATEntry(FILE **virDrive, size_t entryNum, size_t value)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += entryNum * 4;
	writeNum(virDrive, loc, 4, value);
}

size_t addClusterToChain(FILE **virDrive, size_t entryNum)
{
	size_t next = getNextFreeCluster(virDrive)
	setFATEntry(virDrive, entryNum, next);
	setFATEntry(virDrive, next, 0xffffffff);
	findAndSetNextFreeCluster(virDrive);
}

void findAndSetNextFreeCluster(FILE **virDrive)
{
	size_t entryNum = 0;
	size_t entriesPerSector = getBytesPerSector(virDrive) / 4;
	size_t maxEntries = getNumberOfSectorsPerFAT(virDrive) * entriesPerSector; 
	while(getFATEntry(virDrive, entryNum) != 0x0)
	{
		if(entryNum < entriesPerSector)
		{
			/* FAT is full, handle error */
		}
		entryNum++;
	} 
	setNextFreeCluster(virDrive, entryNum);
}