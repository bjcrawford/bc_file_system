/* File: prefix_igncase.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Returns 1 if pre is a prefix of s, ignoring case. Otherwise 0 
   If either s or pre is blank, 0 is returned 

   Input:  C String - The string to be tested for a prefix
           C string - The string containing the prefix

   Output: Integer - Returns 1 if string pre is a prefix of s,
           otherwise returns 0. */
int prefixcmp_igncase(char *s, char *pre)
{
	if(is_blank(s) || is_blank(pre))
		return 0;
	int sl = strlen(s);
	int pl = strlen(pre);
	while(sl-- && pl--)
		if(tolower(*s++) != tolower(*pre++))
			return 0;

	return 1;
}
