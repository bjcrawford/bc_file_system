/**
 * @file bc_dir_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     The file holds all of the operations which can be performed on
 *     the directory clusters of the virtual drive. Each directory 
 *     entry will consist of 32 bytes. This will allow for 16 entries
 *     per cluster. The layout of the data contained in each directory 
 *     entry is as follows:
 *
 *        (bytes) | value 
 *       ---------------------------------------------------------------
 *        (0)     | A byte holding the attributes of the file/directory:
 *                    Bit 0: 0 for unused entry, 1 for used entry
 *                    Bit 1: 0 for R/O, 1 for R/W
 *                    Bit 2: 1 for hidden file/directory
 *                    Bit 3: 1 for system file
 *                    Bit 4: 1 for subdirectory
 *                    The remaining bits are unused
 *        (1-12)  | The file/directory name (12 chars max)
 *        (13-15) | The file/directory extension (3 chars max)
 *        (16-19) | Creation date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (20-23) | Last modified date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (24-27) | First cluster of file (empty file: 0)
 *        (28-31) | The file size in bytes
 *
 *     The entries will be organized within a directory cluster chain
 *     with entry addresses ranging from 0 upwards. The entry address
 *     refers to an entries position relative to the entire directory
 *     cluster chain.
 *  
 *  This program was written for use in Linux.
*/

#include "bc_dir_operations.h"

/**
 * Returns the starting cluster of the root directory
 *
 * @param  virDrive A pointer to the file pointer of the virtual drive
 * @return          The cluster address of the root diretory
 */
size_t getRootDirectoryCluster(FILE** virDrive)
{
	return getFirstClusterOfRootDir(virDrive);
}

/**
 * Creates a directory entry for a file in the given directory cluster
 *
 * @param  virDrive    A pointer to the file pointer of the virtual drive
 * @param  clusterAddr The starting cluster of the directory
 * @param  attr        The attributes of the file (one byte)
 *                       Bit 0: 0 for unused entry, 1 for used entry
 *                       Bit 1: 0 for R/O, 1 for R/W
 *                       Bit 2: 1 for hidden file/directory
 *                       Bit 3: 1 for system file
 *                       Bit 4: 1 for subdirectory
 *                       The remaining bits are unused
 * @param  name        The name of the file/directory (maximum 12 characters)
 * @param  ext         The extension of the file (maximum 3 characters)          
 */
void createDirFileEntry(FILE **virDrive, size_t clusterAddr,
	                     char attr, char *name, char *ext)
{
	size_t loc = getFirstFreeDirEntry(virDrive, clusterAddr);
	size_t currentTime = encodeTimeBytes();

	/* Set attributes */
	writeNum(virDrive, loc, 1, attr);

	/* Set name */
	writeStr(virDrive, loc + 1, 12, name);

	/* Set extension */
	writeStr(virDrive, loc + 13, 3, ext);

	/* Set create date/time */
	writeNum(virDrive, loc + 16, 4, currentTime);

	/* Set modified date/time */
	writeNum(virDrive, loc + 20, 4, currentTime);

	/* Set first cluster of file (0: empty file) */
	writeNum(virDrive, loc + 24, 4, 0);

	/* Set file size */
	writeNum(virDrive, loc + 28, 4, 0);
}

/**
 * Creates a directory entry for a subdirectory in the given directory cluster
 *
 * @param  virDrive    A pointer to the file pointer of the virtual drive
 * @param  clusterAddr The starting cluster of the parent directory
 * @param  attr        The attributes of the directory (one byte)
 *                       Bit 0: 0 for unused entry, 1 for used entry
 *                       Bit 1: 0 for R/O, 1 for R/W
 *                       Bit 2: 1 for hidden directory
 *                       Bit 3: 1 for system directory
 *                       Bit 4: 1 for subdirectory
 *                       The remaining bits are unused
 * @param  name        The name of the directory (maximum 12 characters)
 * @return             The starting cluster address of the subdirectory
 */
size_t createDirSubEntry(FILE **virDrive, size_t clusterAddr, char attr, char *name)
{
	size_t startCluster = getNextFreeCluster(virDrive);
	setFATEntry(virDrive, startCluster, 0xffffffff);
	findAndSetNextFreeCluster(virDrive);

	size_t loc = getFirstFreeDirEntry(virDrive, clusterAddr);
	size_t currentTime = encodeTimeBytes();

	/* Set attributes */
	writeNum(virDrive, loc, 1, attr);

	/* Set name */
	writeStr(virDrive, loc + 1, 12, name);

	/* Set create date/time */
	writeNum(virDrive, loc + 16, 4, currentTime);

	/* Set modified date/time */
	writeNum(virDrive, loc + 20, 4, currentTime);

	/* Set first cluster of directory */
	writeNum(virDrive, loc + 24, 4, startCluster);

	/* Set file size */
	writeNum(virDrive, loc + 28, 4, 0);

	return startCluster;
}

