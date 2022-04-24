CC = gcc

main : main.o init.o show.o fetch.o decode.o execute.o handle_reg_mem.o
	gcc -o main main.o init.o show.o fetch.o decode.o execute.o handle_reg_mem.o

main.o : main.c
	gcc -c main.c

init.o : ./utils/init.c
	gcc -c ./utils/init.c

show.o : ./utils/show.c
	gcc -c ./utils/show.c

fetch.o : ./stages/fetch.c
	gcc -c ./stages/fetch.c

decode.o : ./stages/decode.c
	gcc -c ./stages/decode.c

# execute.c depends on handle_reg_mem.c
execute.o : ./stages/execute.c
	gcc -c ./stages/execute.c

handle_reg_mem.o : ./stages/handle_reg_mem.c
	gcc -c ./stages/handle_reg_mem.c

clean :
	rm *.o

cleanall :
	rm *.o main