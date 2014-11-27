/* File: count_occurrences.c
   Author: Brett Crawford
   Date: 2014-09-22
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Returns the numbers of occurrences of string pat in string s 

   Input:  C string - The string to be search for occurrences
           C string - The pattern to search for

   Output: Integer - The number of occurence of string pat in
           string s */
int count_occurences(char *s, char *pat)
{
	int i;
	int c = 0;
	int lpat = strlen(pat);
	char *ps = s;
	char *ppat = pat;
	while(*ps != '\0')
	{
		int match = 1;
		for(i = 0; i < lpat && match == 1; i++)
		{
			if(*ps == '\0' || *ps != *ppat)
				match = 0;
			else
				ppat++;
			ps++;
		}
		if(match == 1)
			c++;
		ps -= i - 1;
		ppat = pat;
	}

	return c;
}
