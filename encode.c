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

void *memcat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (char*)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

void e_init(dict *d) {
	/* Initially the Max Limit is 4096 */
	d->dictionary = (char **) malloc (sizeof(char *) * MAX_DICT_LEN);   
	uint16_t ind;
	
	/* Initializing idices 0 to 255 to respective characters */
	for(ind = 0; ind < 255; ind++) {
		d->dictionary[ind] = (char *) malloc(sizeof(char) * 1); 
		d->dictionary[ind][0] = ind;
	}
	d->lim_code = 256;
}

FILE * e_open_file(char *fname) {
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
	fp = e_open_file(fname);

	/* Append ".mtz" extension to the output file */
	op = fopen(strcat(fname, ".mtz"), "w");	
	
	char str[128], temp[128], arr[2];
	char ch1, ch2;
	uint16_t code;
	int count;

	fread(&ch1, 1, 1, fp);
	arr[0] = ch1;
	memcpy(str, arr, 1);  
	count = 1;
	while(fread(&ch2, 1, 1, fp)) {    
		arr[0] = ch2;
		memcpy(temp, str, count);    
		if(e_search_dict(d,memcat(str, count, arr, 1), count + 1) == MAX_DICT_LEN) {            
			code = e_search_dict(d, temp, count); 
			fwrite(&code, sizeof(uint16_t), 1, op);
			e_addto_dict(d, str, count + 1);
			memcpy(str, arr, 1);    
			count = 1;
		}
		else
			count++;
		
	}
	if(d->lim_code == MAX_DICT_LEN) {
		printf("End of dictionary length.\n");
	}
	code = e_search_dict(d, str, count);
	fwrite(&code, sizeof(uint16_t), 1, op);
	// Write a free function.
	free(d->dictionary);
	fclose(fp);
	fclose(op);
}

uint16_t e_search_dict(dict *d, char *str, int len) {      
	if(len == 1) {                  
		return (uint16_t)str[0];	 
	}
	uint16_t i;
	for(i = 256; i < d->lim_code; i++) {
		if(!memcmp(str, d->dictionary[i], len))      
			return i;			
	}
	return MAX_DICT_LEN;
}

/* Appends the new string at the lim_code position of the dictionary */
void e_addto_dict(dict *d, char *str, int count) {
	d->dictionary[d->lim_code] = (char *) malloc(count);    
	if(d->lim_code == MAX_DICT_LEN) {  
		exit(1);
	}
	memcpy(d->dictionary[d->lim_code], str, count);
	d->lim_code++;
}

