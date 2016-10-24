#ifndef __DICTIONARY_H
#define __DICTIONARY_H

#include <inttypes.h>
#define MAX_DICT_LEN 16000 

typedef struct dict{
	uint8_t **dictionary;   
	uint16_t lim_code;
}dict;

#endif
