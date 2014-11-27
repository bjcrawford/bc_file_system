/* File: chop.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdlib.h>

/* Chops the given c string into a set of c strings delimiting
   by a given character.

   Input:  C string - The string to be chopped
           Char - The character to be used as a delimiter

   Output: Array of C strings - The strings chopped from the 
           original string given delimited by the given
           character */
char **chop(char *s, char c)
{
	if(s == NULL)
		return NULL;
	
	char pat[2];
	pat[0] = c;
	pat[1] = '\0';
	int l = count_occurences(s, pat) + 2;
	char **slices;
	if((slices = calloc(l, sizeof(char*))) == NULL)
	{
		return NULL;
	}
	char *psb = s;
	char *pse = s;
	int i;
	for(i = 0; i < l - 1; i++)
	{
		while(*pse != '\0' && *pse != c)
			pse++;
		int wl = pse - psb;
		if((slices[i] = calloc(wl, sizeof(char))) == NULL)
		{
			return NULL;
		} 
		char *pslice = slices[i];
		while(*psb != '\0' && *psb != c)
			*pslice++ = *psb++;
		*pslice = '\0';
		psb += 1;
		pse = psb;
	}
	slices[i] = NULL;

	return slices;
}