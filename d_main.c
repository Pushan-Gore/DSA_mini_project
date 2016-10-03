#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

int main(int argc, char *argv[]) {
	dict d;
	d_init(&d);
	decode(&d, argv[1]);
	return 0;
}
