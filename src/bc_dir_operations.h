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
#include <string.h>
#include <time.h>
#include "bc_file_system.h"
#include "bc_dir_entry_struct.h"
#include "bc_drive_operations.h"
#include "bc_boot_operations.h"
#include "bc_fat_operations.h"
#include "bc_strlib/bc_strlib.h"

u_int createDirFileEntry(u_int clusterAddr, char attr, char *name, char *ext);
u_int createDirSubEntry(u_int clusterAddr, char attr, char *name);
u_int dirFileEntryExists(u_int clusterAddr, char *fileName, char *fileExt);
u_int getDirFileEntryAddr(u_int clusterAddr, char *fileName, char *fileExt);
char *getDirectoryListing(char *dirPath);
u_int getDirectoryClusterAddress(u_int currentClusterAddr, char *dirName);

u_int encodeTimeBytes();
struct tm *decodeTimeBytes(u_int);

u_int getDataStartLoc();
u_int getDirEntryLoc(u_int dirCluster, u_int entryAddr);
u_int getFirstFreeDirEntryAddr(u_int dirCluster);

char getDirEntryAttr(u_int dirCluster, u_int entryAddr);
char *getDirEntryFileName(u_int dirCluster, u_int entryAddr);
char *getDirEntryFileExt(u_int dirCluster, u_int entryAddr);
u_int getDirEntryCreateTimeBytes(u_int dirCluster, u_int entryAddr);
u_int getDirEntryModifiedTimeBytes(u_int dirCluster, u_int entryAddr);
u_int getDirEntryStartCluster(u_int dirCluster, u_int entryAddr);
u_int getDirEntryFileSize(u_int dirCluster, u_int entryAddr);

void setDirEntryAttr(u_int dirCluster, u_int entryAddr, char attr);
void setDirEntryFileName(u_int dirCluster, u_int entryAddr, char *name);
void setDirEntryFileExt(u_int dirCluster, u_int entryAddr, char *ext);
void setDirEntryModifiedTimeBytes(u_int dirCluster, u_int entryAddr, u_int timeBytes);
void setDirEntryStartCluster(u_int dirCluster, u_int entryAddr, u_int clusterAddr);
void setDirEntryFileSize(u_int dirCluster, u_int entryAddr, u_int fileSize);

u_int adjustDirEntryFileSize(u_int dirCluster, u_int entryAddr, u_int adjFileSize, u_int increase);

#endif