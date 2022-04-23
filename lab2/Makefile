CC = gcc

main : main.o init.o handle_reg.o fetch.o decode.o
	gcc -o main main.o init.o handle_reg.o fetch.o decode.o

main.o : main.c
	gcc -c main.c

init.o : init.c
	gcc -c init.c

handle_reg.o : handle_reg.c
	gcc -c handle_reg.c

fetch.o : fetch.c
	gcc -c fetch.c

decode.o : decode.c
	gcc -c decode.c

clean :
	rm *.o main