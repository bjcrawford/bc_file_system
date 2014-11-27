/* File: str-copy.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdio.h>

/* Creates a copy of a given string. The caller is responsible
   for freeing the dynamically allocated memory for the
   returned string.

   Input:  C string - The string to copied

   Output: C string - A copy of the given string */
char *str_copy(char *s)
{
	int l = strlen(s) + 1;
	char *ns;
	if((ns = calloc(l, sizeof(char))) == NULL)
	{
		return NULL;
	}
	char *p = ns;
	while(*s != '\0')
		*p++ = *s++;
	*p = '\0';

	return ns;
}