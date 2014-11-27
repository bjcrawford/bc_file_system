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

 	fprintf(stdout, "\n===================================================\n");
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
 	fprintf(stdout, "===================================================\n\n");

 	size_t rootAddr = getFirstClusterOfRootDir(&vd);

 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile0", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile1", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile2", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile3", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile4", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile5", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile6", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile7", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile8", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile9", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile10", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile11", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile12", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile13", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile14", "txt");
 	
 	size_t subFolder0Addr = createDirSubEntry(&vd, getFirstClusterOfRootDir(&vd), 0x13, "folder0");
 	createDirFileEntry(&vd, subFolder0Addr, 0x3, "file0", "txt");
 	createDirFileEntry(&vd, subFolder0Addr, 0x3, "file1", "txt");
 	createDirFileEntry(&vd, subFolder0Addr, 0x3, "file2", "txt");
 	
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile15", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile16", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile17", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile18", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile19", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile20", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile21", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile22", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile23", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile24", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile25", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile26", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile27", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile28", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile29", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile30", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile31", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile32", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile33", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile34", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile35", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile36", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile37", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile38", "txt");
 	createDirFileEntry(&vd, rootAddr, 0x3, "testfile39", "txt");

 	fprintf(stdout, "Root Directory Listing: \n%s", getDirectoryListing(&vd, getFirstClusterOfRootDir(&vd)));
 	fprintf(stdout, "folder0 Directory Listing: \n%s", getDirectoryListing(&vd, 33));

 	fprintf(stdout, "Directory cluster address of folder0: %ld\n", 
 		    getDirectoryClusterAddress(&vd, 32, "folder0"));


 	fprintf(stdout, "\n===================================================\n");
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
 	fprintf(stdout, "===================================================\n\n");

 	openFile(&vd, "one/two/three/brett.txt");
 	fprintf(stdout, "Root Directory Listing: \n%s", getDirectoryListing(&vd, getFirstClusterOfRootDir(&vd)));
 	size_t subOneAddr = getDirectoryClusterAddress(&vd, getFirstClusterOfRootDir(&vd), "one");
 	fprintf(stdout, "one Directory Listing: \n%s", getDirectoryListing(&vd, subOneAddr));
 	size_t subTwoAddr = getDirectoryClusterAddress(&vd, subOneAddr, "two");
 	fprintf(stdout, "two Directory Listing: \n%s", getDirectoryListing(&vd, subTwoAddr));
 	size_t subThreeAddr = getDirectoryClusterAddress(&vd, subTwoAddr, "three");
 	fprintf(stdout, "three Directory Listing: \n%s", getDirectoryListing(&vd, subThreeAddr));


 	fprintf(stdout, "\n===================================================\n");
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
 	fprintf(stdout, "===================================================\n\n");
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
