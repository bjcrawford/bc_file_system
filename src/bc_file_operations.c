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
/**
 * Opens an a file and returns a pointer to a custom file pointer
 * structure. If the file exists, the file is opened with the 
 * location pointer at the beginning of the file. If the file
 * and/or the directories in the given path do not exist, they will
 * be created.
 *
 * @param  virDrive A pointer to the file pointer of the virtual drive
 * @param  filePath A string containing the absolute file path
 * @return          A custom file pointer to the file
 */
BC_FILE *openFile(FILE **virDrive, char *filePath)
{
	/* Allocate memory for the custom file pointer */
	BC_FILE *fp = malloc(sizeof(*fp));
	if(!fp)
	{
		fprintf(stderr, "Error allocating space for BC_FILE\n");
		fprintf(stderr, "Exiting\n");
		exit(1);
	}
	
	/* Allocate memory for strings to parse filePath */
	char *file = (char*) calloc(17, sizeof(char));
	char *fileName = (char*) calloc(13, sizeof(char));
	char *fileExt = (char*) calloc(4, sizeof(char));

	/* Declare variables for cluster and directory navigation */
	size_t clusterAddr = getRootDirectoryCluster(virDrive);
	size_t nextClusterAddr;
	size_t entryAddr;

	/* If the file to open is not in the root directory,
	   parse filePath to locate the file's parent directory */
	if(strchr(filePath, '/') != NULL)
	{
		/* Split the absolute file path into separate directories */
		char **path = chop(filePath, '/');

		/* Traverse the directories to locate the parent directory 
		   NOTE: The last p[i] will be the file name and extension
		   of the absolute file path given */
		char **p = path;
		int i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			nextClusterAddr = getDirectoryClusterAddress(virDrive, clusterAddr, p[i]);
			if(nextClusterAddr == 0) /* If directory is not found, create */
			{
				size_t nextClusterEntryAddr = createDirSubEntry(virDrive, clusterAddr, 0x13, p[i]);
				nextClusterAddr = getDirEntryStartCluster(virDrive, clusterAddr, nextClusterEntryAddr);
			}
			clusterAddr = nextClusterAddr;
			free(p[i]);
			i++;
		}
		strcpy(file, p[i]); 
		free(p[i]);
		free(path);
	}
	else /* The file to open is in the root directory */
		strcpy(file, filePath);

	/* Parse the file name and extension */
	strcpy(fileName, strtok(file, "."));
	strcpy(fileExt, strtok(NULL, "."));

	/* Determine if file exists */
	/* If so, get entry address from current directory cluster */
	if(dirFileEntryExists(virDrive, clusterAddr, fileName, fileExt))
		entryAddr = getDirFileEntryAddr(virDrive, clusterAddr, fileName, fileExt);
	/* If not, create file and record entry address */
	else 	
		entryAddr = createDirFileEntry(virDrive, clusterAddr, 0x3, fileName, fileExt);

	/* NOTE: This function could be modified to take a "mode" and to set the 
	         file's attributes accordingly */
	
	/* Set the properties of the file pointer using the metadata located 
	   in the file's directory entry */
	char attr = getDirEntryAttr(virDrive, clusterAddr, entryAddr);
	fp->used = attr & 0x1;
	fp->write = attr & 0x2;
	fp->hidden = attr & 0x4;
	fp->subDir = attr & 0x8;
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

	fp->dirClusterAddr = clusterAddr;
	fp->dirEntryAddr = entryAddr;

	free(file);
	free(fileName);
	free(fileExt);

	return fp;
}

/**
 * Creates a directory. If any directories in the absolute path do not
 * exist, they will be created.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 * @param dirPath  A string containing the absolute folder path
 */
void createDirectory(FILE **virDrive, char *dirPath)
{
	/* Allocate memory for a string to parse the directory path */
	char *dir = (char*) calloc(13, sizeof(char));

	/* Declare variables for cluster navigation */
	size_t clusterAddr = getRootDirectoryCluster(virDrive);
	size_t nextClusterAddr;

	/* If the directory to create is not in the root directory,
	   parse dirPath to locate the directory's parent directory */
	if(strchr(dirPath, '/') != NULL)
	{
		/* Split the absolute directory path into separate directories */
		char **path = chop(dirPath, '/');

		/* Traverse the directories to locate the parent directory 
		   NOTE: The last p[i] will be the file name and extension
		   of the absolute file path given */
		char **p = path;
		int i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			nextClusterAddr = getDirectoryClusterAddress(virDrive, clusterAddr, p[i]);
			if(nextClusterAddr == 0) /* If directory is not found, create it */
			{
				size_t nextClusterEntryAddr = createDirSubEntry(virDrive, clusterAddr, 0x13, p[i]);
				nextClusterAddr = getDirEntryStartCluster(virDrive, clusterAddr, nextClusterEntryAddr);
			}
			clusterAddr = nextClusterAddr;
			i++;
		}
		strcpy(dir, p[i]);
	}
	else /* The directory to open is in the root directory */
		strcpy(dir, dirPath);


	/* NOTE: This function could be modified to take a "mode" and to set the 
	         directory's attributes accordingly */
	createDirSubEntry(virDrive, clusterAddr, 0x13, dir);
}

void writeFile(FILE **virDrive, void *src, size_t len, BC_FILE *dest)
{
	size_t lenLeft = len;
	size_t bytesLeft;
	
	while(lenLeft > 0)
	{
		bytesLeft = ((dest->currentClusterAddr + 1) * getBytesPerCluster(virDrive)) - dest->currentLoc; 

		if(lenLeft < bytesLeft) /* write to current cluster only */
		{
			writeStr(virDrive, dest->currentLoc, lenLeft, src);
			dest->currentLoc += lenLeft;
			lenLeft -= lenLeft;
		}
		else /* write may span multiple clusters */
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
	setDirEntryModifiedTimeBytes(virDrive, dest->dirClusterAddr, dest->dirEntryAddr, dest->modifyDate);
	dest->filePosition += len;
	dest->fileSize += len;
	setDirEntryFileSize(virDrive, dest->dirClusterAddr, dest->dirEntryAddr, dest->fileSize);
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

		char *srcChunk = readStr(virDrive, src->currentLoc, lenLeft);
		memcpy(dest, srcChunk, lenLeft);
		free(srcChunk);

		if(lenLeft < bytesLeft) /* read from current cluster only */
		{
			src->currentLoc += lenLeft;
			lenLeft -= lenLeft;
		}
		else /* read may span multiple clusters */
		{
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
