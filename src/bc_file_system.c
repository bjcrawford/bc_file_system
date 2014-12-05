/**
 * @file bc_file_system.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
 *     This file contains the implementation for the file system that
 *     I have created. My file system has been implemented using a design
 *     which closely resembles the FAT32 scheme, but with some minor 
 *     adjustments to account for the fact that a virtual representation
 *     of a drive is being used.
 */

#include "bc_file_system.h"

/** 
 * ======================================================================== 
 * |                       File System Operations                         | 
 * ======================================================================== 
 *
 */

/** 
 * Initializes the file system. If the given virtual drive has 
 * previously been initialized, the drive will be opened and
 * ready for use and the virtual drive label parameter will
 * not be used. If the given virtual drive has not been 
 * previously initialized, the virtual drive will be
 * initialized and use the given virtual drive label.
 * 
 *
 * @param virDriveName  The file name of the virtual drive
 * @param virDriveLabel The label to give the virtual drive
 */
void initFileSystem(char *virDriveName, char *virDriveLabel)
{
	virDrive = openVirDrive(virDriveName);

	/* Check if the drive has previously been initialized */
	rewind(virDrive);
	char init = getc(virDrive);
	if(init)
	{
		fprintf(stdout, "\nVirtual drive has previously been initialized.\n");
		fprintf(stdout, "Loading virtual drive properties.\n");
		bootRecord = calloc(1, sizeof(*bootRecord));
		readBootRecord();
		fileAllocTable = (u_int*) calloc(sizeof(u_int), bootRecord->clustersOnDrive);
		readFAT();
	}
	else
	{
		fprintf(stdout, "\nVirtual drive has not previously been initialized.\n");
		fprintf(stdout, "Initializing virtual drive properties.\n");
		formatVirDrive();
		bootRecord = initBootRecord(virDriveLabel);
		writeBootRecord();
		fileAllocTable = initFATClusters();
		writeFAT();
	}
}

void closeFileSystem()
{
	writeBootRecord();
	writeFAT();
	closeVirDrive();
}

/** 
 * ======================================================================== 
 * |                      Virtual Drive Operations                        | 
 * ======================================================================== 
 * 
 *     This section holds all of the operations which can be performed on
 *     the virtual drive. The virtual drive will be represented by a 
 *     file (one of the four files provided: Drive10MB, Drive5MB, Drive3MB, 
 *     or Drive2MB). The virtual drive's allocation unit size, or cluster 
 *     size (as it will be refered to throughout these docs) will be 512 
 *     bytes. The layout of the clusters on the virtual drive will be as 
 *     follows (cluster addresses):
 *
 *       -   0: The boot cluster
 *       -   1: The first cluster of the file allocation table
 *       -   2: The second cluster of the file allocation table
 *         ...
 *       -   n: The last cluster of the file allocation table
 *              Note: n = (((drive size / cluster size) * FAT entry size) / cluster size) + 1
 *       - n+1: The root directory cluster
 *       - n+2: The first cluster of the data region
 *         ...
 *       -   x: The last cluster of the data region
 *              Note: x = (drive size / cluster size) - 1
 */

/**
 * Opens the virtual drive using the given file name
 *
 * @param  virDriveName A string containing the name of the virtual drive (file)
 *                      to open
 * @return              A pointer to the file pointer of the virtual drive
*/
FILE *openVirDrive(char *virDriveName)
{
	return fopen(virDriveName, "r+");
}

/** 
 * Formats the virtual drive. 
 *
*/
void formatVirDrive()
{
	rewind(virDrive);
	int i;
	u_int driveSize = 0;
	while(fgetc(virDrive) != EOF)
		driveSize++;
	rewind(virDrive);
	for(i = 0; i < driveSize; i++)
		fputc(0x00, virDrive);
	rewind(virDrive);
}

/**
 * Closes the virtual drive
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
*/
void closeVirDrive()
{
	fclose(virDrive);
}

/**
 * Formats the given cluster on the virtual drive
 *
 * @param clusterAddr The address of the cluster to format
 */
