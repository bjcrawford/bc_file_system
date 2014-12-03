/**
 * @file bc_dir_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the header elements for the bc_dir_operations.c
 *     file.
 * 
 *     This program was written for use in Linux.
*/

#ifndef BC_DIR_OPERATIONS
#define BC_DIR_OPERATIONS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bc_header.h"
#include "bc_dir_entry_struct.h"
#include "bc_fat_operations.h"
#include "bc_strlib/bc_strlib.h"

u_int createDirFileEntry(u_int clusterAddr, char attr, char *name, char *ext);
u_int createDirSubEntry(u_int clusterAddr, char attr, char *name);

void deleteDirEntry(u_int dirCluster, u_int entryAddr);

u_int dirFileEntryExists(u_int clusterAddr, char *fileName, char *fileExt);
u_int getDirFileEntryAddr(u_int clusterAddr, char *fileName, char *fileExt);

char *getDirectoryListing(char *dirPath);
u_int getDirectoryClusterAddress(u_int currentClusterAddr, char *dirName);

u_int encodeTimeBytes();
struct tm *decodeTimeBytes(u_int timeBytes);

u_int getDataStartLoc();
u_int getFirstFreeDirEntryAddr(u_int dirCluster);

u_int getDirEntryLoc(u_int dirCluster, u_int entryAddr);
DirEntry *getDirEntry(u_int dirCluster, u_int entryAddr);
void setDirEntry(u_int dirCluster, u_int entryAddr, DirEntry *entry);

#endif