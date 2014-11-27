/* File: replace.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdlib.h>

/* Returns a copy of the string s, but with each instance of
   string pat replaced with string rep. The caller is responsible
   for freeing the dynamically allocated memory for the returned
   c string. 

   Input:  C string - The string to be modified
           C string - The string containing the pattern
           to be replaced
           C string - The string containing the pattern
           to replace with

   Output: C string - A copy of s with all instances of pat
           replaced with rep */
char *replace(char *s, char *pat, char *rep)
{
	if(s == NULL)
		return NULL;
	int ls = strlen(s);
	int lpat = strlen(pat);
	int lrep = strlen(rep);
	int o = count_occurences(s, pat);
	int lns = ls - (o*lpat) + (o*lrep) + 1;
	char *ns;
	if((ns = calloc(lns, sizeof(char))) == NULL)
	{
		return NULL;
	}

	char *ps = s;
	char *pns = ns;
	char *ppat = pat;
	char *prep = rep;
	int i;

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
		{
			prep = rep;
			for(i = 0; i < lrep; i++)
				*pns++ = *prep++;
		}
		else
		{
			ps -= i;
			*pns++ = *ps;
			ps += i;
			ps -= i - 1;
		}
		ppat = pat;
	}
	*pns = '\0';

	return ns;
}