/***** Pushan Gore *****/
/*****  111503023  *****/

/* Compression */
/* The program takes a file name as an argument and a dictionary. */
/* This Compression algorithm uses a dictionary to store the byte sequence */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <error.h>
#include "encode.h"
#include "dictionary.h"

int C_MAX_DICT_LEN = MAX_LEN;

/* Stores the length of the byte sequence */
uint16_t *index_len;


/* Memory concatination function. */
/* Same as strcat. */

void *memcat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (uint8_t *)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

/* Returns file size */
/* Returns the size of the file "file" in bytes */
unsigned long fsize(char *file) {
	FILE *f = fopen(file, "r");
	fseek(f, 0 , SEEK_END);
	unsigned long len = (unsigned long) ftell(f);
	fclose(f);
	return len;	
}

/* Initializes the dictionary 'd' */
/* The first 256 data entries are also initiated here */
/* Code limit (lim_code) is the number of dictionary entries */

void e_init(dict *d) {
	/* Malloc the dictionary of pointers with MAX_DICT_LEN as 16000 */ 
	d->dictionary = (uint8_t **) malloc (sizeof(uint8_t *) * C_MAX_DICT_LEN);   
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

/* Main Compression Function */
void encode(dict *d, char *fname) {
	FILE *fp, *op;
	char file[128];
	strcpy(file, fname);
	
	fp = e_open_file(fname);
	
	unsigned long len = fsize(fname);
	unsigned long byte = 1;

	/* Append ".mtz" extension to the output file */
	op = fopen(strcat(fname, ".mtz"), "w");	
	/* Exit from the program if No file exists */
	if(op == NULL) {
		perror("File open failed\n");
		exit(1);
	}	

	uint8_t str[16000], temp[16000], arr[1];
	uint16_t code;
	uint16_t count;
	
	printf("\n");
 
	index_len = (uint16_t *) malloc(sizeof(uint16_t) * C_MAX_DICT_LEN);

	/* Main Compression Loop */
	fread(arr, sizeof(uint8_t), 1, fp);
	memcpy(str, arr, sizeof(uint8_t));  
	count = sizeof(uint8_t);

	while(fread(arr, sizeof(uint8_t), 1, fp)) {
		memcpy(temp, str, count);    
		
		if(e_search_dict(d, memcat(str, count, arr, sizeof(uint8_t)), count + sizeof(uint8_t)) == C_MAX_DICT_LEN) {            
			code = e_search_dict(d, temp, count); 
			fwrite(&code, sizeof(uint16_t), 1, op);
			e_addto_dict(d, str, count + sizeof(uint8_t));
			memcpy(str, arr, sizeof(uint8_t));    
			count = sizeof(uint8_t);
		} 
		else {
			count = count + sizeof(uint8_t);
		}
		
		if(byte + 1 == len){
			printf("\r"); 
			printf("Compression on '%s' [%ld%%] Done.",file ,((byte + 1)*100)/len);
			fflush(stdout);
		}
		else {
			printf("\r"); 
			printf("Compression on '%s' [%ld%%] Done...",file, ((byte + 1)*100)/len);
			fflush(stdout);
			byte++;
		}

		/* Realloc the dictionary if dictionary size has reached its limit */ 
		if(d->lim_code == C_MAX_DICT_LEN) {
			C_MAX_DICT_LEN *= 2;
			d->dictionary = (uint8_t **) realloc (d->dictionary, sizeof(uint8_t *) * C_MAX_DICT_LEN);   
			index_len = (uint16_t *) realloc(index_len, sizeof(uint16_t) * C_MAX_DICT_LEN);
			if(d->dictionary == NULL) {
				printf("Memory limit reached.\n");
				exit(1);
			}
		}
	}
	code = e_search_dict(d, str, count);
	fwrite(&code, sizeof(uint16_t), 1, op);

	printf("\nOutput file : %s\n", fname);
	printf("Compression ratio -> %lf : 1", ((double) len/ (double) (fsize(fname))));
	printf("\n\n");

	/* Call the free dictionary function to clear all malloced pointers */ 
	free(index_len);
	e_free_dict(d);

	/* Close both files */
	fclose(fp);
	fclose(op);
}

/* Searches the dictionary for occurance of str */
/* Returns MAX_DICT_LEN if no occurance is found */
/* Else returns the index of the found occurance */	

uint16_t e_search_dict(dict *d, uint8_t *str, uint16_t len) {      
	if(len == 1) 
		return (int) str[0];
 
	uint16_t i;
	for(i = d->lim_code - 1; i > 255; i--) {
		if((len == index_len[i - 256]) && !memcmp(str, d->dictionary[i], len)){     
			return i;			
		}
	}
	return C_MAX_DICT_LEN;
}

/* Appends the new string at the lim_code position of the dictionary */
/* Increments the end of dictionary (lim_code) */

void e_addto_dict(dict *d, uint8_t *str, uint16_t count) {
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
void e_free_dict(dict *d){
	uint16_t i = 0;
	for(i = 0; i < d->lim_code; i++) {
		free(d->dictionary[i]);
	}
	free(d->dictionary); 
}
