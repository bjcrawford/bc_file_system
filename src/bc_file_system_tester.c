/**
 * @file bc_file_system_tester.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include "bc_file_system.h"

 typedef struct 
 {
 	FILE *virDisk;
 } VirtualDrive;

 int main(int argc, char **argv)
 {
 	FILE *vd;

 	if(!openVirDrive(&vd, "Drive2MB"))
 		fprintf(stderr, "Error opening drive");
 	fprintf(stdout, "VDrive Opened\n");

 	initVirDrive(&vd, "VDrive");

 	fprintf(stdout, "Initialized: %zu\n", getInitialized(&vd));
 	fprintf(stdout, "Drive Label: %s\n", getDriveLabel(&vd));
 	fprintf(stdout, "Bytes Per Cluster: %zu\n", getBytesPerCluster(&vd));
 	fprintf(stdout, "Number Of Reserved Clusters: %zu\n", getNumberOfReservedClusters(&vd));
 	fprintf(stdout, "Number Of Clusters On Drive: %zu\n", getNumberOfClustersOnDrive(&vd));
 	fprintf(stdout, "Number Of Clusters Per FAT: %zu\n", getNumberOfClustersPerFAT(&vd));
 	fprintf(stdout, "First Cluster Of Root Dir: %zu\n", getFirstClusterOfRootDir(&vd));
 	fprintf(stdout, "Number Of Free Clusters: %zu\n", getNumberOfFreeClusters(&vd));
 	fprintf(stdout, "Next Free Cluster: %zu\n", getNextFreeCluster(&vd));
 	fprintf(stdout, "Size Of Drive: %zu\n", getSizeOfDrive(&vd));

 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile0", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile1", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile2", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile3", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile4", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile5", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile6", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile7", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile8", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile9", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile10", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile11", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile12", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile13", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile14", "txt");
 	
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x13, "folder0", "");
 	setFATEntry(&vd, 33, 0xffffffff);
 	findAndSetNextFreeCluster(&vd);
 	createDirEntry(&vd, 33, 0x3, "file0", "txt");
 	createDirEntry(&vd, 33, 0x3, "file1", "txt");
 	createDirEntry(&vd, 33, 0x3, "file2", "txt");
 	
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile15", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile16", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile17", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile18", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile19", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile20", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile21", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile22", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile23", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile24", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile25", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile26", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile27", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile28", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile29", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile30", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile31", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile32", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile33", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile34", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile35", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile36", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile37", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile38", "txt");
 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile39", "txt");

 	printf("Root Directory Listing: \n%s", getDirectoryListing(&vd, getFirstClusterOfRootDir(&vd)));
 	printf("folder0 Directory Listing: \n%s", getDirectoryListing(&vd, 33));


 	fprintf(stdout, "Initialized: %zu\n", getInitialized(&vd));
 	fprintf(stdout, "Drive Label: %s\n", getDriveLabel(&vd));
 	fprintf(stdout, "Bytes Per Cluster: %zu\n", getBytesPerCluster(&vd));
 	fprintf(stdout, "Number Of Reserved Clusters: %zu\n", getNumberOfReservedClusters(&vd));
 	fprintf(stdout, "Number Of Clusters On Drive: %zu\n", getNumberOfClustersOnDrive(&vd));
 	fprintf(stdout, "Number Of Clusters Per FAT: %zu\n", getNumberOfClustersPerFAT(&vd));
 	fprintf(stdout, "First Cluster Of Root Dir: %zu\n", getFirstClusterOfRootDir(&vd));
 	fprintf(stdout, "Number Of Free Clusters: %zu\n", getNumberOfFreeClusters(&vd));
 	fprintf(stdout, "Next Free Cluster: %zu\n", getNextFreeCluster(&vd));
 	fprintf(stdout, "Size Of Drive: %zu\n", getSizeOfDrive(&vd));

 	/*
 	size_t t = getDirEntryCreateTimeBytes(&vd, getFirstClusterOfRootDir(&vd), 0);
 	struct tm *dt = decodeTimeBytes(t);

 	fprintf(stdout, "Year: %d\n", dt->tm_year);
 	fprintf(stdout, "Month: %d\n", dt->tm_mon);
 	fprintf(stdout, "Day: %d\n", dt->tm_mday);
 	fprintf(stdout, "Hour: %d\n", dt->tm_hour);
 	fprintf(stdout, "Min: %d\n", dt->tm_min);
 	fprintf(stdout, "Sec: %d\n", dt->tm_sec);
 	*/

 	closeVirDrive(&vd);

 	return 0;
 }
