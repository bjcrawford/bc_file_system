/**
 * @file bc_header.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the type definitions, symbolic constants,
 *     and global variables used by the file system.
 *     
 *     This program was written for use in Linux.
*/

#ifndef BC_HEADER
#define BC_HEADER

/* ======= Type definitions ======= */

typedef unsigned int u_int;

/* ======= Constants ======= */

/* The maximum drive label length */
#define DRIVE_LABEL_MAX 23

/* The minimum file name length */
#define FILE_NAME_MIN 8

/* The maximum file name length */
#define FILE_NAME_MAX 42

/* The required file extension length */
#define FILE_EXT_SIZE 3

/* The maximum file size */
#define FILE_SIZE_MAX 16384

/* The allocation unit/cluster size */
#define CLUSTER_SIZE 512

/* The bytes per file allocation table entry */
#define FAT_ENTRY_BYTES 4

/* The bytes per directory entry */
#define DIR_ENTRY_BYTES 64

/* The directory entries per cluster */
#define DIR_ENTRIES_PER_CLUSTER 8

/* ======= Includes ======= */

#include <stdio.h>
#include "bc_boot_record_struct.h"

/* ======= Globals ======= */

/* The file pointer of the virtual drive file */
FILE *virDrive;

/* The boot record data structure */
BootRecord *bootRecord;

/* An array representing the file allocation table */
u_int *fileAllocTable;

#endif