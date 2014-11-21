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
 	fprintf(stdout, "Bytes Per Sector: %zu\n", getBytesPerSector(&vd));
 	fprintf(stdout, "Sectors Per Cluster: %zu\n", getSectorsPerCluster(&vd));
 	fprintf(stdout, "Number Of Reserved Sectors: %zu\n", getNumberOfReservedSectors(&vd));
 	fprintf(stdout, "Number Of Sectors On Drive: %zu\n", getNumberOfSectorsOnDrive(&vd));
 	fprintf(stdout, "Number Of Sectors Per FAT: %zu\n", getNumberOfSectorsPerFAT(&vd));
 	fprintf(stdout, "First Cluster Of Root Dir: %zu\n", getFirstClusterOfRootDir(&vd));
 	fprintf(stdout, "Number Of Free Clusters: %zu\n", getNumberOfFreeClusters(&vd));
 	fprintf(stdout, "Next Free Cluster: %zu\n", getNextFreeCluster(&vd));
 	fprintf(stdout, "Size Of Drive: %zu\n", getSizeOfDrive(&vd));

 	createDirEntry(&vd, getFirstClusterOfRootDir(&vd), 0x3, "testfile", "txt");

 	size_t t = getDirEntryCreateTimeBytes(&vd, getFirstClusterOfRootDir(&vd), 0);
 	struct tm *dt = decodeTimeBytes(t);

 	fprintf(stdout, "Year: %d\n", dt->tm_year);
 	fprintf(stdout, "Month: %d\n", dt->tm_mon);
 	fprintf(stdout, "Day: %d\n", dt->tm_mday);
 	fprintf(stdout, "Hour: %d\n", dt->tm_hour);
 	fprintf(stdout, "Min: %d\n", dt->tm_min);
 	fprintf(stdout, "Sec: %d\n", dt->tm_sec);

 	closeVirDrive(&vd);

 	return 0;
 }
