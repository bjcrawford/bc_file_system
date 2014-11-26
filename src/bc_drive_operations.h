/**
 * @file bc_drive_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#ifndef BC_DRIVE_OPERATIONS
#define BC_DRIVE_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include "bc_boot_sec_operations.h"
#include "bc_fat_operations.h"

int openVirDrive(FILE**, char*);
int closeVirDrive(FILE**);
void initVirDrive(FILE**, char*);
void formatVirDrive(FILE**);

void writeNum(FILE**, size_t, size_t, size_t);
void writeStr(FILE**, size_t, size_t, char*);
size_t readNum(FILE**, size_t, size_t);
char *readStr(FILE**, size_t, size_t);

 #endif