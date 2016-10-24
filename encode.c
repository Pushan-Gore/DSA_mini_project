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
#include <error.h>
#include "encode.h"
#include "dictionary.h"

uint16_t index_len[16000];

void *memcat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (uint8_t *)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

void e_init(dict *d) {
	d->dictionary = (uint8_t **) malloc (sizeof(uint8_t *) * MAX_DICT_LEN);   
	uint16_t ind;
	
	/* Initializing idices 0 to 255 to respective characters */
	for(ind = 0; ind < 256; ind++) {
		d->dictionary[ind] = (uint8_t *) malloc(sizeof(uint8_t) * 1); 
		d->dictionary[ind][0] = (uint8_t) ind;
	}
	d->lim_code = ind;
}

FILE * e_open_file(char *fname) {
	FILE *fp;
	fp = fopen(fname, "r");
	
	/* Exit from the program if No file exists */
	if(fp == NULL) {
		perror("File open failed\n");
		exit(1);
	}
	return fp;
}

void encode(dict *d, char *fname) {
	FILE *fp, *op;
	fp = e_open_file(fname);

	/* Append ".mtz" extension to the output file */
	op = fopen(strcat(fname, ".mtz"), "w");	
	if(op == NULL) {
		perror("File open failed\n");
		exit(1);
	}	

	uint8_t str[16000], temp[16000], arr[1];
	uint16_t code;
	uint16_t count;

	fread(arr, sizeof(uint8_t), 1, fp);
	memcpy(str, arr, sizeof(uint8_t));  
	count = sizeof(uint8_t);

	while(fread(arr, sizeof(uint8_t), 1, fp)) {

		memcpy(temp, str, count);    

		if(e_search_dict(d, memcat(str, count, arr, sizeof(uint8_t)), count + sizeof(uint8_t)) == MAX_DICT_LEN) {            

			code = e_search_dict(d, temp, count); 
			fwrite(&code, sizeof(uint16_t), 1, op);
			e_addto_dict(d, str, count + sizeof(uint8_t));
			memcpy(str, arr, sizeof(uint8_t));    
			count = sizeof(uint8_t);

		}

		else {
			count = count + sizeof(uint8_t);
		}
	}

	if(d->lim_code == MAX_DICT_LEN) {
		printf("End of dictionary length.\n");
	}

	code = e_search_dict(d, str, count);
	fwrite(&code, sizeof(uint16_t), 1, op);

	// Debug code
	printf("%d\n", d->lim_code);
	
	// Write a free function.
	free_dict(d);
	fclose(fp);
	fclose(op);
}

uint16_t e_search_dict(dict *d, uint8_t *str, uint16_t len) {      
	if(len == 1) 
		return (int) str[0];
 
	uint16_t i;
	for(i = 256; i < d->lim_code; i++) {
		if((len == index_len[i - 256]) && !memcmp(str, d->dictionary[i], len)){     
			return i;			
		}
	}
	return MAX_DICT_LEN;
}

/* Appends the new string at the lim_code position of the dictionary */
void e_addto_dict(dict *d, uint8_t *str, uint16_t count) {
	d->dictionary[d->lim_code] = (uint8_t *) malloc(sizeof(uint8_t) * count);    

	if(d->lim_code == MAX_DICT_LEN) {  
		exit(1);

	}
	index_len[d->lim_code - 256] = count; 
	memcpy(d->dictionary[d->lim_code], str, count);
	d->lim_code++;
}

void free_dict(dict *d){
	uint16_t i = 0;
	for(i = 0; i < d->lim_code; i++) {
		d->dictionary[i] = NULL;
		free(d->dictionary[i]);
	}
	//free(d->dictionary); 
	//d->dictionary = NULL;
}