/**
 * Returns a string containing a listing of the contents of a
 * directory. 
 *
 * @param  virDrive    A pointer to the file pointer of the virtual drive
 * @param  clusterAddr The cluster address of the directory
 * @return             A string containing the directory listing
 */
char *getDirectoryListing(FILE **virDrive, size_t clusterAddr)
{
	char *listing;
	char attr;
	int end = 0;
	size_t count= 0;
	size_t entryAddr = 0;
	size_t currentCluster = clusterAddr;

	/* Determine the number of files/directorys in the directory */
	while(!end)
	{
		attr = getDirEntryAttr(virDrive, clusterAddr, entryAddr);
		if((attr & 0x1) ^ 0x1)
			end = 1;
		else
		{
			entryAddr++;
			count++;
		
			if(entryAddr % 16 == 0)
			{
				currentCluster = getFATEntry(virDrive, currentCluster);
				if(currentCluster == 0xffffffff) /* No more entries to check */
					end = 1;
			}
		}
	}

	/* Allocate space for the listing string */
	listing = calloc(17 * count, sizeof(char));
	strcpy(listing, "");

	/* Reset iteration variables */
	end = 0;
	entryAddr = 0;
	currentCluster = clusterAddr;

	/* Record file info for each directory listing */
	while(!end)
	{
		attr = getDirEntryAttr(virDrive, clusterAddr, entryAddr);
		if((attr & 0x1) ^ 0x1)
			end = 1;
		else
		{
			strcat(listing, getDirEntryFileName(virDrive, clusterAddr, entryAddr));
			if((attr & 0x10) ^ 0x10)
			{
				strcat(listing, ".");
				strcat(listing, getDirEntryFileExt(virDrive, clusterAddr, entryAddr));
			}
			strcat(listing, "\n");
		
			entryAddr++;
			if(entryAddr % 16 == 0)
			{
				currentCluster = getFATEntry(virDrive, currentCluster);
				if(currentCluster == 0xffffffff) /* No more entries to check */
					end = 1;
			}
		}
	}

	return listing;
}

/**
 * Returns the directory cluster address of a directory. Searches through a 
 * given directory for a subdirectory of a given name. Returns if no matching 
 * directory is found.
 *
 * @param  virDrive           A pointer to the file pointer of the virtual drive
 * @param  currentClusterAddr The cluster address of the directory to search
 * @param  dirName            The name of the directory to locate
 * @return                    The cluster address of the directory to locate
 */
size_t getDirectoryClusterAddress(FILE **virDrive, size_t currentClusterAddr, char *dirName)
{
	size_t dirClusterAddress = 0;
	char attr;
	char *name;
	int end = 0;
	int found = 0;
	size_t entryAddr = 0;
	size_t currentCluster = currentClusterAddr;
	size_t nextCluster = 0;

	while(!found && !end)
	{
		attr = getDirEntryAttr(virDrive, currentClusterAddr, entryAddr);
		if(!(attr & 0x1))
			end = 1;
		else if(attr & 0x10) /* Subdirectory */
		{
			name = getDirEntryFileName(virDrive, currentClusterAddr, entryAddr);
			if(strcmp(name, dirName) == 0)
			{
				found = 1;
				dirClusterAddress = getDirEntryStartCluster(virDrive, currentClusterAddr, entryAddr);
			}
		}
		entryAddr++;
	
		if(entryAddr % 16 == 0)
		{
			nextCluster = getFATEntry(virDrive, currentCluster);
			if(nextCluster == 0xffffffff)
				nextCluster = addClusterToChain(virDrive, currentCluster);
			currentCluster = nextCluster;
		}
	}

	return dirClusterAddress;
}

/**
 * Returns 4 bytes containing a time stamp of the current time. The layout
 * of the time stamp is as follows:
 *    Bits 0-5:   Year - 1985
 *    Bits 6-9:   Month
 *    Bits 10-14: Day
 *    Bits 15-19: Hour
 *    Bits 20-25: Minute
 *    Bits 26-31: Second
 *
 * @return A time stamp encoded in 4 bytes
 */
size_t encodeTimeBytes()
{
	size_t timeBytes = 0;

	time_t calTime = time(NULL);
	struct tm *localTime = localtime(&calTime);

	timeBytes ^= ((localTime->tm_year - 85) & 0x3f);
	timeBytes <<= 4;
	timeBytes ^= ((localTime->tm_mon + 1) & 0xf);
	timeBytes <<= 5;
	timeBytes ^= (localTime->tm_mday & 0x1f);
	timeBytes <<= 5;
	timeBytes ^= ((localTime->tm_hour + 1) & 0x1f);
	timeBytes <<= 6;
	timeBytes ^= (localTime->tm_min & 0x3f);
	timeBytes <<= 6;
	timeBytes ^= (localTime->tm_sec & 0x3f);
	
	return timeBytes;
}

