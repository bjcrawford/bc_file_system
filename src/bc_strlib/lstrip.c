/* File: lstrip.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Removes the leading whitespace from string s. String s is
   modified by this function.

   Input:  C string - The string to remove the leading 
           whitespace from

   Output: None */
void lstrip(char *s)
{
	char *ps = s;
	while(*ps == ' ')
		ps++;
	while((*s = *ps) != '\0')
	{
		ps++;
		s++;
	}
}