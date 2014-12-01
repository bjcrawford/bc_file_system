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

void initFileSystem(char *virDriveName, char *virDriveLabel)
{
	virDrive = openVirDrive(virDriveName);

	/* Check if the drive has previously been initialized */

	formatVirDrive();

	bootRecord = initBootRecord(virDriveLabel);
	writeBootRecord();
	fileAllocTable = initFATClusters();
	writeFAT();
}