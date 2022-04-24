CC = gcc

main : main.o init.o handle_reg.o show.o fetch.o decode.o
	gcc -o main main.o init.o handle_reg.o show.o fetch.o decode.o

main.o : main.c
	gcc -c main.c

init.o : ./utils/init.c
	gcc -c ./utils/init.c

handle_reg.o : ./utils/handle_reg.c
	gcc -c ./utils/handle_reg.c

show.o : ./utils/show.c
	gcc -c ./utils/show.c

fetch.o : ./stages/fetch.c
	gcc -c ./stages/fetch.c

decode.o : ./stages/decode.c
	gcc -c ./stages/decode.c

clean :
	rm *.o main