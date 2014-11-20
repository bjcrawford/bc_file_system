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

#include <stdio.h>
#include <stdlib.h>
#include "bc_boot_sec_operations.h"

#ifndef BC_DRIVE_OPERATIONS
#define BC_DRIVE_OPERATIONS

int openVirDrive(FILE **virDrive, char *driveName);
int closeVirDrive(FILE **virDrive);
int initVirDrive(FILE **virDrive, char *driveLabel);
void formatVirDrive(FILE **virDrive);

void writeNum(FILE **virDrive, size_t loc, size_t len, size_t num);
void writeStr(FILE **virDrive, size_t loc, size_t len, char *str);
size_t readNum(FILE **virDrive, size_t loc, size_t len);
char *readStr(FILE **virDrive, size_t loc, size_t len);

 #endif