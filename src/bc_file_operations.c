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
 *     This file holds all of the input and output file operations that 
 *     can be performed on the the virtual drive.
*/

#include "bc_file_operations.h"

/**
 * Opens an a file and returns a pointer to a custom file pointer
 * structure. If the file exists, the file is opened with the 
 * location pointer at the beginning of the file. If the file
 * and/or the directories in the given path do not exist, they will
 * be created.
 *
 * @param  filePath A string containing the absolute file path
 * @return          A custom file pointer to the file
 */
BC_FILE *openFile(char *filePath)
{
	BC_FILE *fp = NULL;

	/* Allocate memory for the custom file pointer */
	fp = malloc(sizeof(*fp));
	if(!fp)
	{
		fprintf(stderr, "Error allocating space for BC_FILE\n");
		fprintf(stderr, "Exiting\n");
		return NULL;
	}
	
	char file[FILE_NAME_MAX + FILE_EXT_SIZE + 2];
	char fileName[FILE_NAME_MAX + 1];
	char fileExt[FILE_EXT_SIZE + 1];

	/* Declare variables for cluster and directory navigation */
	u_int clusterAddr = bootRecord->rootDirStart;
	u_int nextClusterAddr;
	u_int entryAddr;
	DirEntry *entry;

	/* If the file to open is not in the root directory,
	   parse filePath to locate the file's parent directory */
	if(strchr(filePath, '/') != NULL)
	{
		/* Split the absolute file path into separate directories */
		char **path = chop(filePath, '/');

		/* Locate the file name and extension */
		char **p = path;
		int i = 0;
		while(p[i+1] != NULL)
			i++;
		strcpy(file, p[i]);

		/* Parse the file name and extension */
		strcpy(fileName, strtok(file, "."));
		strcpy(fileExt, strtok(NULL, "."));

		/* Check file name and extension lengths */
		if(strlen(fileName) < FILE_NAME_MIN || strlen(fileExt) > FILE_NAME_MAX)
		{
			fprintf(stderr, "Could not create file: ");
			fprintf(stderr, "invalid file name length\n");
			fprintf(stderr, "File name must be between ");
			fprintf(stderr, "%d and %d characters in length\n", FILE_NAME_MIN, FILE_NAME_MAX);
			free(fp);

			return NULL;
		}
		else if(strlen(fileExt) != FILE_EXT_SIZE)
		{
			fprintf(stderr, "Could not create file: ");
			fprintf(stderr, "invalid file extension length\n");
			fprintf(stderr, "File extension must be ");
			fprintf(stderr, "%d characters in length\n", FILE_EXT_SIZE);
			free(fp);

			return NULL;
		}

		/* Traverse the directories to locate the parent directory 
		   NOTE: The last p[i] will be the file name and extension
		   of the absolute file path given */
		p = path;
		i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			nextClusterAddr = getDirectoryClusterAddress(clusterAddr, p[i]);
			if(nextClusterAddr == 0) /* If directory is not found, create */
			{
				u_int nextClusterEntryAddr = createDirSubEntry(clusterAddr, 0x13, p[i]);
				entry = getDirEntry(clusterAddr, nextClusterEntryAddr);
				nextClusterAddr = entry->startCluster;
				free(entry);
			}
			clusterAddr = nextClusterAddr;
			free(p[i]);
			i++;
		}
		free(p[i]);
		free(path);
	}
	else /* The file to open is in the root directory */
	{
		strcpy(file, filePath);

		/* Parse the file name and extension */
		strcpy(fileName, strtok(file, "."));
		strcpy(fileExt, strtok(NULL, "."));

		/* Check file name and extension lengths */
		if(strlen(fileName) < FILE_NAME_MIN || strlen(fileExt) > FILE_NAME_MAX)
		{
			fprintf(stderr, "Could not create file: ");
			fprintf(stderr, "invalid file name length\n");
			fprintf(stderr, "File name must be between ");
			fprintf(stderr, "%d and %d characters in length\n", FILE_NAME_MIN, FILE_NAME_MAX);
			free(fp);

			return NULL;
		}
		else if(strlen(fileExt) != FILE_EXT_SIZE)
		{
			fprintf(stderr, "Could not create file: ");
			fprintf(stderr, "invalid file extension length\n");
			fprintf(stderr, "File extension must be ");
			fprintf(stderr, "%d characters in length\n", FILE_EXT_SIZE);
			free(fp);
			
			return NULL;
		}

	}

	/* Determine if file exists */
	/* If so, get entry address from current directory cluster */
	if(dirFileEntryExists(clusterAddr, fileName, fileExt))
		entryAddr = getDirFileEntryAddr(clusterAddr, fileName, fileExt);
	/* If not, create file and record entry address */
	else 	
		entryAddr = createDirFileEntry(clusterAddr, 0x3, fileName, fileExt);

	/* NOTE: This function could be modified to take a "mode" and to set the 
	         file's attributes accordingly */
	
	/* Set the properties of the file pointer using the metadata located 
	   in the file's directory entry */
	entry = getDirEntry(clusterAddr, entryAddr);
	fp->used = entry->attr & 0x1;
	fp->write = entry->attr & 0x2;
	fp->hidden = entry->attr & 0x4;
	fp->subDir = entry->attr & 0x8;
	strncpy(fp->fileName, entry->fileName, FILE_NAME_MAX);
	strncpy(fp->fileExt, entry->fileExt, FILE_EXT_SIZE);
	fp->createDate = entry->createDate;
	fp->modifyDate = entry->modifiedDate;
	fp->filePosition = 0;
	fp->fileSize = entry->fileSize;
	fp->startClusterAddr = entry->startCluster;
	fp->startLoc = fp->startClusterAddr * bootRecord->bytesPerCluster;
	fp->currentClusterAddr = fp->startClusterAddr;
	fp->currentLoc = fp->startClusterAddr * bootRecord->bytesPerCluster;
	fp->dirClusterAddr = clusterAddr;
	fp->dirEntryAddr = entryAddr;
	free(entry);

	return fp;
}

