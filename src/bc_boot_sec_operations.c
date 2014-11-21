/**
 * @file bc_boot_sec_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file holds all of the operations which can be performed on 
 *     the boot sector of the virtual drive. The boot sector will have 
 *     a size of 512 bytes. The layout of the data contained in the 
 *     boot sector is as follows:
 *     
 *        (bytes) | value 
 *       ---------------------------------------------------------------
 *        (0)     | A byte designating the virtual drive as initialized
 *        (1-20)  | A label for the virtual drive
 *        (21-22) | The number of bytes per sector
 *        (23)    | The number of sectors per cluster
 *        (24-25) | The number of reserved sectors
 *        (26-29) | The number of sectors on the virtual drive
 *        (30-33) | The number of sectors per file allocation table
 *        (34-37) | The first cluster of the root directory
 *        (38-41) | The number of free clusters
 *        (42-45) | The next free cluster
 *        (46-49) | The size of the virtual drive in bytes
 *
 * 
 *  This program was written for use in Linux.
*/

#include "bc_boot_sec_operations.h"

/**
 * Initilizes the boot sector of a virtual drive. The virtual drive should
 * be formatted prior to using this function.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] driveLabel A string holding the label to associate with the
 *                       drive. Must be 20 characters or fewer.
*/
void initBootSector(FILE **virDrive, char *driveLabel)
{
	rewind(*virDrive);
	/* Rewind can throw an error under certain conditions */

	size_t driveSize = 0;
	while(fgetc(*virDrive) != EOF)
		driveSize++;

	rewind(*virDrive);
	setDriveLabel(virDrive, driveLabel);
	setBytesPerSector(virDrive, 512);
	setSectorsPerCluster(virDrive, 1);
	setNumberOfReservedSectors(virDrive, 1);
	setNumberOfSectorsOnDrive(virDrive, driveSize / 512);
	setNumberOfSectorsPerFAT(virDrive, (((driveSize / 512) * 4) / 512) + 1);
	setFirstClusterOfRootDir(virDrive, (((driveSize / 512) * 4) / 512) + 2);
	/* total clusters - (FAT + boot + root) */
	setNumberOfFreeClusters(virDrive, (driveSize / 512) - ((((driveSize / 512) * 4) / 512) + 3));
	setNextFreeCluster(virDrive, (((driveSize / 512) * 4) / 512) + 3);
	setSizeOfDrive(virDrive, driveSize);
	setInitialized(virDrive, 1);

	rewind(*virDrive);
}

/*
 * Returns the initialized value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             1 if drive is initialized, otherwise 0
*/
size_t getInitialized(FILE **virDrive)
{
	return readNum(virDrive, 0, 1);
}

/*
 * Returns the drive label from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             A string containing the drive label
*/
char *getDriveLabel(FILE **virDrive)
{
	return readStr(virDrive, 1, 20);
}

/*
 * Returns the bytes per sector value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The bytes per sector
*/
size_t getBytesPerSector(FILE **virDrive)
{
	return readNum(virDrive, 21, 2);
}

/*
 * Returns the sectors per cluster value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The sectors per cluster
*/
size_t getSectorsPerCluster(FILE **virDrive)
{
	return readNum(virDrive, 23, 1);
}

/*
 * Returns the number of reserved sectors value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of reserved sectors
*/
size_t getNumberOfReservedSectors(FILE **virDrive)
{
	return readNum(virDrive, 24, 2);
}

/*
 * Returns the number of sectors on drive value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of sectors
*/
size_t getNumberOfSectorsOnDrive(FILE **virDrive)
{
	return readNum(virDrive, 26, 4);
}

/*
 * Returns the number of sectors per FAT value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of sectors per FAT
*/
size_t getNumberOfSectorsPerFAT(FILE **virDrive)
{
	return readNum(virDrive, 30, 4);
}

/*
 * Returns the first cluster of root root directory value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The first cluster of the root directory
*/
size_t getFirstClusterOfRootDir(FILE **virDrive)
{
	return readNum(virDrive, 34, 4);
}

/*
 * Returns the number of free clusters value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The number of free clusters on the drive
*/
size_t getNumberOfFreeClusters(FILE **virDrive)
{
	return readNum(virDrive, 38, 4);
}

/*
 * Returns the next free cluster value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The next free cluster on the drive
*/
size_t getNextFreeCluster(FILE **virDrive)
{
	return readNum(virDrive, 42, 4);
}

/*
 * Returns the size of drive value from the boot sector of the virtual drive.
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @return             The size of the drive in bytes
*/
size_t getSizeOfDrive(FILE **virDrive)
{
	return readNum(virDrive, 46, 4);
}

/*
 * Sets the initialized value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] init     The initialized value, 1 if initialized, otherwise 0
*/
void setInitialized(FILE **virDrive, size_t init)
{
	writeNum(virDrive, 0, 1, init);
}

/*
 * Sets the drive label value on the boot sector of the virtual drive
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
 * Sets the bytes per sector value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] bytes    The bytes per sector
*/
void setBytesPerSector(FILE **virDrive, size_t bytes)
{
	writeNum(virDrive, 21, 2, bytes);
}

/*
 * Sets the sectors per cluster value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] sectors  The sectors per cluster
*/
void setSectorsPerCluster(FILE **virDrive, size_t sectors)
{
	writeNum(virDrive, 23, 1, sectors);
}

/*
 * Sets the number of reserved sectors value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] reserved The number of reserved sectors
*/
void setNumberOfReservedSectors(FILE **virDrive, size_t reserved)
{
	writeNum(virDrive, 24, 2, reserved);
}

/*
 * Sets the number of sector on drive value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] sectors  The number of sectors on the virtual drive
*/
void setNumberOfSectorsOnDrive(FILE **virDrive, size_t sectors)
{
	writeNum(virDrive, 26, 4, sectors);
}

/*
 * Sets the number of sectors per FAT value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] sectors  The number of sectors per FAT
*/
void setNumberOfSectorsPerFAT(FILE **virDrive, size_t sectors)
{
	writeNum(virDrive, 30, 4, sectors);
}

/*
 * Sets the first cluster of root directory value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] cluster  The number of the first cluster of the root directory
*/
void setFirstClusterOfRootDir(FILE **virDrive, size_t cluster)
{
	writeNum(virDrive, 34, 4, cluster);
}

/*
 * Sets the number of free clusters value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] clusters The number of free clusters 
*/
void setNumberOfFreeClusters(FILE **virDrive, size_t clusters)
{
	writeNum(virDrive, 38, 4, clusters);
}

/*
 * Sets the next free cluster value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] cluster  The number of the next free cluster
*/
void setNextFreeCluster(FILE **virDrive, size_t cluster)
{
	writeNum(virDrive, 42, 4, cluster);
}

/*
 * Sets the size of drive value on the boot sector of the virtual drive
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] size     The size of the drive (bytes)
*/
void setSizeOfDrive(FILE **virDrive, size_t size)
{
	writeNum(virDrive, 46, 4, size);
}
