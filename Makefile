all: encode decode

encode: e_main.c encode.c encode.h
	gcc -o encode e_main.c encode.c -Wall -g

decode: d_main.c decode.c decode.h
	gcc -o decode d_main.c decode.c -Wall -g
