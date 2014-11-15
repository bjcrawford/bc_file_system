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


int openVirDrive(FILE **virDrive, char *virDriveName)
{
	*virDrive = fopen(virDriveName, "r+");

	return !(*virDrive == NULL);
}

int closeVirDrive(FILE **virDrive)
{
	return fclose(*virDrive);
}

int initVirDrive(FILE **virDrive, char *driveLabel)
{
	return initBootSector(virDrive, driveLabel);
}

int initBootSector(FILE **virDrive, char *driveLabel)
{
	/* Determine size (in bytes) of drive */
	size_t driveSize = 0;
	rewind(*virDrive);
	while(fgetc(*virDrive) != EOF)
		driveSize++;
	rewind(*virDrive);

	/* Drive label */
	writeStrParam(virDrive, 1, 10, driveLabel);

	/* Bytes per sector */
	writeNumParam(virDrive, 11, 2, 512);

	/* Sectors per cluster */
	writeNumParam(virDrive, 13, 1, 1);

	/* Number of reserved sectors */
	writeNumParam(virDrive, 14, 2, 1);

	/* Number of sectors on the drive */
	writeNumParam(virDrive, 16, 4, driveSize / 512);

	/* Number of sectors per FAT */
	writeNumParam(virDrive, 20, 4, (((driveSize / 512) * 4) / 512) + 1);

	/* First cluster of root directory */
	writeNumParam(virDrive, 24, 4, (((driveSize / 512) * 4) / 512) + 2);

	/* Number of free clusters */
	writeNumParam(virDrive, 28, 4, (driveSize / 512) - ((((driveSize / 512) * 4) / 512) + 3));

	/* Next free cluster */
	writeNumParam(virDrive, 32, 4, (((driveSize / 512) * 4) / 512) + 3);

	/* Size of drive */
	writeNumParam(virDrive, 36, 4, driveSize);

	/* Initialized */
	writeNumParam(virDrive, 0, 1, 1);

	rewind(*virDrive);

	return 0;
}

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

void writeNumParam(FILE **virDrive, size_t loc, size_t len, size_t param)
{
	size_t i;
	char *p = (char*) &param;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len; i++)
		fputc(*p++, *virDrive);
}

void writeStrParam(FILE **virDrive, size_t loc, size_t len, char *param)
{
	size_t i;
	char c;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len && (c = *param++) != '\n'; i++)
		fputc(c, *virDrive);
	while(i++ < len)
		fputc(0, *virDrive);
}

size_t readNumParam(FILE **virDrive, size_t loc, size_t len)
{
	size_t i;
	size_t result = 0;
	char *p = (char*) &result;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len && i < sizeof(size_t); i++)
		*p++ = fgetc(*virDrive);

	return result;
}

char *readStrParam(FILE **virDrive, size_t loc, size_t len)
{
	size_t i;
	char *result = (char*) calloc(sizeof(char), len);
	char *p = result;
	fseek(*virDrive, loc, SEEK_SET);
	for(i = 0; i < len; i++)
		*p++ = fgetc(*virDrive);

	return result;
}

size_t getInitialized(FILE **virDrive)
{
	return readNumParam(virDrive, 0, 1);
}

char *getDriveLabel(FILE **virDrive)
{
	return readStrParam(virDrive, 1, 10);
}

size_t getBytesPerSector(FILE **virDrive)
{
	return readNumParam(virDrive, 11, 2);
}

size_t getSectorsPerCluster(FILE **virDrive)
{
	return readNumParam(virDrive, 13, 1);
}

size_t getNumberOfReservedSectors(FILE **virDrive)
{
	return readNumParam(virDrive, 14, 2);
}

size_t getNumberOfSectorsOnDrive(FILE **virDrive)
{
	return readNumParam(virDrive, 16, 4);
}

size_t getNumberOfSectorsPerFAT(FILE **virDrive)
{
	return readNumParam(virDrive, 20, 4);
}

size_t getFirstClusterOfRootDir(FILE **virDrive)
{
	return readNumParam(virDrive, 24, 4);
}

size_t getNumberOfFreeClusters(FILE **virDrive)
{
	return readNumParam(virDrive, 28, 4);
}

size_t getNextFreeCluster(FILE **virDrive)
{
	return readNumParam(virDrive, 32, 4);
}

size_t getSizeOfDrive(FILE **virDrive)
{
	return readNumParam(virDrive, 36, 4);
}




