#ifndef __DECODE_H
#define __DECODE_H

#include "dictionary.h"

/* Initializes the dictionary. Puts value from 0 to 255 in the dictionary */
/* Returns the malloced pointer of the dictionary */
void d_init(dict *d);

/* Opens the file "fname" and returns the pointer */
FILE *d_open_file(char *fname);

/* Decode should decompress the file with format ".mtz" */
/* It checks for the format in the name */
/* Outputs a file with "fname" - ".mtz" */
void decode(dict *d, char *fname);

/* Appends to the dictionary at lim_code of the dictionary */
void d_addto_dict(dict *d, uint8_t *str, uint16_t count);

/* Frees all the malloced pointers */
void d_free_dict(dict *d);

#endif		
