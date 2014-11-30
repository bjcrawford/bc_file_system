/**
 * @file bc_file_system.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the necessary includes for my file system
 *     implementation. The file also contains some constants to be 
 *     used in the file system as specified in the project guidelines.
 *     
 *     This program was written for use in Linux.
*/


#ifndef BC_FILE_SYSTEM
#define BC_FILE_SYSTEM

/* The minimum file name length */
#define FILE_NAME_MIN 8

/* The maximum file name length */
#define FILE_NAME_MAX 12

/* The required file extension length */
#define FILE_EXT_SIZE 3

/* The maximum file size */
#define FILE_SIZE_MAX 16384

/* The allocation unit/cluster size */
#define CLUSTER_SIZE  512

/* The bytes per file allocation table entry */
#define FAT_ENTRY_BYTES 4

#include "bc_drive_operations.h"
#include "bc_boot_operations.h"
#include "bc_dir_operations.h"
#include "bc_file_operations.h"
#include "bc_strlib/bc_strlib.h"

#endif
