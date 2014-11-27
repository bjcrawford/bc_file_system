/* File: rstrip.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Removes the trailing whitespace from string s. String s is
   modified by this function.

   Input:  C string - The string to remove the trailing 
           whitespace from

   Output: None */
void rstrip(char *s)
{
	char *ps = s + strlen(s) - 1;
	while(*ps == ' ')
		ps--;
	ps++;
	*ps = '\0';
}