void formatCluster(u_int clusterAddr)
{
	u_int i;
	u_int loc = clusterAddr * bootRecord->bytesPerCluster;
	fseek(virDrive, loc, SEEK_SET);
	for(i = 0; i < bootRecord->bytesPerCluster; i++)
		fputc(0x00, virDrive);
}

/** 
 * ======================================================================== 
 * |                    Boot Record Operations                            | 
 * ======================================================================== 
 * 
 *     This section holds all of the operations which can be performed on 
 *     the boot record of the virtual drive. The boot cluster will have 
 *     a size of one cluster (512 bytes). The layout of the data contained 
 *     in the boot record/cluster is as follows:
 *     
 *        (bytes) | property 
 *       ---------------------------------------------------------------
 *        (0)      | A byte designating the virtual drive as initialized
 *        (1-23)   | A label for the virtual drive
 *        (24-27)  | The number of bytes per cluster
 *        (28-31)  | The number of reserved clusters
 *        (32-35)  | The number of clusters on the virtual drive
 *        (36-39)  | The number of clusters per file allocation table
 *        (40-43)  | The first cluster of the root directory
 *        (44-47)  | The number of free clusters
 *        (48-51)  | The next free cluster
 *        (52-55)  | The size of the virtual drive in bytes
 *        (55-511) | This space is unused
 *
 *     The boot record will be represented in the file system application 
 *     as a struct containing all of the properties listed above. 
 */


/**
 * Initializes a boot record struct for the virtual drive.
 * NOTE: This function does not write the boot record struct to
 * the virtual drive.
 *
 * @param  driveLabel A string containing the label for the drive
 * @return            An pointer to an initialized boot record struct
 */
BootRecord *initBootRecord(char *driveLabel)
{
	BootRecord *boot = calloc(1, sizeof(*boot));

	/* Get size of the drive in bytes */
	rewind(virDrive);
	u_int dSize = 0;
	while(fgetc(virDrive) != EOF)
		dSize++;
	rewind(virDrive);

	boot->init = 1;
	strncpy(boot->label, driveLabel, DRIVE_LABEL_MAX);
	boot->bytesPerCluster = CLUSTER_SIZE;
	boot->reservedClusters = 1;
	boot->clustersOnDrive = dSize / CLUSTER_SIZE;
	boot->clustersPerFat = ceil((double) boot->clustersOnDrive * FAT_ENTRY_BYTES / CLUSTER_SIZE);
	boot->rootDirStart = boot->clustersPerFat + 1;
	boot->freeClusters = boot->clustersOnDrive - (boot->clustersPerFat + 2);
	boot->nextFreeCluster = boot->clustersPerFat + 2;
	boot->driveSize = dSize;

	return boot;
}

/**
 * Writes the global boot record struct to the virtual drive
 */
void writeBootRecord()
{
	rewind(virDrive);
	fwrite(bootRecord, sizeof(BootRecord), 1, virDrive);
}

/**
 * Reads the boot record from the drive and stores it in the
 * global boot record struct
 */
void readBootRecord()
{
	rewind(virDrive);
	fread(bootRecord, sizeof(BootRecord), 1, virDrive);
}

/** 
 * ======================================================================== 
 * |               File Allocation Table Operations                       | 
 * ======================================================================== 
 *
 *     This section holds all of the operations that can be performed on
 *     the file allocation table clusters on the virtual drive. Each 
 *     file allocation table entry will consist of 4 bytes. This will
 *     allow for 128 entries per cluster. Each file allocation table
 *     entry can hold one of three types of values:
 *     
 *       - The address of the next cluster in the chain
 *       - The end of chain identifier 0xffffffff
 *       - The empty cluster identifier 0x0
 *       
 *     File allocation table cluster addresses (clusterAddr) refer to the 
 *     position of the cluster on disk. The cluster addresses are defined 
 *     as follows:
 *     
 *       -   0: The boot cluster
 *       -   1: The first cluster of the file allocation table
 *       -   2: The second cluster of the file allocation table
 *         ...
 *       -   n: The last cluster of the file allocation table
 *              Note: n = (((drive size / cluster size) * entry size) / cluster size) + 1
 *       - n+1: The root directory cluster
 *       - n+2: The first cluster of the data region
 *         ...
 *       -   x: The last cluster of the data region
 *              Note: x = (drive size / cluster size) - 1
 */

