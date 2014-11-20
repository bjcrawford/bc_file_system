/**
 * @file bc_fat_operations.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include "bc_fat_operations.h"

 size_t getFATEntry(FILE **virDrive, size_t entryNum)
 {
 	size_t loc = 0;
 	loc += getBytesPerSector(virDrive);
 	loc += entryNum * 4;

 	return readNum(virDrive, loc, 4);
 }

 void setFATEntry(FILE **virDrive, size_t entryNum, size_t value)
 {
 	size_t loc = 0;
 	loc += getBytesPerSector(virDrive);
 	loc += entryNum * 4;
 	writeNum(virDrive, loc, 4, value);
 }