/**
 * Creates a directory. If any directories in the absolute path do not
 * exist, they will be created.
 *
 * @param dirPath  A string containing the absolute directory path
 */
void createDirectory(char *dirPath)
{
	/* Allocate memory for a string to parse the directory path */
	char *dir = (char*) calloc(13, sizeof(char));

	/* Declare variables for cluster navigation */
	u_int clusterAddr = bootRecord->rootDirStart;
	u_int nextClusterAddr;
	DirEntry *entry;

	/* If the directory to create is not in the root directory,
	   parse dirPath to locate the directory's parent directory */
	if(strchr(dirPath, '/') != NULL)
	{
		/* Split the absolute directory path into separate directories */
		char **path = chop(dirPath, '/');

		/* Locate the directory name */
		char **p = path;
		int i = 0;
		while(p[i+1] != NULL)
			i++;
		strcpy(dir, p[i]);

		/* Check directory name length */
		if(strlen(dir) < FILE_NAME_MIN || strlen(dir) > FILE_NAME_MAX)
		{
			fprintf(stderr, "Could not create directory: ");
			fprintf(stderr, "invalid directory name length\n");
			fprintf(stderr, "Directory name must be between ");
			fprintf(stderr, "%d and %d characters in length\n", FILE_NAME_MIN, FILE_NAME_MAX);

			return;
		}

		/* Traverse the directories to locate the parent directory 
		   NOTE: The last p[i] will be the file name and extension
		   of the absolute file path given */
		p = path;
		i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			nextClusterAddr = getDirectoryClusterAddress(clusterAddr, p[i]);
			if(nextClusterAddr == 0) /* If directory is not found, create it */
			{
				u_int nextClusterEntryAddr = createDirSubEntry(clusterAddr, 0x13, p[i]);
				entry = getDirEntry(clusterAddr, nextClusterEntryAddr);
				nextClusterAddr = entry->startCluster;
				free(entry);
			}
			clusterAddr = nextClusterAddr;
			i++;
		}
	}
	else /* The directory to open is in the root directory */
	{
		strcpy(dir, dirPath);

		/* Check directory name length */
		if(strlen(dir) < FILE_NAME_MIN || strlen(dir) > FILE_NAME_MAX)
		{
			fprintf(stderr, "Could not create directory: ");
			fprintf(stderr, "invalid directory name length\n");
			fprintf(stderr, "Directory name must be between ");
			fprintf(stderr, "%d and %d characters in length\n", FILE_NAME_MIN, FILE_NAME_MAX);

			return;
		}
	}

	/* Determine if directory exists */
	/* If not, create the directory */
	if(!dirFileEntryExists(clusterAddr, dir, ""))
		createDirSubEntry(clusterAddr, 0x13, dir);

	/* NOTE: This function could be modified to take a "mode" and to set the 
	         directory's attributes accordingly */
}

