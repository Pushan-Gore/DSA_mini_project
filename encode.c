/* Encoding using LZW compressiong algorithm

 * Algorithm is :
 * BEGIN:
	s = next input character
	while not EOF
		{ c = next input character
		  if s + c exists in dictionary 
			s = s + c
		  else {
			output the code for s;
			add string s + c to the dictionary with a new code
			s = c
		  }
		}
	output the code for s
   END
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "encode.h"

void d_init(dict *d) {
	/* Initially the Max Limit is 4096 */
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
/*
dict * reinit(dict *d) {
	dict 
	d->dictionary = realloc (d->dictionary
*/
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

void encode(dict *d, char *fname) {
	FILE *fp, *op;
	fp = open_file(fname);

	/* Append ".mtz" extension to the output file */
	op = fopen(strcat(fname, ".mtz"), "w");	
	
	char str[128], temp[128], arr[2];
	char ch1, ch2;
	uint16_t code;

	fscanf(fp, "%c", &ch1);
	arr[0] = ch1;
	arr[1] = '\0';
	strcpy(str, arr);
	while(fscanf(fp, "%c", &ch2) != EOF) {
		arr[0] = ch2;
		arr[1] = '\0';
		strcpy(temp, str);
		if(search_dict(d,strcat(str,arr)) == MAX_DICT_LEN) {
			code = search_dict(d, temp);
			fwrite(&code, sizeof(uint16_t), 1, op);
			addto_dict(d, str);
			strcpy(str, arr);
		}
	}
	code = search_dict(d, str);
	fwrite(&code, sizeof(uint16_t), 1, op);
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

