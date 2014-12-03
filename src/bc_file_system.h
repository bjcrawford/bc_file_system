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
 *     This file contains the header for the file system implementation. 
 *     
 *     This program was written for use in Linux.
*/

#ifndef BC_FILE_SYSTEM
#define BC_FILE_SYSTEM

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bc_strlib/bc_strlib.h"

 /* Constants */

#define DRIVE_LABEL_MAX 23
#define FILE_NAME_MIN 8
#define FILE_NAME_MAX 42
#define FILE_EXT_SIZE 3
#define FILE_SIZE_MAX 16384
#define CLUSTER_SIZE 512
#define FAT_ENTRY_BYTES 4
#define DIR_ENTRY_BYTES 64
#define DIR_ENTRIES_PER_CLUSTER 8

/* Type definitions */

typedef unsigned int u_int;

/* Structs */

typedef struct 
{
	char init;
	char label[DRIVE_LABEL_MAX + 1];
	u_int bytesPerCluster;
	u_int reservedClusters;
	u_int clustersOnDrive;
	u_int clustersPerFat;
	u_int rootDirStart;
	u_int freeClusters;
	u_int nextFreeCluster;
	u_int driveSize;

} BootRecord;

typedef struct 
{
	char attr;
	char fileName[FILE_NAME_MAX + 1];
	char fileExt[FILE_EXT_SIZE + 1];
	u_int createDate;
	u_int modifiedDate;
	u_int startCluster;
	u_int fileSize;

} DirEntry;

typedef struct
{
	u_int used;
	u_int write;
	u_int hidden;
	u_int subDir;
	char fileName[FILE_NAME_MAX + 1];
	char fileExt[FILE_EXT_SIZE + 1];
	u_int createDate;
	u_int modifyDate;
	u_int filePosition;
	u_int fileSize;
	u_int startClusterAddr;
	u_int startLoc;
	u_int currentClusterAddr;
	u_int currentLoc;
	u_int dirClusterAddr;
	u_int dirEntryAddr;

} BC_FILE;

/* Globals */

FILE *virDrive;
BootRecord *bootRecord;
u_int *fileAllocTable;

/* File System Operations */

void initFileSystem(char *virDriveName, char *virDriveLabel);
void closeFileSystem();

/* Virtual Drive Operations */

FILE *openVirDrive(char *virDriveName);
void formatVirDrive();
void closeVirDrive();
void formatCluster(u_int clusterAddr);

/* Boot Record Operations */

BootRecord *initBootRecord(char *driveLabel);
void writeBootRecord();
void readBootRecord();

/* File Allocation Table Operations */

u_int *initFATClusters();
void writeFAT();
void readFAT();
u_int addClusterToChain(u_int clusterAddr);
void findAndSetNextFreeCluster();

/* Directory Entry Operations */

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

/* File Struct Operations */

void rewindBC_File(BC_FILE*);
void destroyBC_File(BC_FILE*);

/* File Operations */

BC_FILE *openFile(char *filePath);
void createDirectory(char *dirPath);
void writeFile(void *src, u_int len, BC_FILE *dest);
void readFile(void *dest, u_int size, u_int len, BC_FILE *src);
void closeFile(BC_FILE *file);
void deleteFile(BC_FILE *file);

#endif
