/**
 * @file bc_file_operations.h
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: 
*/

#ifndef BC_FILE_OPERATIONS
#define BC_FILE_OPERATIONS


BC_FILE *createFile(FILE**, char*);
void createDirectory(FILE**, char*);
size_t writeFile(FILE**, void*, size_t, size_t, BC_FILE*);
size_t readFile(FILE**, void*, size_t, size_t, BC_FILE*);
int closeFile(FILE**, BC_FILE*);
int deleteFile(FILE**, BC_FILE*);

#endif