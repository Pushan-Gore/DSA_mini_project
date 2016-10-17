#ifndef __ENCODE_H
#define __ENCODE_H

#include "dictionary.h"

/* Initializes the dictionary. Puts value from 0 to 255 in the dictionary 
 * Returns the malloced pointer of the dictionary
 */
void e_init(dict *d);

FILE *e_open_file(char *fname);

/* Encode should create a file with some extension (say .mtz) ( stands for "My tar zip) */
void encode(dict *d, char *fname);

/* Returns the code of the string if the string is encountered int the dictionary 
 * Else returns -1
 */
uint16_t e_search_dict(dict *d, char *str, int len);

/* Appends to the dictionary */
void e_addto_dict(dict *d, char *str, int count);

#endif		
