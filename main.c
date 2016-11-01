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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "dictionary.h"
#include "decode.h"
#include "encode.h"

int main(int argc, char *argv[]) {
	char *_encode = "-c";
	char *_mul_encode1 = "-C";
	char *_mul_encode2 = "--Call";
	char *_decode = "-x";
	char *_mul_decode1 = "-X";
	char *_mul_decode2 = "--Xall";
	char *_help1 = "-h";
	char *_help2 = "--help";

	char *help = "Usage: ./project [OPTION]... [FILE]...\n\
	Mandatory arguments \n\
  	-c		Compress the given file\n\
	-C, --Call	Compress ALL files in the directory given as command line argument \n\
					Usage: ./project -C <DIRECTORY> \n\
  	\n\
	-x		Decompress the given file\n\
	-X, --Xall	Decompress ALL files with '.mtz' extension in the directory given as command line argument \n\
					Usage: ./project -X <DIRECTORY> \n\
    \n\
	-h, --help	Display this help and exit \n";
	
	dict d;
	
	/* Return when command line arguments are equal to 1 or greater than 3 */
	if(argc == 1 || argc > 3) {
		printf("Incorrect Usage.\n");
		printf("Try './project --help' or './project -h' for more information\n"); 
		exit(1);
	}
	
	/* Display the help and exit */
	if((argc == 2) && (!strcmp(argv[1], _help1) || !strcmp(argv[1], _help2))) {
		printf("%s", help);
		return 0;
	}
	
	/* project -c <FILENAME> */
	if((argc == 3) && (!strcmp(argv[1], _encode))) {
		e_init(&d);
		encode(&d, argv[2]);
		return 0;
	}	
	
	/* project -x <FILENAME> */
	else if((argc == 3) && (!strcmp(argv[1], _decode))) {
		d_init(&d);
		decode(&d, argv[2]);
		return 0;
	}

	/* Encoding all files in current directory */	
	/* project -C */
	struct dirent *de;  
	DIR *dr = opendir(argv[2]);
	if(dr == NULL) {
			printf("Directory open failed.\nNo such directory exists\n");
			exit(1);
	}
	char string[128];
	int dir_len = strlen(argv[2]);
	if(argv[2][dir_len - 1] != '/') {
			strcat(argv[2], "/");
	} 

	if((argc == 3) && ((!strcmp(argv[1], _mul_encode1)) || (!strcmp(argv[1], _mul_encode2)))) {
		while ((de = readdir(dr)) != NULL) {
			if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
					continue;
			if((de->d_type != DT_DIR)) {
				e_init(&d);
				strcpy(string, argv[2]);
				strcat(string, de->d_name);
				encode(&d, string);
			}
		}
		closedir(dr);		
		return 0;
	}

	/* Decoding all ".mtz" files in the directory */
	/* project -X */
	char *token, *ext = "mtz";
	char file[128], flag = 'r';

	if((argc == 3) && ((!strcmp(argv[1], _mul_decode1)) || (!strcmp(argv[1], _mul_decode2)))) {
		while ((de = readdir(dr)) != NULL) {
			if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
				continue;
	 		strcpy(file, de->d_name);	
			token = strtok(file, ".");
			while(token != NULL) {
				if(!strcmp(token, ext))
					flag = 's'; /* Flag is set */
				else
					flag = 'r';
				token = strtok(NULL, ".");
			}
			if(flag == 'r')
				continue;
			if((de->d_type) != DT_DIR) { 
				d_init(&d);
				strcpy(string, argv[2]);
				strcat(string, de->d_name);
				decode(&d, string);
			}
		}
		closedir(dr);		
		return 0;
	}

	printf("Incorrect Usage.\n");
	printf("Try './project --help' or './project -h' for more information\n"); 
	return 0;	
}

