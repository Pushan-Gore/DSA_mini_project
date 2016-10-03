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

void d_init(dict *d) {
	/* Initially the Max Limit is 2048 */
	d->dictionary = (char **) malloc (sizeof(char *) * MAX_DICT_LEN); 
	uint16_t ind;
	
	/* Initializing idices 0 to 255 to respective characters */
	for(ind = 0; ind < 255; ind++) {
		d->dictionary[ind] = (char *) malloc(sizeof(char) * 2);
		d->dictionary[ind][0] = ind;
		d->dictionary[ind][1] = '\0';
	}
	d->lim_code = 256;
}

FILE * open_file(char *fname) {
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
	fp = open_file(fname);
	
	/* Remove the ".mtz" extension from the input to write to the output file */
	uint16_t len = strlen(fname);
	fname[len - 4] = '\0';
	op = fopen(fname, "w");	
	
	char str[1024], arr[2];
	uint16_t key;

	fread(&key, sizeof(uint16_t), 1, fp); 		
	fprintf(op, "%s", d->dictionary[key]); 		
	strcpy(str, d->dictionary[key]);
	while(fread(&key, sizeof(uint16_t), 1, fp)) {
		if(key < d->lim_code) {
			fprintf(op, "%s", d->dictionary[key]);
			arr[0] = d->dictionary[key][0];
			arr[1] = '\0';
			strcat(str, arr);
			addto_dict(d, str);
			strcpy(str, d->dictionary[key]);
		}
		else {
			arr[0] = str[0];
			arr[1] = '\0';
			strcat(str, arr);
			fprintf(op, "%s", str);
			addto_dict(d, str);
		}
	}
	
	free(d->dictionary);
	fclose(fp);
	fclose(op);
}

uint16_t search_dict(dict *d, char *str) {
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
void addto_dict(dict *d, char *str) {
	d->dictionary[d->lim_code] = (char *) malloc(strlen(str) + 1);
	if(d->dictionary[d->lim_code] == NULL) {
		exit(1);
	}
	strcpy(d->dictionary[d->lim_code], str);
	d->lim_code++;
}