/**
 * Initializes the file allocation table clusters of a virtual drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
u_int *initFATClusters()
{
	int i;
	int n = bootRecord->clustersPerFat;
	u_int *fat = (u_int*) calloc(sizeof(u_int), bootRecord->clustersOnDrive);
	
	/* Boot Cluster */
	fat[0] = 0xffffffff;
	/* FAT Clusters */
	for(i = 1; i < n; i++)
		fat[i] = i + 1;
	fat[n] = 0xffffffff;
	/* Root Dir Cluster */
	fat[bootRecord->rootDirStart] = 0xffffffff;

	return fat;
}

void writeFAT()
{
	u_int loc = bootRecord->bytesPerCluster * bootRecord->reservedClusters;
	fseek(virDrive, loc, SEEK_SET);
	fwrite(fileAllocTable, sizeof(u_int), bootRecord->clustersOnDrive, virDrive);
}

void readFAT()
{
	u_int loc = bootRecord->bytesPerCluster * bootRecord->reservedClusters;
	fseek(virDrive, loc, SEEK_SET);
	fread(fileAllocTable, sizeof(u_int), bootRecord->clustersOnDrive, virDrive);
}

/**
 * Adds a cluster to the end of a cluster chain. The cluster address 
 * that is passed in must be the ending cluster of the chain.
 *
 * @param  clusterAddr The cluster address of the entry that is being extended
 * @return             The new end of chain cluster address
 */
u_int addClusterToChain(u_int clusterAddr)
{
	u_int next = bootRecord->nextFreeCluster;
	fileAllocTable[clusterAddr] = next;
	fileAllocTable[next] = 0xffffffff;
	findAndSetNextFreeCluster();
	bootRecord->freeClusters--;

	return next;
}

/**
 * Locates the next free cluster using the file allocation table and
 * stores the address in the boot cluster of the drive.
 *
 * @param virDrive A pointer to the file pointer of the virtual drive
 */
void findAndSetNextFreeCluster()
{
	u_int clusterAddr = bootRecord->rootDirStart + 1;
	u_int entriesPerCluster = bootRecord->bytesPerCluster / 4;
	u_int maxEntries = bootRecord->clustersPerFat * entriesPerCluster; 
	while(fileAllocTable[clusterAddr] != 0x0)
	{
		if(clusterAddr >= maxEntries)
		{
			/* FAT is full, handle error */
			fprintf(stderr, "FAT is full\n");
		}
		clusterAddr++;
	} 
	bootRecord->nextFreeCluster = clusterAddr;
}


/** 
 * ======================================================================== 
 * |              Directory Table Entry Operations                        | 
 * ======================================================================== 
 *
 *     The section holds all of the operations which can be performed on
 *     the directory clusters of the virtual drive. Each directory 
 *     entry will consist of 64 bytes. This will allow for 8 entries
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
 *        (1-43)  | The file/directory name (42 chars max)
 *        (44-47) | The file/directory extension (3 chars max)
 *        (48-51) | Creation date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (52-55) | Last modified date/time
 *                    Bits 0-5:   Year - 1985
 *                    Bits 6-9:   Month
 *                    Bits 10-14: Day
 *                    Bits 15-19: Hour
 *                    Bits 20-25: Minute
 *                    Bits 26-31: Second
 *        (56-59) | Starting cluster of file (empty file: 0)
 *        (60-63) | The file size in bytes
 *
 *     The entries will be organized within a directory cluster chain
 *     with entry addresses ranging from 0 upwards. The entry address
 *     refers to an entries position relative to the entire directory
 *     cluster chain.
 */


