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
#include "bc_file_system.h"
#include "bc_drive_operations.h"
#include "bc_boot_operations.h"

u_int *initFATClusters();

void writeFAT();
void readFAT();

u_int addClusterToChain(u_int clusterAddr);
void findAndSetNextFreeCluster();

#endif