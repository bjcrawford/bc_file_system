/**
 * @file bc_file_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the header elements for the bc_file_operations.c
 *     file.
 * 
 *     This program was written for use in Linux.
*/

#ifndef BC_FILE_OPERATIONS
#define BC_FILE_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bc_header.h"
#include "bc_file_struct.h"
#include "bc_dir_entry_struct.h"
#include "bc_drive_operations.h"
#include "bc_dir_operations.h"
#include "bc_strlib/bc_strlib.h"

BC_FILE *openFile(char *filePath);
void createDirectory(char *dirPath);

void writeFile(void *src, u_int len, BC_FILE *dest);
void readFile(void *dest, u_int size, u_int len, BC_FILE *src);

void closeFile(BC_FILE *file);
void deleteFile(BC_FILE *file);

#endif