/**
 * Creates a directory entry for a file in the given directory cluster
 *
 * @param  clusterAddr The starting cluster of the directory
 * @param  attr        The attributes of the file (one byte)
 *                       Bit 0: 0 for unused entry, 1 for used entry
 *                       Bit 1: 0 for R/O, 1 for R/W
 *                       Bit 2: 1 for hidden file
 *                       Bit 3: 1 for system file
 *                       Bit 4: 1 for subdirectory
 *                       The remaining bits are unused
 * @param  name        The name of the file/directory (maximum 12 characters)
 * @param  ext         The extension of the file (maximum 3 characters)
 * @return             The entry address of the new file
 */
u_int createDirFileEntry(u_int clusterAddr, char attr, char *name, char *ext)
{
	u_int startCluster = bootRecord->nextFreeCluster;
	fileAllocTable[startCluster] = 0xffffffff;
	findAndSetNextFreeCluster(virDrive);
	bootRecord->freeClusters--;

	u_int entryAddr = getFirstFreeDirEntryAddr(clusterAddr);
	u_int loc = getDirEntryLoc(clusterAddr, entryAddr);
	u_int currentTime = encodeTimeBytes();
	
	DirEntry fileEntry;
	fileEntry.attr = attr;
	strncpy(fileEntry.fileName, name, FILE_NAME_MAX);
	strncpy(fileEntry.fileExt, ext, FILE_EXT_SIZE);
	fileEntry.createDate = currentTime;
	fileEntry.modifiedDate = currentTime;
	fileEntry.startCluster = startCluster;
	fileEntry.fileSize = 0;
	fseek(virDrive, loc, SEEK_SET);
	fwrite(&fileEntry, sizeof(DirEntry), 1, virDrive);

	return entryAddr;
}

/**
 * Creates a directory entry for a subdirectory in the given directory cluster
 *
 * @param  clusterAddr The starting cluster of the parent directory
 * @param  attr        The attributes of the directory (one byte)
 *                       Bit 0: 0 for unused entry, 1 for used entry
 *                       Bit 1: 0 for R/O, 1 for R/W
 *                       Bit 2: 1 for hidden directory
 *                       Bit 3: 1 for system directory
 *                       Bit 4: 1 for subdirectory
 *                       The remaining bits are unused
 * @param  name        The name of the directory (maximum 42 characters)
 * @return             The starting cluster address of the subdirectory
 */
u_int createDirSubEntry(u_int clusterAddr, char attr, char *name)
{
	u_int startCluster = bootRecord->nextFreeCluster;
	fileAllocTable[startCluster] = 0xffffffff;
	findAndSetNextFreeCluster(virDrive);
	bootRecord->freeClusters--;

	u_int entryAddr = getFirstFreeDirEntryAddr(clusterAddr);
	u_int loc = getDirEntryLoc(clusterAddr, entryAddr);
	u_int currentTime = encodeTimeBytes();

	DirEntry subEntry;
	subEntry.attr = attr;
	strncpy(subEntry.fileName, name, FILE_NAME_MAX);
	subEntry.createDate = currentTime;
	subEntry.modifiedDate = currentTime;
	subEntry.startCluster = startCluster;
	subEntry.fileSize = 0;
	fseek(virDrive, loc, SEEK_SET);
	fwrite(&subEntry, sizeof(DirEntry), 1, virDrive);

	return entryAddr;
}

/**
 * Deletes a directory entry
 *
 * @param dirCluster The starting cluster of the directory
 * @param entryAddr  The address of the entry to delete
 */
void deleteDirEntry(u_int dirCluster, u_int entryAddr)
{
	u_int loc = getDirEntryLoc(dirCluster, entryAddr);
	fseek(virDrive, loc, SEEK_SET);
	int i;
	for(i = 0; i < DIR_ENTRY_BYTES; i++)
		fputc(0x00, virDrive);
}

/**
 * Determines if a file exists in the given directory cluster.
 *
 * @param  clusterAddr The directory cluster to search
 * @param  fileName    The name of the file to locate
 * @param  fileExt     The extension of the file to locate
 * @return             1 if found, 0 otherwise
 */
