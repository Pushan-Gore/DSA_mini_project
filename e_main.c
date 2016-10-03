#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"

int main(int argc, char *argv[]) {
	dict d;
	d_init(&d);
	encode(&d, argv[1]);
	return 0;
}
