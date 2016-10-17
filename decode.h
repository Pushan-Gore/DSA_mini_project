#ifndef __DECODE_H
#define __DECODE_H

#include "dictionary.h"

/* Initializes the dictionary. Puts value from 0 to 255 in the dictionary 
 * Returns the malloced pointer of the dictionary
 */
void d_init(dict *d);

FILE *d_open_file(char *fname);

/* Encode should create a file with some extension (say .mtz) ( stands for "My tar zip) */
void decode(dict *d, char *fname);

/* Returns the code of the string if the string is encountered int the dictionary 
 * Else returns -1
 */
uint16_t d_search_dict(dict *d, char *str);

/* Appends to the dictionary */
void d_addto_dict(dict *d, char *str, int count);

#endif		