u_int dirFileEntryExists(u_int clusterAddr, char *fileName, char *fileExt)
{
	u_int found = 0;
	u_int end = 0;
	u_int entryAddr = 0;
	u_int currentCluster = clusterAddr;
	u_int nextCluster = 0;
	DirEntry *entry;

	while(!found && !end)
	{
		entry = getDirEntry(clusterAddr, entryAddr);
		if(strcmp(entry->fileName, fileName) == 0 && strcmp(entry->fileExt, fileExt) == 0)
			found = 1;
		else
		{
			entryAddr++;
			if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
			{
				nextCluster = fileAllocTable[currentCluster];
				if(nextCluster == 0xffffffff)
					end = 1;
				currentCluster = nextCluster;
			}
		}
		free(entry);
	}

	return found;
}

/**
 * Returns the directory entry address of a file in the given directory cluster.
 *
 * @param  clusterAddr The directory cluster to search
 * @param  fileName    The name of the file to locate
 * @param  fileExt     The extension of the file to locate
 * @return             The file's directory entry address
 */
u_int getDirFileEntryAddr(u_int clusterAddr, char *fileName, char *fileExt)
{
	u_int found = 0;
	u_int end = 0;
	u_int entryAddr = 0;
	u_int currentCluster = clusterAddr;
	u_int nextCluster = 0;
	DirEntry *entry;

	while(!found)
	{
		entry = getDirEntry(clusterAddr, entryAddr);
		if(strcmp(entry->fileName, fileName) == 0 && strcmp(entry->fileExt, fileExt) == 0)
			found = 1;
		else
		{
			entryAddr++;
			if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
			{
				nextCluster = fileAllocTable[currentCluster];
				if(nextCluster == 0xffffffff)
					end = 1;
				currentCluster = nextCluster;
			}
		}
		free(entry);
	}

	if(end)
	{
		fprintf(stderr, "Could not locate file in getDirFileEntryAddr()\n");
		fprintf(stderr, "Call dirFileEntryExists() beforehand to ensure file existence\n");
		fprintf(stderr, "Exit\n");
		exit(1);
	}

	return entryAddr;
}

/**
 * Returns a string containing a listing of the contents of a
 * directory. 
 *
 * @param  dirPath     The absolute path of the directory
 * @return             A string containing the directory listing
 */
