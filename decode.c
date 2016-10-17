/* Decoding using LZW compressiong algorithm

 * Algorithm is :
 * BEGIN:
	s = NILL
   	while not EOF {
		k = next input code
		entry = dictionary entry for k
		//exception case
		if(entry == NULL)
			entry = s + s[0]
		output entry
		if(s != NILL) 
			add string s + entry[0] to dictionary with a new code
		s = entry
	}
   END
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "decode.h"

/* 7936 = 8192 - 256 */
int index_len[7926];

void *d_memcat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (char*)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

void d_init(dict *d) {
	/* Initially the Max Limit is 8192 */
	d->dictionary = (char **) malloc (sizeof(char *) * MAX_DICT_LEN); 
	uint16_t ind;
	
	/* Initializing idices 0 to 255 to respective characters */
	for(ind = 0; ind < 255; ind++) {
		d->dictionary[ind] = (char *) malloc(sizeof(char) * 1);
		d->dictionary[ind][0] = ind;
	}
	d->lim_code = 256;
}

FILE * d_open_file(char *fname) {
	FILE *fp;
	fp = fopen(fname, "r");
	
	/* Exit from the program is No file exists */
	if(fp == NULL) {
		printf("File open failed : %s\n", fname);
		exit(1);
	}
	return fp;
}

void decode(dict *d, char *fname) {
	FILE *fp, *op;
	fp = d_open_file(fname);
	
	/* Remove the ".mtz" extension from the input to write to the output file */
	uint16_t len = strlen(fname);
	fname[len - 4] = '\0';
	op = fopen(fname, "w");	
	
	char str[1024], arr[2];
	uint16_t key;
	int count;

	fread(&key, sizeof(uint16_t), 1, fp);
	fwrite(d->dictionary[key], 1, 1, op);
	memcpy(str, d->dictionary[key], 1);
	count = 1;
	
	while(fread(&key, sizeof(uint16_t), 1, fp)) {
		if(key < d->lim_code) {
			if(key > 255)
				fwrite(d->dictionary[key], index_len[key - 256], 1, op);   
			else
				fwrite(d->dictionary[key], 1, 1, op);

			arr[0] = d->dictionary[key][0];
			d_memcat(str, count, arr, 1);
			d_addto_dict(d, str, count + 1);

			if(key > 255) {
				memcpy(str, d->dictionary[key], index_len[key - 256]);
				count = index_len[key - 256];
			}
			else {
				memcpy(str, d->dictionary[key], 1);
				count = 1;
			}
		}
		else {
			arr[0] = str[0];
			d_memcat(str, count, arr, 1) ;
			fwrite(str, count + 1, 1, op);
			d_addto_dict(d, str, count + 1);
			count++;
		}
	}
	free(d->dictionary);
	fclose(fp);
	fclose(op);
}

uint16_t d_search_dict(dict *d, char *str) {
	if(strlen(str) == 1) {
		return (uint16_t)str[0];
	}
	uint16_t i;
	for(i = 256; i < d->lim_code; i++) {
		if(!strcmp(str, d->dictionary[i])) 
			return i;
	}
	return MAX_DICT_LEN;
}

/* Appends the new string at the lim_code position of the dictionary */
void d_addto_dict(dict *d, char *str, int count) {
	d->dictionary[d->lim_code] = (char *) malloc(count);
	/*if(d->dictionary[d->lim_code] == NULL) {
		exit(1);
	}*/
	memcpy(d->dictionary[d->lim_code], str, count);
	index_len[d->lim_code - 256] = count;
	d->lim_code++;
}

