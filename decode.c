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
/* Decompression */
/* The program takes a file name as an argument and a dictionary. */
/* This Decompression algorithm uses a dictionary to store the byte sequence 
 * by reading the 16 bit numbers stored in the ".mtz" file. */

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
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
	/* Exit from the program if No file exists */
	if(fp == NULL) {
		perror("File open failed\n");
		exit(1);
	}
	return fp;
}

FILE * d_open_op_file(char *fname) {
	FILE *fp;
	fp = fopen(fname, "w");
	/* Exit from the program file open fails */
	if(fp == NULL) {
		perror("File open failed\n");
		exit(1);
	}
	return fp;
}

/* Main Decompression Function */
void decode(dict *d, char *fname, char *op_fname) {
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
	
	strcpy(file, fname);
	/* Remove the ".mtz" extension from the input to write to the output file */
	uint16_t len = strlen(op_fname);
	op_fname[len - 4] = '\0';

	/* Open the compressed file with format ".mtz" */
	fp = d_open_file(file);
	op = d_open_op_file(op_fname);
	
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

		/* Realloc the dictionary if dictionary size has reached its limit */ 
		if(d->lim_code == D_MAX_DICT_LEN) {
			D_MAX_DICT_LEN *= 2;
			d->dictionary = (uint8_t **) realloc (d->dictionary, sizeof(uint8_t *) * D_MAX_DICT_LEN); 
			index_len = (uint16_t *) realloc(index_len, sizeof(uint16_t) * D_MAX_DICT_LEN);
		}
	}
	
	/* Call the free dictionary function to clear all malloced pointers */ 
	free(index_len);
	d_free_dict(d);
	free(file);
	
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

void dir_decode(char *dir_name, char *op_dir_name) {
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
	/* Remove the ".mtz" extension */
	op_dir[op_dir_len - 5] = '\0';
	strcat(op_dir, "/");
	mkdir(op_dir, 0777);

	while ((de = readdir(dr)) != NULL) {
		if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
				continue;
		if((de->d_type != DT_DIR)) {
			d_init(&d);
			strcpy(string_ip, dir_name);
			strcat(string_ip, de->d_name);
			strcpy(string_op, op_dir);
			strcat(string_op, de->d_name);
			decode(&d, string_ip, string_op);
		}
		else {
			strcpy(string_ip, dir_name);
			strcat(string_ip, de->d_name);		
			strcpy(string_op, op_dir);
			strcat(string_op, de->d_name);
			dir_decode(string_ip, string_op);
		}
	}
	closedir(dr);		
}

/* Function to free all malloced pointers */
void d_free_dict(dict *d){
	uint16_t i = 0;
	for(i = 0; i < d->lim_code; i++) {
		free(d->dictionary[i]);
	}
	free(d->dictionary); 
}