char *getDirectoryListing(char *dirPath)
{
	char *listing;
	char *dir = (char*) calloc(FILE_NAME_MAX, sizeof(char));
	u_int clusterAddr;
	DirEntry *entry;

	if(strcmp_igncase(dirPath, "root") == 0)
	{
		clusterAddr = bootRecord->rootDirStart;
	}
	else if(strchr(dirPath, '/') != NULL)
	{
		clusterAddr = bootRecord->rootDirStart;
		char **path = chop(dirPath, '/');
		char **p = path;
		int i = 0;
		while(p[i] != NULL && p[i+1] != NULL)
		{
			clusterAddr = getDirectoryClusterAddress(clusterAddr, p[i]);
			if(clusterAddr == 0)
				break;
			i++;
		}
		strcpy(dir, p[i]);
		if(clusterAddr)
			clusterAddr = getDirectoryClusterAddress(clusterAddr, dir);
	}
	else
	{
		strcpy(dir, dirPath);
		clusterAddr = getDirectoryClusterAddress(bootRecord->rootDirStart, dir);
	}


	if(clusterAddr == 0)
	{
		/* Directory not found */
		listing = calloc(25, sizeof(char));
		strcpy(listing, "Directory not found\n");
	}
	else
	{
		int end = 0;
		u_int count = 0;
		u_int entryAddr = 0;
		u_int currentCluster = clusterAddr;

		/* Determine the number of files/directories in the directory */
		while(!end)
		{
			entry = getDirEntry(clusterAddr, entryAddr);
			entryAddr++;
			if(entry->attr & 0x01)
				count++;
			if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
			{
				currentCluster = fileAllocTable[currentCluster];
				if(currentCluster == 0xffffffff) /* No more entries to check */
					end = 1;
			}
			free(entry);
		}

		/* Allocate space for the listing string */
		listing = calloc(100 * (count + 3), sizeof(char));
		strcpy(listing, "    File Name          | File Size |  Date/Time Created  |  Date/Time Modified | Start Cluster \n");
		strcat(listing, "  ==============================================================================================\n");

		/* Reset iteration variables */
		u_int i = 0;
		end = 0;
		entryAddr = 0;
		currentCluster = clusterAddr;

		/* Record file info for each directory listing */
		while(!end && i < count)
		{
			entry = getDirEntry(clusterAddr, entryAddr);
			entryAddr++;
			if(entry->attr & 0x01)
			{
				char fileName[47];
				char timeStr[20];
				char fileInfo[100];
				char temp[50];
				strcpy(fileInfo, "    ");
				strncpy(fileName, entry->fileName, FILE_NAME_MAX);
				if((entry->attr & 0x10) ^ 0x10)
				{
					strncat(fileName, ".", 1);
					strncat(fileName, entry->fileExt, FILE_EXT_SIZE);
				}
				sprintf(temp, "%-18s", fileName);
				strcat(fileInfo, temp);

				if((entry->attr & 0x10) ^ 0x10)
					sprintf(temp, " | %9u", entry->fileSize);
				else
					sprintf(temp, " |     -    ");
				strcat(fileInfo, temp);

				struct tm *createTime = decodeTimeBytes(entry->createDate);

				sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
					    createTime->tm_year,
					    createTime->tm_mon,
					    createTime->tm_mday,
					    createTime->tm_hour,
					    createTime->tm_min,
					    createTime->tm_sec);
				free(createTime);
				sprintf(temp, " | %19s", timeStr);
				strcat(fileInfo, temp);

				struct tm *modifiedTime = decodeTimeBytes(entry->modifiedDate);

				sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
					    modifiedTime->tm_year,
					    modifiedTime->tm_mon,
					    modifiedTime->tm_mday,
					    modifiedTime->tm_hour,
					    modifiedTime->tm_min,
					    modifiedTime->tm_sec);
				free(modifiedTime);
				sprintf(temp, " | %19s", timeStr);
				strcat(fileInfo, temp);

				sprintf(temp, " | %13u", entry->startCluster);
				strcat(fileInfo, temp);

				strcat(listing, fileInfo);
				strcat(listing, "\n");
			
				if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
				{
					currentCluster = fileAllocTable[currentCluster];
					if(currentCluster == 0xffffffff) /* No more entries to check */
						end = 1;
				}
				i++;
			}
			free(entry);
		}
		strcat(listing, "  ==============================================================================================\n");
	}

	return listing;
}

/**
 * Returns the directory cluster address of a directory. Searches through a 
 * given directory for a subdirectory of a given name. Returns 0 if no matching 
 * directory is found.
 *
 * @param  currentClusterAddr The cluster address of the directory to search
 * @param  dirName            The name of the directory to locate
 * @return                    The cluster address of the directory to locate
 */
