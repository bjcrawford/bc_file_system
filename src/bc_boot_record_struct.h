/**
 * @file bc_boot_record_struct.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the struct which represents the properties
 *     contained within the boot record. For a more detailed explanation
 *     of the properties see the documentation in bc_boot_operations.c
 *     
 *     This program was written for use in Linux.
*/

#ifndef BC_BOOT_RECORD_STRUCT
#define BC_BOOT_RECORD_STRUCT

#include "bc_header.h"

typedef struct 
{
	char init;
	char label[DRIVE_LABEL_MAX + 1];
	u_int bytesPerCluster;
	u_int reservedClusters;
	u_int clustersOnDrive;
	u_int clustersPerFat;
	u_int rootDirStart;
	u_int freeClusters;
	u_int nextFreeCluster;
	u_int driveSize;

} BootRecord;

#endif