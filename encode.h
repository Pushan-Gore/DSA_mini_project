#ifndef __ENCODE_H
#define __ENCODE_H

#define MAX_DICT_LEN 8192 
#include <inttypes.h>
/* Dictionary is an array of 4096 (2 ^ 12) elements 
 * This array is an array of character pointers
 */

typedef struct dict{
	char **dictionary;
	uint16_t lim_code;
}dict;

/* Initializes the dictionary. Puts value from 0 to 255 in the dictionary 
 * Returns the malloced pointer of the dictionary
 */
void d_init(dict *d);

FILE *open_file(char *fname);

/* Encode should create a file with some extension (say .mtz) ( stands for "My tar zip) */
void encode(dict *d, char *fname);

/* Returns the code of the string if the string is encountered int the dictionary 
 * Else returns -1
 */
uint16_t search_dict(dict *d, char *str);

/* Appends to the dictionary */
void addto_dict(dict *d, char *str);

#endif		
