/**
 * @file bc_fat_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#ifndef BC_FAT_OPERATIONS
#define BC_FAT_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include "bc_drive_operations.h"
#include "bc_boot_sec_operations.h"

void initFATClusters(FILE**);

size_t getFATEntry(FILE**, size_t);
size_t getFATStartLocation(FILE**);

void setFATEntry(FILE**, size_t, size_t);

size_t addClusterToChain(FILE**, size_t);
void findAndSetNextFreeCluster(FILE**);

#endif