void writeFile(void *src, u_int len, BC_FILE *dest)
{
	u_int lenLeft = len;
	u_int bytesLeft;
	DirEntry *entry;

	if(dest->filePosition + len >= FILE_SIZE_MAX)
	{
		fprintf(stderr, "Write unsuccessful: ");
		fprintf(stderr, "write length exceeds max file size of %d bytes\n", FILE_SIZE_MAX);
	}
	else
	{
		while(lenLeft > 0)
		{
			bytesLeft = ((dest->currentClusterAddr + 1) * bootRecord->bytesPerCluster) - dest->currentLoc; 

			if(lenLeft < bytesLeft) /* write to current cluster only */
			{
				writeStr(dest->currentLoc, lenLeft, src);
				dest->currentLoc += lenLeft;
				lenLeft -= lenLeft;
			}
			else /* write may span multiple clusters */
			{
				void *srcChunk = calloc(bytesLeft, sizeof(char));
				memcpy(srcChunk, src, bytesLeft);
				writeStr(dest->currentLoc, bytesLeft, srcChunk);
				free(srcChunk);
				u_int nextClusterAddr = fileAllocTable[dest->currentClusterAddr];
				if(nextClusterAddr != 0xffffffff)
				{
					dest->currentClusterAddr = nextClusterAddr;
					dest->currentLoc = dest->currentClusterAddr * bootRecord->bytesPerCluster;
				}
				else
				{
					dest->currentClusterAddr = addClusterToChain(dest->currentClusterAddr);
					dest->currentLoc = dest->currentClusterAddr * bootRecord->bytesPerCluster;
				}
				lenLeft -= bytesLeft;
				src += bytesLeft;
			}
		}
		dest->modifyDate = encodeTimeBytes();
		dest->filePosition += len;
		dest->fileSize += len;
		entry = getDirEntry(dest->dirClusterAddr, dest->dirEntryAddr);
		entry->modifiedDate = dest->modifyDate;
		entry->fileSize = dest->fileSize;
		setDirEntry(dest->dirClusterAddr, dest->dirEntryAddr, entry);
		free(entry);
	}
}

void readFile(void *dest, u_int size, u_int len, BC_FILE *src)
{
	/* If length of read exceeds the remaining length of the file,
	   set the length of read to the remaining length of the file */
	if(len > (src->fileSize - src->filePosition))
		len = (src->fileSize - src->filePosition);

	u_int lenLeft = len;
	u_int bytesLeft;
	
	while(lenLeft > 0)
	{
		bytesLeft = ((src->currentClusterAddr + 1) * bootRecord->bytesPerCluster) - src->currentLoc; 

		char *srcChunk = readStr(src->currentLoc, lenLeft);
		memcpy(dest, srcChunk, lenLeft);
		free(srcChunk);

		if(lenLeft < bytesLeft) /* read from current cluster only */
		{
			src->currentLoc += lenLeft;
			lenLeft -= lenLeft;
		}
		else /* read may span multiple clusters */
		{
			u_int nextClusterAddr = fileAllocTable[src->currentClusterAddr];
			if(nextClusterAddr != 0xffffffff) 
			{
				src->currentClusterAddr = nextClusterAddr;
				src->currentLoc = src->currentClusterAddr * bootRecord->bytesPerCluster;
			}
			else
			{
				src->currentClusterAddr = addClusterToChain(src->currentClusterAddr);
				src->currentLoc = src->currentClusterAddr * bootRecord->bytesPerCluster;
			}
			lenLeft -= bytesLeft;
			dest += bytesLeft;
		}
	}
	src->filePosition += len;
}

void closeFile(BC_FILE *file)
{
	destroyBC_File(file);
}

void deleteFile(BC_FILE *file)
{
	
}
