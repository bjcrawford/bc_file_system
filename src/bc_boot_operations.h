/**
 * @file bc_boot_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the header elements for the bc_boot_operations.c
 *     file.
 * 
 *     This program was written for use in Linux.
*/

#ifndef BC_BOOT_OPERATIONS
#define BC_BOOT_OPERATIONS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bc_header.h"
#include "bc_boot_record_struct.h"

BootRecord *initBootRecord(char *driveLabel);
void writeBootRecord();
void readBootRecord();

#endif