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
 *     the file allocation table clusters on the virtual drive. Each 
 *     file allocation table entry will consist of 4 bytes. This will
 *     allow for 128 entries per cluster. Each file allocation table
 *     entry can hold one of three types of values:
 *     
 *       - The address of the next cluster in the chain
 *       - The end of chain identifier 0xffffffff
 *       - The empty cluster identifier 0x0
 *       
 *     File allocation table cluster addresses (clusterAddr) refer to the 
 *     position of the cluster on disk. The cluster addresses are defined 
 *     as follows:
 *     
 *       -   0: The boot cluster
 *       -   1: The first cluster of the file allocation table
 *       -   2: The second cluster of the file allocation table
 *         ...
 *       -   n: The last cluster of the file allocation table
 *              Note: n = (((drive size / cluster size) * entry size) / cluster size) + 1
 *       - n+1: The root directory cluster
 *       - n+2: The first cluster of the data region
 *         ...
 *       -   x: The last cluster of the data region
 *              Note: x = (drive size / cluster size) - 1
 *  
 *  This program was written for use in Linux.
*/

#include "bc_fat_operations.h"

/**
 * Initializes the file allocation table clusters of a virtual drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void initFATClusters(FILE **virDrive)
{
	int i;
	int n = getNumberOfClustersPerFAT(virDrive);
	
	/* Boot Cluster */
	setFATEntry(virDrive, 0, 0xffffffff);
	/* FAT Clusters */
	for(i = 1; i < n; i++)
		setFATEntry(virDrive, i, i + 1);
	setFATEntry(virDrive, n, 0xffffffff)
	/* Root Dir Cluster */
	setFATEntry(virDrive, getFirstClusterOfRootDir(virDrive), 0xffffffff);
}

/**
 * Returns the value of a file allocation table entry. This will
 * return one of three types of entries:
 *   - The address of the next cluster in the chain
 *   - The end of chain identifier 0xffffffff
 *   - The empty cluster identifier 0x0
 *
 * @param  virDrive    A pointer to the file pointer of the virtual drive
 * @param  clusterAddr The cluster address of the entry to check
 * @return             The value of the entry
 */
size_t getFATEntry(FILE **virDrive, size_t clusterAddr)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += clusterAddr * 4;

	return readNum(virDrive, loc, 4);
}
/**
 * Returns the offset (in bytes) from the beginning of the virtual drive
 * to the start of the first file allocation table cluster.
 *
 * @param  virDrive A pointer to the file pointer of the virtual drive
 * @return          The offset of first file allocation table cluster in bytes
 */
size_t getFATStartLocation(FILE **virDrive) 
{
	size_t loc = 0;
	/* Skip over boot cluster */
	loc += getBytesPerCluster(virDrive) * getNumberOfReservedClusters(virDrive);

	return loc;
}

/**
 * Sets the value of a file allocation table entry. The value must
 * be one of the following types:
 *   - The cluster address of the next cluster in the chain
 *   - The end of chain identifier 0xffffffff
 *   - The empty cluster identifier 0x0
 *
 * @param virDrive    A pointer to the file pointer of the virtual drive
 * @param clusterAddr The address of the entry to be modified
 * @param value       The value for the entry
 */
void setFATEntry(FILE **virDrive, size_t clusterAddr, size_t value)
{
	size_t loc = getFATStartLocation(virDrive);
	loc += clusterAddr * 4;
	writeNum(virDrive, loc, 4, value);
}

/**
 * Adds a cluster to the end of a cluster chain. The cluster address 
 * that is passed in must be the ending cluster of the chain.
 *
 * @param  virDrive    A pointer to the file pointer of the virtual drive
 * @param  clusterAddr The cluster address of the entry that is being extended
 * @return             The new end of chain cluster address
 */
size_t addClusterToChain(FILE **virDrive, size_t clusterAddr)
{
	size_t next = getNextFreeCluster(virDrive);
	setFATEntry(virDrive, clusterAddr, next);
	setFATEntry(virDrive, next, 0xffffffff);
	findAndSetNextFreeCluster(virDrive);

	return next;
}

/**
 * Locates the next free cluster using the file allocation table and
 * stores the address in the boot cluster of the drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void findAndSetNextFreeCluster(FILE **virDrive)
{
	size_t clusterAddr = getFirstClusterOfRootDir(virDrive) + 1;
	size_t entriesPerCluster = getBytesPerCluster(virDrive) / 4;
	size_t maxEntries = getNumberOfClustersPerFAT(virDrive) * entriesPerCluster; 
	while(getFATEntry(virDrive, clusterAddr) != 0x0)
	{
		if(clusterAddr < maxEntries)
		{
			/* FAT is full, handle error */
		}
		clusterAddr++;
	} 
	setNextFreeCluster(virDrive, clusterAddr);
}