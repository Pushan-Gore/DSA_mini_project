/***** Pushan Gore *****/
/*****  111503023  *****/
/*****  __MAIN_C   *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "decode.h"
#include "encode.h"

int main(int argc, char *argv[]) {
	char *_encode = "-c";
	char *_mul_encode = "-C";
	char *_decode = "-x";
	char *_mul_decode = "-X";
	char *_help1 = "-h";
	char *_help2 = "--help";
	char *help = "Usage: ./project [OPTION]... [FILE]...\n\
	Mandatory arguments \n\
  	-c            Compress the given file\n\
	-C			  Compress Multiple files as arguments\n\
				  Usage: ./project -C... [FILE_1]... [FILE_2]... [FILE_3]...\n\
  	\n\
	-x            Decompress the given file\n\
	-X			  Decompress Multiple files as arguments\n\
				  Usage: ./project -X... [FILE_1]... [FILE_2]... [FILE_3]...\n\
      	\n\
	-h, --help    Display this help and exit \n";
	
	dict d;
	
	/* Return when command line arguments are less than or more than 3 */
	if(argc < 3) {
		printf("Incorrect Usage.\n");
		printf("Try './project --help' or './project -h' for more information\n"); 
		exit(1);
	}

	/* Display the help ad exit */
	if(!strcmp(argv[1], _help1) || !strcmp(argv[1], _help2)) {
		printf("%s", help);
		return 0;
	}

	/* project -c <FILENAME> */
	if(!strcmp(argv[1], _encode)) {
		e_init(&d);
		encode(&d, argv[2]);
	}	
	
	/* project -x <FILENAME> */
	else if(!strcmp(argv[1], _decode)) {
		d_init(&d);
		decode(&d, argv[2]);
	}

	else if(!strcmp(argv[1], _mul_encode)) {
		int j;
		char fname[128];
		for(j = 2; j < argc - 1; j++) {
			strcpy(fname, argv[j+1]);
			e_init(&d);
			encode(&d, argv[j]);		
			strcpy(argv[j+1], fname);
		}		
		e_init(&d);
		encode(&d, argv[j]);		
	}
	else if(!strcmp(argv[1], _mul_decode)) {
		int j;
		char fname[128];
		for(j = 2; j < argc - 1; j++) {
			strcpy(fname, argv[j+1]);
			d_init(&d);
			decode(&d, argv[j]);		
			strcpy(argv[j+1], fname);
		}		
		d_init(&d);
		decode(&d, argv[j]);		
	}
	else {
		printf("Incorrect Usage.\n");
		printf("Try './project --help' or './project -h' for more information\n"); 
	}

	return 0;	
}

