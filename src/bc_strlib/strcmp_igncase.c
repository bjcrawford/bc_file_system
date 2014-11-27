/* File: strcmp_igncase.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <ctype.h>

/* Compares strings s1 and s2 ignoring case. 

   Input:  C string - A string to be compared
           C string - A string to be compared

   Output: Integer - Returns a negative
           number if s1 is lexicographically less than s2, a positive 
           number if s1 is lexicographically greater than s2, and zero
           if the two strings are equivalent  */
int strcmp_igncase(char *s1, char*s2)
{
	char *ps1 = s1;
	char *ps2 = s2;
	int diff = 0;
	while(*ps1 != '\0' && *ps2 != '\0' && diff == 0)
		diff = tolower(*ps1++) - tolower(*ps2++);
	if(diff == 0)
		diff = tolower(*ps1) - tolower(*ps2);

	return diff;
}