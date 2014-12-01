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
 *        (1-20)   | A label for the virtual drive
 *        (21-24)  | The number of bytes per cluster
 *        (25-28)  | The number of reserved clusters
 *        (29-32)  | The number of clusters on the virtual drive
 *        (33-36)  | The number of clusters per file allocation table
 *        (37-40)  | The first cluster of the root directory
 *        (41-44)  | The number of free clusters
 *        (45-48)  | The next free cluster
 *        (49-52)  | The size of the virtual drive in bytes
 *        (53-511) | This space is unused
 *
 *     The boot record will be represented in the file system application 
 *     as a struct containing all of the properties listed above. 
 *
 *  This program was written for use in Linux.
*/

#include "bc_boot_operations.h"

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
	strncpy(boot->label, driveLabel, 20);
	boot->bytesPerCluster = CLUSTER_SIZE;
	boot->reservedClusters = 1;
	boot->clustersOnDrive = dSize / CLUSTER_SIZE;
	boot->clustersPerFat = ceil((double) boot->clustersOnDrive * FAT_ENTRY_BYTES) / CLUSTER_SIZE;
	boot->rootDirStart = boot->clustersPerFat + 1;
	boot->freeClusters = boot->clustersOnDrive - (boot->clustersPerFat + 2);
	boot->nextFreeCluster = boot->clustersPerFat + 2;
	boot->driveSize = dSize;

	return boot;
}

void writeBootRecord()
{
	rewind(virDrive);
	fwrite(bootRecord, sizeof(bootRecord), 1, virDrive);
}

void readBootRecord()
{
	rewind(virDrive);
	fread(bootRecord, sizeof(bootRecord), 1, virDrive);
}