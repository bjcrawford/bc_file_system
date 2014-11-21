/**
 * @file bc_bc_boot_sec_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#ifndef BC_BOOT_SEC_OPERATIONS
#define BC_BOOT_SEC_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include "bc_drive_operations.h"

void initBootSector(FILE **virDrive, char *driveLabel);

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

void setInitialized(FILE **virDrive, size_t init);
void setDriveLabel(FILE **virDrive, char *driveLabel);
void setBytesPerSector(FILE **virDrive, size_t bytes);
void setSectorsPerCluster(FILE **virDrive, size_t sectors);
void setNumberOfReservedSectors(FILE **virDrive, size_t reserved);
void setNumberOfSectorsOnDrive(FILE **virDrive, size_t sectors);
void setNumberOfSectorsPerFAT(FILE **virDrive, size_t sectors);
void setFirstClusterOfRootDir(FILE **virDrive, size_t cluster);
void setNumberOfFreeClusters(FILE **virDrive, size_t clusters);
void setNextFreeCluster(FILE **virDrive, size_t cluster);
void setSizeOfDrive(FILE **virDrive, size_t size);

#endif