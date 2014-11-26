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

void initBootCluster(FILE**, char*);

size_t getInitialized(FILE**);
char *getDriveLabel(FILE**);
size_t getBytesPerCluster(FILE**);
size_t getNumberOfReservedClusters(FILE**);
size_t getNumberOfClustersOnDrive(FILE**);
size_t getNumberOfClustersPerFAT(FILE**);
size_t getFirstClusterOfRootDir(FILE**);
size_t getNumberOfFreeClusters(FILE**);
size_t getNextFreeCluster(FILE**);
size_t getSizeOfDrive(FILE**);

void setInitialized(FILE**, size_t);
void setDriveLabel(FILE**, char*);
void setBytesPerCluster(FILE**, size_t);
void setNumberOfReservedClusters(FILE**, size_t);
void setNumberOfClustersOnDrive(FILE**, size_t);
void setNumberOfClustersPerFAT(FILE**, size_t);
void setFirstClusterOfRootDir(FILE**, size_t);
void setNumberOfFreeClusters(FILE**, size_t);
void setNextFreeCluster(FILE**, size_t);
void setSizeOfDrive(FILE**, size_t);

#endif