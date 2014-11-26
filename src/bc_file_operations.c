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

BC_FILE *createFile(FILE **virDrive, char *fileName)
{
	BC_FILE *fp = malloc(sizeof(fp));

	return fp;
}

void createDirectory(FILE **virDrive, char *dirName)
{

}

size_t writeFile(FILE **virDrive, void *src, size_t size, size_t len, BC_FILE *dest)
{
	size_t written = 0;

	return written;
}

size_t readFile(FILE **virDrive, void *dest, size_t size, size_t len, BC_FILE *src)
{
	size_t read = 0;

	return read;
}

int closeFile(FILE **virDrive, BC_FILE *file)
{

}

int deleteFile(FILE **virDrive, BC_FILE *file)
{
	
}