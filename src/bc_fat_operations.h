/**
 * @file bc_fat_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/


#ifndef BC_FAT_OPERATIONS
#define BC_FAT_OPERATIONS

 size_t getFATEntry(FILE **virDrive, size_t entryNum);
 size_t getFATStartLocation(FILE **virDrive);
 void setFATEntry(FILE **virDrive, size_t entryNum, size_t value);
 size_t addClusterToChain(FILE **virDrive, size_t entryNum);
void findAndSetNextFreeCluster(FILE **virDrive);

#endif