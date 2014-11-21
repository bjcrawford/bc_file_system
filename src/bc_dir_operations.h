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

void createDirEntry(FILE **virDrive, size_t dirCluster,
	                     char attr, char *name, char *ext);
size_t encodeTimeBytes();
struct tm *decodeTimeBytes(size_t timeBytes);

size_t getDataStartLoc(FILE **virDrive);
size_t getDirEntryLoc(FILE **virDrive, size_t dirCluster, size_t entryAddr);
char getDirEntryAttr(FILE **virDrive, size_t dirCluster, size_t entryAddr);
char *getDirEntryFileName(FILE **virDrive, size_t dirCluster, size_t entryAddr);
char *getDirEntryFileExt(FILE **virDrive, size_t dirCluster, size_t entryAddr);
size_t getDirEntryCreateTimeBytes(FILE **virDrive, size_t dirCluster, size_t entryAddr);
size_t getDirEntryModifiedTimeBytes(FILE **virDrive, size_t dirCluster, size_t entryAddr);
size_t getDirEntryStartCluster(FILE **virDrive, size_t dirCluster, size_t entryAddr);
size_t getDirEntryFileSize(FILE **virDrive, size_t dirCluster, size_t entryAddr);
size_t getFirstFreeDirEntry(FILE **virDrive, size_t dirCluster);

void setDirEntryAttr(FILE **virDrive, size_t dirCluster, 
	                 size_t entryAddr, char attr);
void setDirEntryFileName(FILE **virDrive, size_t dirCluster,
	                     size_t entryAddr, char *name);
void setDirEntryFileExt(FILE **virDrive, size_t dirCluster,
	                    size_t entryAddr, char *ext);
void setDirEntryModifiedTimeBytes(FILE **virDrive, size_t dirCluster,
	                              size_t entryAddr, size_t timeBytes);
void setDirEntryStartCluster(FILE **virDrive, size_t dirCluster,
	                         size_t entryAddr, size_t clusterAddr);
void setDirEntryFileSize(FILE **virDrive, size_t dirCluster,
	                     size_t entryAddr, size_t fileSize);
size_t adjustDirEntryFileSize(FILE **virDrive, size_t dirCluster,
	                          size_t entryAddr, size_t adjFileSize,
	                          size_t increase);

#endif