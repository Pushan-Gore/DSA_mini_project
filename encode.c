/*
 **********************************************************************
 * Copyright (C) 2016  Pushan Gore (111503023)

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *********************************************************************** */
/* Compression */
/* The program takes a file name as an argument and a dictionary. */
/* This Compression algorithm uses a dictionary to store the byte sequence */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <dirent.h>
#include <sys/stat.h>
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

FILE * e_open_op_file(char *fname) {
	FILE *fp;
	fp = fopen(fname, "w");
	/* Exit from the program if file open failed */
	if(fp == NULL) {
		perror("File open failed\n");
		exit(1);
	}
	return fp;
}

/* Main Compression Function */
void encode(dict *d, char *fname, char *op_fname, int mode) {
	FILE *fp, *op;
	char file[128];
	strcpy(file, fname);
	fp = e_open_file(fname);
	op = e_open_op_file(strcat(op_fname, ".mtz"));  /* Append ".mtz" to the file name */ 

	unsigned long len = fsize(file);
	unsigned long byte = 1;

	uint8_t str[16000], temp[16000], arr[1];
	uint16_t code;
	uint16_t count;
	
	index_len = (uint16_t *) malloc(sizeof(uint16_t) * C_MAX_DICT_LEN);

	/* Main Compression Loop */
	printf("\n");
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

	if(mode == FIL) {
		printf("\nOutput file : %s\n", op_fname);
		printf("Compression ratio -> %lf : 1", ((double) len/ (double) (fsize(op_fname))));
		printf("\n\n");
	}

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

/* Function compresses the entire folder into a single folder with .mtz extension */
void dir_encode(char *dir_name, char *op_dir_name, int mode) {
	dict d;
	struct dirent *de;  
	DIR *dr = opendir(dir_name);
	if(dr == NULL) {
			printf("Directory open failed.\nNo such directory exists\n");
			exit(1);
	}
	char string_ip[128], string_op[128], op_dir[128];

	int dir_len = strlen(dir_name);
	int op_dir_len = strlen(op_dir_name);
	if(dir_name[dir_len - 1] != '/') {
			strcat(dir_name, "/");
			dir_len++;
	}
	if(op_dir_name[op_dir_len - 1] != '/') {
			strcat(op_dir_name, "/");
			op_dir_len++;
	}
	strcpy(op_dir, op_dir_name);
	op_dir[op_dir_len - 1] = '\0';
	strcat(op_dir, ".mtz");
	strcat(op_dir, "/");
	mkdir(op_dir, 0777);

	while ((de = readdir(dr)) != NULL) {
		if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
				continue;
		if((de->d_type != DT_DIR)) {
			e_init(&d);
			strcpy(string_ip, dir_name);
			strcat(string_ip, de->d_name);
			strcpy(string_op, op_dir);
			strcat(string_op, de->d_name);
			encode(&d, string_ip, string_op, DIRECT);
		}
		else {
			strcpy(string_ip, dir_name);
			strcat(string_ip, de->d_name);		
			strcpy(string_op, op_dir);
			strcat(string_op, de->d_name);
			dir_encode(string_ip, string_op, DIRECT);
		}
	}
	closedir(dr);		
}

/* Function to free all malloced pointers */
void e_free_dict(dict *d){
	uint16_t i = 0;
	for(i = 0; i < d->lim_code; i++) {
		free(d->dictionary[i]);
	}
	free(d->dictionary); 
}
