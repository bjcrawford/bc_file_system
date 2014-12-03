/**
 * @file bc_drive_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file holds all of the operations which can be performed on
 *     the virtual drive. The virtual drive will be represented by a 
 *     pre-made file (one of the four files provided: Drive10MB, 
 *     Drive5MB, Drive3MB, or Drive2MB). The virtual drive's allocation 
 *     unit size, or cluster size (as it will be refered to throughout 
 *     these docs) will be 512 bytes. The layout of the clusters on the 
 *     virtual drive will be as follows (cluster addresses):
 *
 *       -   0: The boot cluster
 *       -   1: The first cluster of the file allocation table
 *       -   2: The second cluster of the file allocation table
 *         ...
 *       -   n: The last cluster of the file allocation table
 *              Note: n = (((drive size / cluster size) * FAT entry size) / cluster size) + 1
 *       - n+1: The root directory cluster
 *       - n+2: The first cluster of the data region
 *         ...
 *       -   x: The last cluster of the data region
 *              Note: x = (drive size / cluster size) - 1
 *              
 *  This program was written for use in Linux.
*/

#include "bc_drive_operations.h"

/**
 * Opens the virtual drive using the given file name
 *
 * @param  virDriveName A string containing the name of the virtual drive (file)
 *                      to open
 * @return              A pointer to the file pointer of the virtual drive
*/
FILE *openVirDrive(char *virDriveName)
{
	return fopen(virDriveName, "r+");
}

/** 
 * Formats the virtual drive. 
 *
*/
void formatVirDrive()
{
	rewind(virDrive);
	int i;
	u_int driveSize = 0;
	while(fgetc(virDrive) != EOF)
		driveSize++;
	rewind(virDrive);
	for(i = 0; i < driveSize; i++)
		fputc(0x00, virDrive);
	rewind(virDrive);
}

/**
 * Closes the virtual drive
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
*/
void closeVirDrive()
{
	fclose(virDrive);
}

/**
 * Formats the given cluster on the virtual drive
 *
 * @param clusterAddr The address of the cluster to format
 */
void formatCluster(u_int clusterAddr)
{
	u_int i;
	u_int loc = clusterAddr * bootRecord->bytesPerCluster;
	fseek(virDrive, loc, SEEK_SET);
	for(i = 0; i < bootRecord->bytesPerCluster; i++)
		fputc(0x00, virDrive);
}