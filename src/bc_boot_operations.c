/**
 * @file bc_boot_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file holds all of the operations which can be performed on 
 *     the boot cluster of the virtual drive. The boot cluster will have 
 *     a size of 512 bytes. The layout of the data contained in the 
 *     boot cluster is as follows:
 *     
 *        (bytes) | value 
 *       ---------------------------------------------------------------
 *        (0)     | A byte designating the virtual drive as initialized
 *        (1-20)  | A label for the virtual drive
 *        (21-22) | The number of bytes per cluster
 *        (23-24) | The number of reserved clusters
 *        (25-28) | The number of clusters on the virtual drive
 *        (29-32) | The number of clusters per file allocation table
 *        (33-36) | The first cluster of the root directory
 *        (37-40) | The number of free clusters
 *        (41-44) | The next free cluster
 *        (45-48) | The size of the virtual drive in bytes
 *
 * 
 *  This program was written for use in Linux.
*/

#include "bc_boot_operations.h"

/**
 * Initilizes the boot cluster of a virtual drive. The virtual drive should
 * be formatted prior to using this function.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] driveLabel A string holding the label to associate with the
 *                       drive. Must be 20 characters or fewer.
*/
void initBootCluster(FILE **virDrive, char *driveLabel)
{
	rewind(*virDrive);
	/* Rewind can throw an error under certain conditions */

	size_t driveSize = 0;
	while(fgetc(*virDrive) != EOF)
		driveSize++;

	rewind(*virDrive);
	setDriveLabel(virDrive, driveLabel);
	setBytesPerCluster(virDrive, 512);
	setNumberOfReservedClusters(virDrive, 1);
	setNumberOfClustersOnDrive(virDrive, driveSize / 512);
	setNumberOfClustersPerFAT(virDrive, ceil(((( (double) driveSize / 512) * 4) / 512)));
	setFirstClusterOfRootDir(virDrive, ceil(((( (double) driveSize / 512) * 4) / 512)) + 1);
	setNumberOfFreeClusters(virDrive, (driveSize / 512) - (ceil(((( (double) driveSize / 512) * 4) / 512)) + 2));
	setNextFreeCluster(virDrive, ceil(((( (double) driveSize / 512) * 4) / 512)) + 2);
	setSizeOfDrive(virDrive, driveSize);
	setInitialized(virDrive, 1);

	rewind(*virDrive);
}

/*
 * Returns the initialized value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             1 if drive is initialized, otherwise 0
*/
size_t getInitialized(FILE **virDrive)
{
	return readNum(virDrive, 0, 1);
}

/*
 * Returns the drive label from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             A string containing the drive label
*/
char *getDriveLabel(FILE **virDrive)
{
	return readStr(virDrive, 1, 20);
}

/*
 * Returns the bytes per cluster value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The bytes per cluster
*/
size_t getBytesPerCluster(FILE **virDrive)
{
	return readNum(virDrive, 21, 2);
}

/*
 * Returns the number of reserved clusters value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of reserved clusters
*/
size_t getNumberOfReservedClusters(FILE **virDrive)
{
	return readNum(virDrive, 23, 2);
}

/*
 * Returns the number of clusters on drive value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of clusters
*/
size_t getNumberOfClustersOnDrive(FILE **virDrive)
{
	return readNum(virDrive, 25, 4);
}

/*
 * Returns the number of clusters per FAT value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of clusters per FAT
*/
size_t getNumberOfClustersPerFAT(FILE **virDrive)
{
	return readNum(virDrive, 29, 4);
}

/*
 * Returns the first cluster of root root directory value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The first cluster of the root directory
*/
size_t getFirstClusterOfRootDir(FILE **virDrive)
{
	return readNum(virDrive, 33, 4);
}

/*
 * Returns the number of free clusters value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of free clusters on the drive
*/
size_t getNumberOfFreeClusters(FILE **virDrive)
{
	return readNum(virDrive, 37, 4);
}

/*
 * Returns the next free cluster value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The next free cluster on the drive
*/
size_t getNextFreeCluster(FILE **virDrive)
{
	return readNum(virDrive, 41, 4);
}

/*
 * Returns the size of drive value from the boot cluster of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The size of the drive in bytes
*/
size_t getSizeOfDrive(FILE **virDrive)
{
	return readNum(virDrive, 45, 4);
}

/*
 * Sets the initialized value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] init     The initialized value, 1 if initialized, otherwise 0
*/
void setInitialized(FILE **virDrive, size_t init)
{
	writeNum(virDrive, 0, 1, init);
}

/*
 * Sets the drive label value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive   A pointer to the file pointer of the virtual drive
 * @param[in] driveLabel A string holding the drive label. Must be 20 characters
 *                       or fewer
*/
void setDriveLabel(FILE **virDrive, char *driveLabel)
{
	writeStr(virDrive, 1, 20, driveLabel);
}

/*
 * Sets the bytes per cluster value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] bytes    The bytes per cluster
*/
void setBytesPerCluster(FILE **virDrive, size_t bytes)
{
	writeNum(virDrive, 21, 2, bytes);
}

/*
 * Sets the number of reserved clusters value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] reserved The number of reserved clusters
*/
void setNumberOfReservedClusters(FILE **virDrive, size_t reserved)
{
	writeNum(virDrive, 23, 2, reserved);
}

/*
 * Sets the number of clusters on drive value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive  A pointer to the file pointer of the virtual drive
 * @param[in] clusters  The number of clusters on the virtual drive
*/
void setNumberOfClustersOnDrive(FILE **virDrive, size_t clusters)
{
	writeNum(virDrive, 25, 4, clusters);
}

/*
 * Sets the number of clusters per FAT value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] clusters  The number of clusters per FAT
*/
void setNumberOfClustersPerFAT(FILE **virDrive, size_t clusters)
{
	writeNum(virDrive, 29, 4, clusters);
}

/*
 * Sets the first cluster of root directory value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] cluster  The number of the first cluster of the root directory
*/
void setFirstClusterOfRootDir(FILE **virDrive, size_t cluster)
{
	writeNum(virDrive, 33, 4, cluster);
}

/*
 * Sets the number of free clusters value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] clusters The number of free clusters 
*/
void setNumberOfFreeClusters(FILE **virDrive, size_t clusters)
{
	writeNum(virDrive, 37, 4, clusters);
}

/*
 * Sets the next free cluster value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] cluster  The number of the next free cluster
*/
void setNextFreeCluster(FILE **virDrive, size_t cluster)
{
	writeNum(virDrive, 41, 4, cluster);
}

/*
 * Sets the size of drive value on the boot cluster of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] size     The size of the drive (bytes)
*/
void setSizeOfDrive(FILE **virDrive, size_t size)
{
	writeNum(virDrive, 45, 4, size);
}
