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
	char *_decode = "-x";
	char *_help1 = "-h";
	char *_help2 = "--help";
	char *help = "Usage: ./project [OPTION]... [FILE]...\n\
	Mandatory arguments \n\
  	-c            Compress the given file\n\
  	-x            Decompress the given file\n\
      	\n\
	-h, --help    Display this help and exit \n";
	dict d;

	/* Display the help ad exit */
	if(!strcmp(argv[1], _help1) || !strcmp(argv[1], _help2)) {
		printf("%s", help);
		return 0;
	}

	/* Return when command line arguments are less than or more than 3 */
	if(argc != 3) {
		printf("Incorrect Usage.\n");
		printf("Try './project --help' or './project -h' for more information\n"); 
		exit(1);
	}

	/* project -c <FILENAME> */
	if(!strcmp(argv[1], _encode)) {
		e_init(&d);
		encode(&d, argv[2]);
	}	
	
	/* project -x <FILENAME> */
	if(!strcmp(argv[1], _decode)) {
		d_init(&d);
		decode(&d, argv[2]);
	}

	return 0;	
}

