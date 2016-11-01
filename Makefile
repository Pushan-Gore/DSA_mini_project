project: main.o encode.o decode.o
	gcc *.o -o project

main.o: main.c decode.h encode.h dictionary.h
	gcc -c main.c -Wall -g

encode.o: encode.c encode.h dictionary.h
	gcc -c encode.c -Wall -g

decode.o: decode.c decode.h dictionary.h
	gcc -c decode.c -Wall -g

clean:
	rm *.o
