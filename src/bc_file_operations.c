/**
 * @file bc_file_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file holds all of the operations that can be performed 
 *     on the file clusters of the virtual drive.
*/

#include "bc_file_operations.h"

BC_FILE *openFile(FILE **virDrive, char *filePath)
{
	BC_FILE *fp = malloc(sizeof(fp));
	char *file = (char*) calloc(17, sizeof(char));
	char *fileName = (char*) calloc(13, sizeof(char));
	char *fileExt = (char*) calloc(4, sizeof(char));
	size_t clusterAddr = getRootDirectoryCluster(virDrive);
	size_t nextClusterAddr;
	size_t entryAddr;

	if(strchr(filePath, '/') != NULL)
	{
		char **path = chop(filePath, '/');
		char **p = path;
		int i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			nextClusterAddr = getDirectoryClusterAddress(virDrive, clusterAddr, p[i]);
			if(nextClusterAddr == 0)
			{
				size_t entryAddr = createDirSubEntry(virDrive, clusterAddr, 0x13, p[i]);
				nextClusterAddr = getDirEntryStartCluster(virDrive, clusterAddr, entryAddr);
			}
			clusterAddr = nextClusterAddr;
			i++;
		}
		strcpy(file, p[i]);
	}
	else
		strcpy(file, filePath);

	strcpy(fileName, strtok(file, "."));
	strcpy(fileExt, strtok(NULL, "."));

	entryAddr = createDirFileEntry(virDrive, clusterAddr, 0x1, fileName, fileExt);

	/* This function could be modified to take a "mode" and
	   to set the following four attributes accordingly */
	fp->used = 1;
	fp->write = 1;
	fp->hidden = 0;
	fp->subDir = 0;

	fp->fileName = getDirEntryFileName(virDrive, clusterAddr, entryAddr);
	fp->fileExt = getDirEntryFileExt(virDrive, clusterAddr, entryAddr);
	fp->createDate = getDirEntryCreateTimeBytes(virDrive, clusterAddr, entryAddr);
	fp->modifyDate = getDirEntryModifiedTimeBytes(virDrive, clusterAddr, entryAddr);
	fp->firstClusterAddr = getDirEntryStartCluster(virDrive, clusterAddr, entryAddr);
	fp->fileSize = getDirEntryFileSize(virDrive,clusterAddr, entryAddr);

	fp->startLoc = fp->firstClusterAddr * getBytesPerCluster(virDrive);
	fp->currentLoc = fp->firstClusterAddr * getBytesPerCluster(virDrive);
	fp->currentClusterAddr = fp->firstClusterAddr;
	fp->endLoc = fp->firstClusterAddr * getBytesPerCluster(virDrive);
	fp->endClusterAddr = fp->firstClusterAddr;
	fp->numOfClusters = 1;

	return fp;
}

void createDirectory(FILE **virDrive, char *dirName)
{

}

size_t writeFile(FILE **virDrive, void *src, size_t size, size_t len, BC_FILE *dest)
{
	size_t written = 0;
	size_t i;
	for(i = 0; i < len; i++)
	{
		
	}

	return written;
}

size_t readFile(FILE **virDrive, void *dest, size_t size, size_t len, BC_FILE *src)
{
	size_t read = 0;

	return read;
}

int closeFile(FILE **virDrive, BC_FILE *file)
{

	return 0;
}

int deleteFile(FILE **virDrive, BC_FILE *file)
{
	
	return 0;
}