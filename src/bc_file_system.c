/**
 * @file bc_file_system.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include "bc_file_system.h"

/** 
 * Initializes the file system. If the given virtual drive has 
 * previously been initialized, the drive will be opened and
 * ready for use and the virtual drive label parameter will
 * not be used. If the given virtual drive has not been 
 * previously initialized, the virtual drive will be
 * initialized and use the given virtual drive label.
 * 
 *
 * @param virDriveName  The file name of the virtual drive
 * @param virDriveLabel The label to give the virtual drive
 */
void initFileSystem(char *virDriveName, char *virDriveLabel)
{
	virDrive = openVirDrive(virDriveName);

	/* Check if the drive has previously been initialized */
	rewind(virDrive);
	char init = getc(virDrive);
	if(init)
	{
		fprintf(stdout, "Drive has previously been initialized.\n");
		fprintf(stdout, "Loading drive properties.\n");
		bootRecord = calloc(1, sizeof(*bootRecord));
		readBootRecord();
		fileAllocTable = (u_int*) calloc(sizeof(u_int), bootRecord->clustersOnDrive);
		readFAT();
	}
	else
	{
		fprintf(stdout, "Drive has not previously been initialized.\n");
		fprintf(stdout, "Initializing drive properties.\n");
		formatVirDrive();
		bootRecord = initBootRecord(virDriveLabel);
		writeBootRecord();
		fileAllocTable = initFATClusters();
		writeFAT();
	}
}

void closeFileSystem()
{
	writeBootRecord();
	writeFAT();
	closeVirDrive();
}