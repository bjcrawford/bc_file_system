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
	fp->filePosition = 0;
	fp->fileSize = getDirEntryFileSize(virDrive,clusterAddr, entryAddr);

	fp->startClusterAddr = getDirEntryStartCluster(virDrive, clusterAddr, entryAddr);
	fp->startLoc = fp->startClusterAddr * getBytesPerCluster(virDrive);
	fp->currentClusterAddr = fp->startClusterAddr;
	fp->currentLoc = fp->startClusterAddr * getBytesPerCluster(virDrive);

	return fp;
}

void createDirectory(FILE **virDrive, char *dirPath)
{
	char *dir = (char*) calloc(13, sizeof(char));
	size_t clusterAddr = getRootDirectoryCluster(virDrive);
	size_t nextClusterAddr;

	if(strchr(dirPath, '/') != NULL)
	{
		char **path = chop(dirPath, '/');
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
		strcpy(dir, p[i]);
	}
	else
		strcpy(dir, dirPath);

	createDirSubEntry(virDrive, clusterAddr, 0x13, dir);
}

void writeFile(FILE **virDrive, void *src, size_t len, BC_FILE *dest)
{
	size_t lenLeft = len;
	size_t bytesLeft;
	
	while(lenLeft > 0)
	{
		bytesLeft = ((dest->currentClusterAddr + 1) * getBytesPerCluster(virDrive)) - dest->currentLoc; 

		if(lenLeft < bytesLeft) /* write will fit in the current cluster with space left over */
		{
			writeStr(virDrive, dest->currentLoc, lenLeft, src);
			dest->currentLoc += lenLeft;
			lenLeft -= lenLeft;
		}
		else if(lenLeft == bytesLeft) /* write will use all remaining space in cluster */
		{
			writeStr(virDrive, dest->currentLoc, lenLeft, src);
			size_t nextClusterAddr = getFATEntry(virDrive, dest->currentClusterAddr);
			if(nextClusterAddr != 0xffffffff)
			{
				dest->currentClusterAddr = nextClusterAddr;
				dest->currentLoc = dest->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			else
			{
				dest->currentClusterAddr = addClusterToChain(virDrive, dest->currentClusterAddr);
				dest->currentLoc = dest->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			lenLeft -= bytesLeft;
		}
		else /* write will span multiple clusters */
		{
			void *srcChunk = calloc(bytesLeft, sizeof(char));
			memcpy(srcChunk, src, bytesLeft);
			writeStr(virDrive, dest->currentLoc, bytesLeft, srcChunk);
			free(srcChunk);
			size_t nextClusterAddr = getFATEntry(virDrive, dest->currentClusterAddr);
			if(nextClusterAddr != 0xffffffff)
			{
				dest->currentClusterAddr = nextClusterAddr;
				dest->currentLoc = dest->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			else
			{
				dest->currentClusterAddr = addClusterToChain(virDrive, dest->currentClusterAddr);
				dest->currentLoc = dest->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			lenLeft -= bytesLeft;
			src += bytesLeft;
		}
	}
	dest->modifyDate = encodeTimeBytes();
	dest->filePosition += len;
	dest->fileSize += len;
}

void readFile(FILE **virDrive, void *dest, size_t size, size_t len, BC_FILE *src)
{
	if(len > (src->fileSize - src->filePosition))
		len = (src->fileSize - src->filePosition);

	size_t lenLeft = len;
	size_t bytesLeft;
	
	while(lenLeft > 0)
	{
		bytesLeft = ((src->currentClusterAddr + 1) * getBytesPerCluster(virDrive)) - src->currentLoc; 

		if(lenLeft < bytesLeft) /* read from current cluster only */
		{
			char *srcChunk = readStr(virDrive, src->currentLoc, lenLeft);
			memcpy(dest, srcChunk, lenLeft);
			free(srcChunk);
			src->currentLoc += lenLeft;
			lenLeft -= lenLeft;
		}
		else if(lenLeft == bytesLeft) /* read entire current cluster, move pointers to next or new cluster */
		{
			char *srcChunk = readStr(virDrive, src->currentLoc, lenLeft);
			memcpy(dest, srcChunk, lenLeft);
			free(srcChunk);

			size_t nextClusterAddr = getFATEntry(virDrive, src->currentClusterAddr);
			if(nextClusterAddr != 0xffffffff)
			{
				src->currentClusterAddr = nextClusterAddr;
				src->currentLoc = src->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			else
			{
				src->currentClusterAddr = addClusterToChain(virDrive, src->currentClusterAddr);
				src->currentLoc = src->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			lenLeft -= bytesLeft;
		}
		else /* write will span multiple clusters */
		{
			void *srcChunk = readStr(virDrive, src->currentLoc, lenLeft);
			memcpy(dest, srcChunk, lenLeft);
			free(srcChunk);

			size_t nextClusterAddr = getFATEntry(virDrive, src->currentClusterAddr);
			if(nextClusterAddr != 0xffffffff)
			{
				src->currentClusterAddr = nextClusterAddr;
				src->currentLoc = src->currentClusterAddr * getBytesPerCluster(virDrive);
			}
			else
			{
				src->currentClusterAddr = addClusterToChain(virDrive, src->currentClusterAddr);
				src->currentLoc = src->currentClusterAddr * getBytesPerCluster(virDrive);
			}

			lenLeft -= bytesLeft;
			dest += bytesLeft;
		}
	}
	src->filePosition += len;
}

int closeFile(FILE **virDrive, BC_FILE *file)
{

	return 0;
}

int deleteFile(FILE **virDrive, BC_FILE *file)
{
	
	return 0;
}