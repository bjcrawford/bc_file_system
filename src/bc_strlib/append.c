/* File: append.c
   Author: Brett Crawford
   Date: 2014-09-22
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdio.h>

/* Appends s2 to the end of s1 and returns the result as a new c string.
   The caller is responsible for freeing the dynamically allocated
   memory for the returned c string.

   Input:  C string, the string to be appended to
           C string, the string to append with

   Output: C string, a new c string - s1 + s2 */
char *append(char *s1, char *s2)
{
	int l = strlen(s1) + strlen(s2) + 1;
	char *ns;
	if((ns = calloc(l, sizeof(char))) == NULL)
	{
		return NULL;
	}
	char *p = ns;
	while(*s1 != '\0')
		*p++ = *s1++;
	while(*s2 != '\0')
		*p++ = *s2++;
	*p = '\0';

	return ns;
}
