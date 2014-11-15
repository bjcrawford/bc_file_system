/**
 * @file bc_file_system.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef BC_FILE_SYSTEM
#define BC_FILE_SYSTEM

int openVirDrive(FILE **virDrive, char *driveName);
int closeVirDrive(FILE **virDrive);
int initVirDrive(FILE **virDrive, char *driveLabel);
int initBootSector(FILE **virDrive, char *driveLabel);
void formatVirDrive(FILE **virDrive);

void writeNumParam(FILE **virDrive, size_t loc, size_t len, size_t param);
void writeStrParam(FILE **virDrive, size_t loc, size_t len, char *param);
size_t readNumParam(FILE **virDrive, size_t loc, size_t len);
char *readStrParam(FILE **virDrive, size_t loc, size_t len);

size_t getInitialized(FILE **virDrive);
char *getDriveLabel(FILE **virDrive);
size_t getBytesPerSector(FILE **virDrive);
size_t getSectorsPerCluster(FILE **virDrive);
size_t getNumberOfReservedSectors(FILE **virDrive);
size_t getNumberOfSectorsOnDrive(FILE **virDrive);
size_t getNumberOfSectorsPerFAT(FILE **virDrive);
size_t getFirstClusterOfRootDir(FILE **virDrive);
size_t getNumberOfFreeClusters(FILE **virDrive);
size_t getNextFreeCluster(FILE **virDrive);
size_t getSizeOfDrive(FILE **virDrive);

#endif
