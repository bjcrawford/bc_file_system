/**
 * @file bc_drive_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the header elements for the bc_drive_operations.c
 *     file.
 * 
 *     This program was written for use in Linux.
*/

#ifndef BC_DRIVE_OPERATIONS
#define BC_DRIVE_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include "bc_header.h"

FILE *openVirDrive(char *virDriveName);
void formatVirDrive();
void closeVirDrive();

void writeNum(u_int loc, u_int len, u_int num);
void writeStr(u_int loc, u_int len, char *str);
u_int readNum(u_int loc, u_int len);
char *readStr(u_int loc, u_int len);

#endif