/**
 * Returns a pointer to a struct containing the decoded components
 * of a 4 byte time stamp.
 *
 * @param  timeBytes A time stamp encoded in 4 bytes
 * @return           A struct containing the decoded components
 *                   of a calendar time
 */
struct tm *decodeTimeBytes(size_t timeBytes)
{
	struct tm *decodedTime = malloc(sizeof(*decodedTime));

	decodedTime->tm_sec = timeBytes & 0x3f;
	timeBytes >>= 6;
	decodedTime->tm_min = timeBytes & 0x3f;
	timeBytes >>= 6;
	decodedTime->tm_hour = timeBytes & 0x1f;
	timeBytes >>= 5;
	decodedTime->tm_mday = timeBytes & 0x1f;
	timeBytes >>= 5;
	decodedTime->tm_mon = timeBytes & 0xf;
	timeBytes >>= 4;
	decodedTime->tm_year = (timeBytes & 0x3f) + 1985;

	return decodedTime;
}

/**
 * Returns the offset (in bytes) from the beginning of the virtual drive
 * to the start of the first data cluster.
 *
 * @param  virDrive A pointer to the file pointer of the virtual drive
 * @return          The drive offset of the first data cluster in bytes
 */
size_t getDataStartLoc(FILE **virDrive)
{
	size_t loc = 0;
	/* Skip over boot cluster */
	loc += getBytesPerCluster(virDrive) * getNumberOfReservedClusters(virDrive);
	/* Skip over FAT */
	loc += getBytesPerCluster(virDrive) * getNumberOfClustersPerFAT(virDrive);

	return loc;
}

/**
 * Returns the offset (in bytes) from the beginning of the virtual drive
 * to the start of the given directory cluster entry. Returns 0 if the 
 * entry address is invalid.
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The starting cluster address of the directory cluster 
 *                    containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The drive offset of the given entry in bytes, 0 on error
 */
size_t getDirEntryLoc(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t entry = 0;
	size_t loc = 0;
	size_t currentCluster = dirCluster;
	size_t nextCluster = 0;

	/* Skip to the given dir cluster */
	loc += getBytesPerCluster(virDrive) * currentCluster;

	while(entry < entryAddr)
	{
		entry++;
		loc += 32;
		if(entry % 16 == 0)
		{
			nextCluster = getFATEntry(virDrive, currentCluster);
			loc = getBytesPerCluster(virDrive) * nextCluster;
			if(nextCluster == 0xffffffff)
				loc = 0;
			else
				loc = getBytesPerCluster(virDrive) * nextCluster;
			currentCluster = nextCluster;
		}
	}

	return loc;
}

/**
 * Returns the attributes byte of a given directory entry.
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The attributes byte of the entry
 */
char getDirEntryAttr(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return (char) readNum(virDrive, loc, 1);
}

/**
 * Returns the file name of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            A string containing the file name (12 chars max)
 */
char *getDirEntryFileName(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readStr(virDrive, loc + 1, 12);
}

/**
 * Returns the file extension of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            A string containing the file extension (3 chars max)
 */
char *getDirEntryFileExt(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readStr(virDrive, loc + 13, 3);
}

/**
 * Returns the encoded create date/time of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The encoded create date/time stamp (4 bytes)
 */
size_t getDirEntryCreateTimeBytes(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readNum(virDrive, loc + 16, 4);
}

/**
 * Returns the encoded last modified date/time of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The encoded modified date/time stamp (4 bytes)
 */
size_t getDirEntryModifiedTimeBytes(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readNum(virDrive, loc + 20, 4);
}

/**
 * Returns the starting cluster address of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The starting cluster address (4 bytes)
 */
size_t getDirEntryStartCluster(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readNum(virDrive, loc + 24, 4);
}

/**
 * Returns the file size of a given directory entry
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The encoded modified date/time stamp (4 bytes)
 */
size_t getDirEntryFileSize(FILE **virDrive, size_t dirCluster, size_t entryAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);

	return readNum(virDrive, loc + 28, 4);
}

/**
 * Returns the starting location of the first free directory entry for a given
 * directory cluster. If the given directory cluster is full, the directory 
 * cluster chain will be extended.
 *
 * @param  virDrive   A pointer to the file pointer of the virtual drive
 * @param  dirCluster The starting cluster of the directory
 * @return            The drive offset of the first free directory entry in bytes
 */
