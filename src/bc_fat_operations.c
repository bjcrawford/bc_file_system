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
u_int *initFATClusters()
{
	int i;
	int n = bootRecord->clustersPerFat;
	u_int *fat = (u_int*) calloc(sizeof(u_int), bootRecord->clustersOnDrive);
	
	/* Boot Cluster */
	fat[0] = 0xffffffff;
	/* FAT Clusters */
	for(i = 1; i < n; i++)
		fat[i] = i + 1;
	fat[n] = 0xffffffff;
	/* Root Dir Cluster */
	fat[bootRecord->rootDirStart] = 0xffffffff;

	return fat;
}

void writeFAT()
{
	u_int loc = bootRecord->bytesPerCluster * bootRecord->reservedClusters;
	fseek(virDrive, loc, SEEK_SET);
	fwrite(fileAllocTable, sizeof(u_int), bootRecord->clustersOnDrive, virDrive);
}

void readFAT()
{
	u_int loc = bootRecord->bytesPerCluster * bootRecord->reservedClusters;
	fseek(virDrive, loc, SEEK_SET);
	fread(fileAllocTable, sizeof(u_int), bootRecord->clustersOnDrive, virDrive);
}

/**
 * Adds a cluster to the end of a cluster chain. The cluster address 
 * that is passed in must be the ending cluster of the chain.
 *
 * @param  clusterAddr The cluster address of the entry that is being extended
 * @return             The new end of chain cluster address
 */
u_int addClusterToChain(u_int clusterAddr)
{
	u_int next = bootRecord->nextFreeCluster;
	fileAllocTable[clusterAddr] = next;
	fileAllocTable[next] = 0xffffffff;
	findAndSetNextFreeCluster();
	bootRecord->freeClusters--;

	return next;
}

/**
 * Locates the next free cluster using the file allocation table and
 * stores the address in the boot cluster of the drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void findAndSetNextFreeCluster()
{
	u_int clusterAddr = bootRecord->rootDirStart + 1;
	u_int entriesPerCluster = bootRecord->bytesPerCluster / 4;
	u_int maxEntries = bootRecord->clustersPerFat * entriesPerCluster; 
	while(fileAllocTable[clusterAddr] != 0x0)
	{
		if(clusterAddr >= maxEntries)
		{
			/* FAT is full, handle error */
			fprintf(stderr, "FAT is full\n");
		}
		clusterAddr++;
	} 
	bootRecord->nextFreeCluster = clusterAddr;
}