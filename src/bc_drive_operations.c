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
 * Opens a virtual drive (file) using the given name
 *
 * @param[out] virDrive     A pointer to the file pointer to assign the virtual drive
 * @param[in]  virDriveName A string containing the name of the virtual drive (file)
 *                          to open
*/
 int openVirDrive(FILE **virDrive, char *virDriveName)
{
	*virDrive = fopen(virDriveName, "r+");

	return !(*virDrive == NULL);
}

/**
 * Closes a virtual drive (file)
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
*/
int closeVirDrive(FILE **virDrive)
{
	return fclose(*virDrive);
}

/** 
 * Initializes a virtual drive (file). Formats the virtual drive and initializes 
 * the boot cluster.
 *
 * @param[in] virDrive   A pointer to the file pointer of the virtual drive
 * @param[in] driveLabel A string containing the drive label
*/
void initVirDrive(FILE **virDrive, char *driveLabel)
{
	formatVirDrive(virDrive);
	initBootCluster(virDrive, driveLabel);
	initFATClusters(virDrive);
}

/** 
 * Formats a virtual drive (file). 
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
*/
void formatVirDrive(FILE **virDrive)
{
	rewind(*virDrive);
	int i;
	size_t driveSize = 0;
	while(fgetc(*virDrive) != EOF)
		driveSize++;
	rewind(*virDrive);
	for(i = 0; i < driveSize; i++)
		fputc(0x00, *virDrive);
	rewind(*virDrive);
}

/**
 * Writes a non-negative value to the virtual drive (file)
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] loc      The offset in bytes at which to write the value
 * @param[in] len      The maximum length in bytes of the write
 * @param[in] num      The value to be written
*/
void writeNum(FILE **virDrive, size_t loc, size_t len, size_t num)
{
	size_t i;
	char *p = (char*) &num;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len; i++)
		fputc(*p++, *virDrive);
}

/**
 * Writes a string to the virtual drive (file) 
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] loc      The offset in bytes at which to write the string
 * @param[in] len      The maximum length in bytes of the write
 * @param[in] str      The string to be written
 */
void writeStr(FILE **virDrive, size_t loc, size_t len, char *str)
{
	fseek(*virDrive, loc, SEEK_SET);
	fwrite(str, sizeof(char), len, *virDrive);
}

/**
 * Reads a value from the virtual drive (file)
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] loc      The offset in bytes at which to read the value
 * @param[in] len      The maximum length in bytes of the read
 * @return             The value read
*/
size_t readNum(FILE **virDrive, size_t loc, size_t len)
{
	size_t i;
	size_t result = 0;
	char *p = (char*) &result;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len && i < sizeof(size_t); i++)
		*p++ = fgetc(*virDrive);

	return result;
}

/**
 * Reads a string from the virtual drive (file) 
 *
 * @param[in] virDrive A pointer to the file pointer of the virtual drive
 * @param[in] loc      The offset in bytes at which to read the string
 * @param[in] len      The maximum length in bytes of the read
 * @param[in] str      The string read
 */
char *readStr(FILE **virDrive, size_t loc, size_t len)
{
	char *result = (char*) calloc(len + 1, sizeof(char));
	fseek(*virDrive, loc, SEEK_SET);
	fread(result, sizeof(char), len, *virDrive);
	
	return result;
}