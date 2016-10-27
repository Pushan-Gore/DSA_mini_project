/***** Pushan Gore *****/
/*****  111503023  *****/

/* Decompression */
/* The program takes a file name as an argument and a dictionary. */
/* This Decompression algorithm uses a dictionary to store the byte sequence 
 * by reading the 16 bit numbers stored in the ".mtz" file. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "decode.h"
#include "dictionary.h"

/* Stores the length of byte sequence*/
uint16_t *index_len;

int D_MAX_DICT_LEN = MAX_LEN;

/* Memory concatination function. */
/* Same as strcat. */

void *d_memcat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (uint8_t*)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

/* Initializes the dictionary 'd' */
/* The first 256 data entries are also initiated here */
/* Code limit (lim_code) is the number of dictionary entries */

void d_init(dict *d) {
	/* Malloc the dictionary of pointers with MAX_DICT_LEN as 16000 */ 
	d->dictionary = (uint8_t **) malloc (sizeof(uint8_t *) * D_MAX_DICT_LEN); 
	
	uint16_t ind;
	/* Initializing idices 0 to 255 to respective characters */
	for(ind = 0; ind < 256; ind++) {
		d->dictionary[ind] = (uint8_t *) malloc(sizeof(uint8_t) * 1);
		d->dictionary[ind][0] = (uint8_t) ind;
	}
	/* Set limit of dictionary (lim_code) as 256 */
	d->lim_code = ind;
}

/*A file open fucntion that returns the pointer of the file passed as an argument */

FILE * d_open_file(char *fname) {
	FILE *fp;
	fp = fopen(fname, "r");
	
	/* Exit from the program is No file exists */
	if(fp == NULL) {
		perror("File open failed\n");
		exit(1);
	}
	return fp;
}

/* Main Decompression Function */
void decode(dict *d, char *fname) {
	FILE *fp, *op;
	char *file, *token, *ext = "mtz";
	char flag = 'r'; /* Flag is reset */ 
	file = (char *) malloc(sizeof(char) * (strlen(fname) + 1));
	strcpy(file, fname);

	/* Check whether the file has a .mtz format or not */ 
	token = strtok(file, ".");
	while(token != NULL) {  
		if(!strcmp(token, ext))
			flag = 's'; /* Flag is set */
		else 
			flag = 'r';
		token = strtok(NULL, ".");	
	}
	if(flag == 'r') {
		printf("Incorrect file format: %s\n", fname);
		exit(1);
	}
	free(file);
	/* Open the compressed file with format ".mtz" */
	fp = d_open_file(fname);
	
	/* Remove the ".mtz" extension from the input to write to the output file */
	uint16_t len = strlen(fname);
	fname[len - 4] = '\0';
	op = fopen(fname, "w");	
	
	uint8_t str[16000], arr[1];
	uint16_t key, prev_key;
	uint16_t count;

	index_len = (uint16_t *) malloc(sizeof(uint16_t) * D_MAX_DICT_LEN);

	/* Main Decompression Loop */
	fread(&key, sizeof(uint16_t), 1, fp);
	fwrite(d->dictionary[key], sizeof(uint8_t), 1, op);
	memcpy(str, d->dictionary[key], 1);
	prev_key = key;
	count = sizeof(uint8_t);
	
	while(fread(&key, sizeof(uint16_t), 1, fp)) {
		if(key < d->lim_code) {	
			if(key > 255) {
				memcpy(str, d->dictionary[key], index_len[key - 256]);
				count = index_len[key - 256];
			}
			else {
				memcpy(str, d->dictionary[key], 1);
				count = sizeof(uint8_t);
			}
		}
		
		else {
			if(prev_key > 255) {
				memcpy(str, d->dictionary[prev_key], index_len[prev_key - 256]);
				count = index_len[prev_key - 256];
			}
			else {
				memcpy(str, d->dictionary[prev_key], 1);
				count = sizeof(uint8_t);
			}
			d_memcat(str, count, arr, sizeof(uint8_t)); 
			count = count + sizeof(uint8_t);
		}
		
		fwrite(str, count, 1, op);
		arr[0] = str[0];
		if(prev_key > 255) { 
			memcpy(str, d->dictionary[prev_key] , index_len[prev_key - 256]);
			d_memcat(str, index_len[prev_key - 256], arr ,sizeof(uint8_t));
			count = index_len[prev_key - 256] + sizeof(uint8_t); 
		}
		else {
			memcpy(str, d->dictionary[prev_key] , sizeof(uint8_t));
			d_memcat(str, sizeof(uint8_t), arr, sizeof(uint8_t));
			count = 2 * sizeof(uint8_t);
		}
		d_addto_dict(d, str, count); 
		prev_key = key;

		/* Exit from the program if dictionary size has reached its limit */ 
		if(d->lim_code == D_MAX_DICT_LEN) {
			D_MAX_DICT_LEN *= 2;
			d->dictionary = (uint8_t **) realloc (d->dictionary, sizeof(uint8_t *) * D_MAX_DICT_LEN); 
			index_len = (uint16_t *) realloc(index_len, sizeof(uint16_t) * D_MAX_DICT_LEN);
		}
	}
	
	/* Call the free dictionary function to clear all malloced pointers */ 
	free(index_len);
	d_free_dict(d);
	
	/* Close both files */
	fclose(fp);
	fclose(op);
}

/* Appends the new string at the lim_code position of the dictionary */
/* Increments the end of dictionary (lim_code) */

void d_addto_dict(dict *d, uint8_t *str, uint16_t count) {
	d->dictionary[d->lim_code] = (uint8_t *) malloc(sizeof(uint8_t) * count);    
	
	if(d->dictionary[d->lim_code] == NULL) {
		printf("Memory limit reached.\n");
		printf("Program terminated.\n");
		exit(1);
	}
	
	index_len[d->lim_code - 256] = count; 
	memcpy(d->dictionary[d->lim_code], str, count);
	d->lim_code++;
}

/* Function to free all malloced pointers */
void d_free_dict(dict *d){
	uint16_t i = 0;
	for(i = 0; i < d->lim_code; i++) {
		free(d->dictionary[i]);
	}
	free(d->dictionary); 
}