u_int getDirectoryClusterAddress(u_int currentClusterAddr, char *dirName)
{
	int end = 0;
	int found = 0;
	u_int entryAddr = 0;
	u_int currentCluster = currentClusterAddr;
	u_int nextCluster = 0;
	u_int dirClusterAddress = 0;
	DirEntry *entry;

	while(!found && !end)
	{
		entry = getDirEntry(currentClusterAddr, entryAddr);
		entryAddr++;
		if(entry->attr & 0x10) /* Subdirectory */
		{
			if(strcmp(entry->fileName, dirName) == 0)
			{
				found = 1;
				dirClusterAddress = entry->startCluster;
			}
		}
	
		if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
		{
			nextCluster = fileAllocTable[currentCluster];
			if(nextCluster == 0xffffffff)
				end = 1;
			currentCluster = nextCluster;
		}
		free(entry);
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
u_int encodeTimeBytes()
{
	u_int timeBytes = 0;

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
struct tm *decodeTimeBytes(u_int timeBytes)
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
 * to the start of the starting data cluster.
 *
 * @return  The drive offset of the starting data cluster in bytes
 */
u_int getDataStartLoc()
{
	u_int loc = 0;
	/* Skip over boot cluster */
	loc += bootRecord->bytesPerCluster * bootRecord->reservedClusters;
	/* Skip over FAT */
	loc += bootRecord->bytesPerCluster * bootRecord->clustersPerFat;

	return loc;
}

/**
 * Returns the offset (in bytes) from the beginning of the virtual drive
 * to the start of the given directory cluster entry.
 *
 * @param  dirCluster The starting cluster address of the directory cluster 
 *                    containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The drive offset of the given entry in bytes
 */
u_int getDirEntryLoc(u_int dirCluster, u_int entryAddr)
{
	u_int entry = 0;
	u_int loc = 0;
	u_int currentCluster = dirCluster;
	u_int nextCluster = 0;

	/* Skip to the given dir cluster */
	loc += bootRecord->bytesPerCluster * currentCluster;

	while(entry < entryAddr)
	{
		entry++;
		loc += DIR_ENTRY_BYTES;
		if(entry % DIR_ENTRIES_PER_CLUSTER == 0)
		{
			nextCluster = fileAllocTable[currentCluster];
			if(nextCluster == 0xffffffff)
				loc = 0;
			else
				loc = bootRecord->bytesPerCluster * nextCluster;
			currentCluster = nextCluster;
		}
	}
	assert(loc);

	return loc;
}

/**
 * Returns the entry of the first free directory entry for a given
 * directory cluster. If the given directory cluster is full, the directory 
 * cluster chain will be extended.
 *
 * @param  dirCluster The starting cluster of the directory
 * @return            The entry address of the first free directory entry
 */
u_int getFirstFreeDirEntryAddr(u_int dirCluster)
{
	int found = 0;
	u_int entryAddr = 0;
	u_int currentCluster = dirCluster;
	u_int nextCluster = 0;
	DirEntry *entry;

	while(!found)
	{
		entry = getDirEntry(currentCluster, entryAddr);
		if((entry->attr & 0x1) ^ 0x1)
			found = 1;
		else
		{
			entryAddr++;
		
			if(entryAddr % DIR_ENTRIES_PER_CLUSTER == 0)
			{
				nextCluster = fileAllocTable[currentCluster];
				if(nextCluster == 0xffffffff)
					nextCluster = addClusterToChain(currentCluster);
				currentCluster = nextCluster;
			}
		}
	}

	return entryAddr;
}

/**
 * Returns a directory entry from a given directory
 *
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @return            The directory entry struct
 */
DirEntry *getDirEntry(u_int dirCluster, u_int entryAddr)
{
	DirEntry *entry = calloc(1, sizeof(*entry));
	u_int loc = getDirEntryLoc(dirCluster, entryAddr);
	fseek(virDrive, loc, SEEK_SET);
	fread(entry, sizeof(*entry), 1, virDrive);

	return entry;
}

/**
 * Sets a directory entry in a given directory
 *
 * @param  dirCluster The address of the directory cluster containing the entry
 * @param  entryAddr  The address of the entry within the directory cluster
 * @param  entry      The directory entry
 */
void setDirEntry(u_int dirCluster, u_int entryAddr, DirEntry *entry)
{
	u_int loc = getDirEntryLoc(dirCluster, entryAddr);
	fseek(virDrive, loc, SEEK_SET);
	fwrite(entry, sizeof(*entry), 1, virDrive);
}

/** 
 * ======================================================================== 
 * |                      File Struct Operations                          | 
 * ======================================================================== 
 *
 *     This section contains an implementation of a custom file object. The 
 *     properties contained within this struct hold all of the properties 
 *     of the file's directory entry and additionally includes some properties
 *     for keeping a record of the position of the pointer within the file.
 *     The record keeping properties are described below:
 *     
 *        - filePosition: holds the offset from the beginning of the file
 *                        to the position of the file's pointer.
 *                        
 *        - startClusterAddr: holds the address of the file's starting 
 *                            cluster
 *        
 *        - startLoc: holds the offset from the beginning of the drive
 *                    to the first location of the starting cluster of the 
 *                    file
 *
 *        - currentClusterAddr: holds the address of the cluster where the
 *                              file's pointer is currently located
 *
 *        - currentLoc: holds the offset from the beginning of the drive
 *                      to the location of the file pointer
 *
 *        - dirClusterAddr: holds the address of the directory cluster
 *                          in which the file's directory entry is 
 *                          located
 *
 *        - dirEntryAddr: holds the address of the file's directory entry 
 *                        within the directory cluster in which the file's
 *                        directory entry is located 
 */


void rewindBC_File(BC_FILE *file)
{
	file->filePosition = 0;
	file->currentLoc = file->startLoc;
	file->currentClusterAddr = file->startClusterAddr;
}

void destroyBC_File(BC_FILE *file)
{
	if(file)
	{	
		free(file);
		file = NULL;
	}
}

/** 
 * ======================================================================== 
 * |                         File Operations                              | 
 * ======================================================================== 
 *
 *     This file holds all of the input and output file operations that 
 *     can be performed on the the virtual drive.
 */

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

/**
 * Writes a number of bytes from a source into a file. The call will fail
 * if the length of the write exceeds the file size maximum.
 *
 * @param src  A pointer to the data to write
 * @param len  The number of bytes to write
 * @param dest A pointer to an open BC_FILE object
 */
void writeFile(void *src, u_int len, BC_FILE *dest)
{
	/* What if file object is invalid? */
	if(!dest)
	{
		fprintf(stdout, "BC_FILE object is null. Invalid operation.\n");
		return;
	}

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
				fseek(virDrive, dest->currentLoc, SEEK_SET);
				fwrite(src, 1, lenLeft, virDrive);
				dest->currentLoc += lenLeft;
				lenLeft -= lenLeft;
			}
			else /* write may span multiple clusters */
			{
				void *srcChunk = calloc(bytesLeft, sizeof(char));
				memcpy(srcChunk, src, bytesLeft);
				fseek(virDrive, dest->currentLoc, SEEK_SET);
				fwrite(srcChunk, 1, bytesLeft, virDrive);
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

/**
 * Reads a number of bytes from a file into a given memory location. The call
 * will fail if the length of the read exceeds.
 *
 * @param dest The buffer to store the data read
 * @param len  The number of bytes to read
 * @param src  A pointer to an open BC_FILE object
 */
void readFile(void *dest, u_int len, BC_FILE *src)
{
	/* What if file object is invalid? */
	if(!dest)
	{
		fprintf(stdout, "BC_FILE object is null. Invalid operation.\n");
		return;
	}

	/* If length of read exceeds the remaining length of the file,
	   set the length of read to the remaining length of the file */
	if(len > (src->fileSize - src->filePosition))
		len = (src->fileSize - src->filePosition);

	u_int lenLeft = len;
	u_int bytesLeft;
	
	while(lenLeft > 0)
	{
		bytesLeft = ((src->currentClusterAddr + 1) * bootRecord->bytesPerCluster) - src->currentLoc; 

		char *srcChunk = calloc(lenLeft, sizeof(char));
		fseek(virDrive, src->currentLoc, SEEK_SET);
		fread(srcChunk, sizeof(char), lenLeft, virDrive);
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

/**
 * Closes a file.
 *
 * @param file A pointer to an open BC_FILE object
 */
void closeFile(BC_FILE *file)
{
	if(file)
		destroyBC_File(file);
}

/**
 * Deletes a file. Removes all traces of a file from the
 * virtual drive and closes the BC_FILE object.
 *
 * @param file A pointer to an open BC_FILE object
 */
void deleteFile(BC_FILE *file)
{
	if(file)
	{
		/* Zero used clusters and FAT entries*/
		u_int currentCluster = file->startClusterAddr;
		u_int nextCluster = fileAllocTable[currentCluster];
		while(nextCluster != 0xffffffff)
		{
			formatCluster(currentCluster);
			fileAllocTable[currentCluster] = 0x00000000;
			currentCluster = nextCluster;
			nextCluster = fileAllocTable[currentCluster];
			bootRecord->freeClusters++;
		}
		formatCluster(currentCluster);
		fileAllocTable[currentCluster] = 0x00000000;
		bootRecord->freeClusters++;
		findAndSetNextFreeCluster();

		/* Zero directory entry */
		deleteDirEntry(file->dirClusterAddr, file->dirEntryAddr);

		destroyBC_File(file);
	}
}