size_t getFirstFreeDirEntry(FILE **virDrive, size_t dirCluster)
{
	char attr;
	int found = 0;
	size_t loc = 0;
	size_t entryAddr = 0;
	size_t currentCluster = dirCluster;
	size_t nextCluster = 0;
	size_t bytesPerCluster = getBytesPerCluster(virDrive);

	/* Skip to directory cluster */
	loc += bytesPerCluster * dirCluster;

	while(!found)
	{
		attr = getDirEntryAttr(virDrive, dirCluster, entryAddr);
		if((attr & 0x1) ^ 0x1)
			found = 1;
		else
		{
			loc += 32;
			entryAddr++;
		
			if(entryAddr % 16 == 0)
			{
				nextCluster = getFATEntry(virDrive, currentCluster);
				if(nextCluster == 0xffffffff)
					nextCluster = addClusterToChain(virDrive, currentCluster);
				loc = bytesPerCluster * nextCluster;
				currentCluster = nextCluster;
			}
		}
	}

	return loc;
}

/**
 * Sets the attribute byte of a given directory entry
 *
 * @param virDrive   A pointer to the file pointer for the virtual drive
 * @param dirCluster The address of the directory cluster containing the entry
 * @param entryAddr  The address of the entry within the directory cluster
 * @param attr       The attribute byte
 */
void setDirEntryAttr(FILE **virDrive, size_t dirCluster, 
	                 size_t entryAddr, char attr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeNum(virDrive, loc, 1, attr);
}

/**
 * Sets the file name of a given directory entry
 *
 * @param virDrive   A pointer to the file pointer for the virtual drive
 * @param dirCluster The address of the directory cluster containing the entry
 * @param entryAddr  The address of the entry within the directory cluster
 * @param name       A string containing the file name (12 chars max)
 */
void setDirEntryFileName(FILE **virDrive, size_t dirCluster,
	                     size_t entryAddr, char *name)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeStr(virDrive, loc + 1, 12, name);
}

/**
 * Sets the file extension of a given directory entry
 *
 * @param virDrive   A pointer to the file pointer for the virtual drive
 * @param dirCluster The address of the directory cluster containing the entry
 * @param entryAddr  The address of the entry within the directory cluster
 * @param ext        A string containing the file extension (3 chars max)
 */
void setDirEntryFileExt(FILE **virDrive, size_t dirCluster,
	                    size_t entryAddr, char *ext)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeStr(virDrive, loc + 13, 3, ext);
}

/**
 * Sets the modified date/time of a given directory entry
 *
 * @param virDrive   A pointer to the file pointer for the virtual drive
 * @param dirCluster The address of the directory cluster containing the entry
 * @param entryAddr  The address of the entry within the directory cluster
 * @param timeBytes  The encoded modified date/time stamp
 */
void setDirEntryModifiedTimeBytes(FILE **virDrive, size_t dirCluster,
	                              size_t entryAddr, size_t timeBytes)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeNum(virDrive, loc + 20, 4, timeBytes);
}

/**
 * Sets the starting cluster of a given directory entry
 *
 * @param virDrive    A pointer to the file pointer for the virtual drive
 * @param dirCluster  The address of the directory cluster containing the entry
 * @param entryAddr   The address of the entry within the directory cluster
 * @param clusterAddr The starting cluster address
 */
void setDirEntryStartCluster(FILE **virDrive, size_t dirCluster,
	                         size_t entryAddr, size_t clusterAddr)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeNum(virDrive, loc + 24, 4, clusterAddr);
}

/**
 * Sets the file size of a given directory entry
 *
 * @param virDrive   A pointer to the file pointer fo the virtual drive
 * @param dirCluster The address of the directory cluster containing the entry
 * @param entryAddr  The address of the entry within the directory cluster
 * @param timeBytes  The file size
 */
void setDirEntryFileSize(FILE **virDrive, size_t dirCluster,
	                     size_t entryAddr, size_t fileSize)
{
	size_t loc = getDirEntryLoc(virDrive, dirCluster, entryAddr);
	writeNum(virDrive, loc + 28, 4, fileSize);
}

/**
 * Adjusts the file size of a given directory entry
 *
 * @param  virDrive    A pointer to the file pointer for the virtual drive
 * @param  dirCluster  The address of the directory cluster containing the entry
 * @param  entryAddr   The address of the entry within the directory cluster
 * @param  adjFileSize The adjustment to the file size
 * @param  increase    1 to increase by the adjusted size, 0 to decrease
 * @return             1 if successful, 0 if invalid input
 */
size_t adjustDirEntryFileSize(FILE **virDrive, size_t dirCluster,
	                          size_t entryAddr, size_t adjFileSize,
	                          size_t increase)
{
	size_t fileSize = getDirEntryFileSize(virDrive, dirCluster, entryAddr);
	if(increase)
		fileSize += adjFileSize;
	else
	{
		if(adjFileSize > fileSize)
		{
			/* Invalid input */
			return 0;
		}
		else
			fileSize -= adjFileSize;
	}
	setDirEntryFileSize(virDrive, dirCluster, entryAddr, fileSize);

	return 1;
}
