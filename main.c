#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "decode.h"
#include "encode.h"

int main(int argc, char *argv[]) {
	char *_encode = "-c";
	char *_decode = "-x";
	char *_help1 = "-h";
	char *_help2 = "--help";
	char *help = "Usage: ./project [OPTION]... [FILE]...\n\
	Mandatory arguments \n\
  	-c           Compress the given file\n\
  	-x           Decompress the given file\n\
      	\n\
	-h    	     Display this help and exit \n";
	dict d;

	if(!strcmp(argv[1], _help1) || !strcmp(argv[1], _help2)) {
		printf("%s", help);
		return 0;
	}

	if(argc != 3) {
		printf("Incorrect Usage.\n");
		printf("Try './project --help' or './project -h' for more information\n"); 
		exit(1);
	}

	if(!strcmp(argv[1], _encode)) {
		e_init(&d);
		encode(&d, argv[2]);
	}	
	
	if(!strcmp(argv[1], _decode)) {
		char arr[20];
		strcpy(arr, argv[2]);
		/*char *token = strtok(arr, ".");
		while(token) {
			if(!strcmp(token, "mtz")) {
				printf("Incorrect extension\n");
				printf("Extension should be .mtz\n");
				exit(1);	
			}
			token = strtok(arr, ".");
		}*/
		d_init(&d);
		decode(&d, argv[2]);
	}	
}

