/**
 * @file bc_fat_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *  This program was written for use in Linux.
*/

#ifndef BC_DIR_OPERATIONS
#define BC_DIR_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bc_drive_operations.h"
#include "bc_boot_sec_operations.h"
#include "bc_fat_operations.h"

void createDirEntry(FILE**, size_t, char, char*, char*);
size_t encodeTimeBytes();
struct tm *decodeTimeBytes(size_t);

size_t getDataStartLoc(FILE**);
size_t getDirEntryLoc(FILE**, size_t, size_t);
char getDirEntryAttr(FILE**, size_t, size_t);
char *getDirEntryFileName(FILE**, size_t, size_t);
char *getDirEntryFileExt(FILE**, size_t, size_t);
size_t getDirEntryCreateTimeBytes(FILE**, size_t, size_t);
size_t getDirEntryModifiedTimeBytes(FILE**, size_t, size_t);
size_t getDirEntryStartCluster(FILE**, size_t, size_t);
size_t getDirEntryFileSize(FILE**, size_t, size_t);
size_t getFirstFreeDirEntry(FILE**, size_t);

void setDirEntryAttr(FILE**, size_t, size_t, char);
void setDirEntryFileName(FILE**, size_t, size_t, char*);
void setDirEntryFileExt(FILE**, size_t, size_t, char*);
void setDirEntryModifiedTimeBytes(FILE**, size_t, size_t, size_t);
void setDirEntryStartCluster(FILE**, size_t, size_t, size_t);
void setDirEntryFileSize(FILE**, size_t, size_t, size_t);
size_t adjustDirEntryFileSize(FILE**, size_t, size_t, size_t, size_t);

#endif