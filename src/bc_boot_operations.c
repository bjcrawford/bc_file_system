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
 *     a size of one cluster (512 bytes). The layout of the data contained 
 *     in the boot cluster is as follows:
 *     
 *        (bytes) | value 
 *       ---------------------------------------------------------------
 *        (0)      | A byte designating the virtual drive as initialized
 *        (1-23)   | A label for the virtual drive
 *        (24-27)  | The number of bytes per cluster
 *        (28-31)  | The number of reserved clusters
 *        (32-35)  | The number of clusters on the virtual drive
 *        (36-39)  | The number of clusters per file allocation table
 *        (40-43)  | The first cluster of the root directory
 *        (44-47)  | The number of free clusters
 *        (48-51)  | The next free cluster
 *        (52-55)  | The size of the virtual drive in bytes
 *        (55-511) | This space is unused
 *
 *     The boot record will be represented in the file system application 
 *     as a struct containing all of the properties listed above. 
 *
 *  This program was written for use in Linux.
*/

#include "bc_boot_operations.h"

/**
 * Initializes a boot record struct for the virtual drive.
 * NOTE: This function does not write the boot record struct to
 * the virtual drive.
 *
 * @param  driveLabel A string containing the label for the drive
 * @return            An pointer to an initialized boot record struct
 */
BootRecord *initBootRecord(char *driveLabel)
{
	BootRecord *boot = calloc(1, sizeof(*boot));

	/* Get size of the drive in bytes */
	rewind(virDrive);
	u_int dSize = 0;
	while(fgetc(virDrive) != EOF)
		dSize++;
	rewind(virDrive);

	boot->init = 1;
	strncpy(boot->label, driveLabel, DRIVE_LABEL_MAX);
	boot->bytesPerCluster = CLUSTER_SIZE;
	boot->reservedClusters = 1;
	boot->clustersOnDrive = dSize / CLUSTER_SIZE;
	boot->clustersPerFat = ceil((double) boot->clustersOnDrive * FAT_ENTRY_BYTES / CLUSTER_SIZE);
	boot->rootDirStart = boot->clustersPerFat + 1;
	boot->freeClusters = boot->clustersOnDrive - (boot->clustersPerFat + 2);
	boot->nextFreeCluster = boot->clustersPerFat + 2;
	boot->driveSize = dSize;

	return boot;
}

/**
 * Writes the global boot record struct to the virtual drive
 */
void writeBootRecord()
{
	rewind(virDrive);
	fwrite(bootRecord, sizeof(BootRecord), 1, virDrive);
}

/**
 * Reads the boot record from the drive and stores it in the
 * global boot record struct
 */
void readBootRecord()
{
	rewind(virDrive);
	fread(bootRecord, sizeof(BootRecord), 1, virDrive);
}