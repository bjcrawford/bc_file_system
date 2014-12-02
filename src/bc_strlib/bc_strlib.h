/* File: bc_strlib.h
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#ifndef BC_STRLIB
#define BC_STRLIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char  *append(char*, char*);
char **chop(char*, char);
int    count_occurences(char*, char*);
void   lstrip(char*);
int    is_blank(char*);
int    prefixcmp_igncase(char*, char*);
char  *replace(char*, char*, char*);
void   rstrip(char*);
int    suffixcmp_igncase(char*, char*);
char  *str_copy(char*);
int    strcmp_igncase(char*, char*);
void   strip(char*);

#endif