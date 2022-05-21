CC = gcc

main : main.o init.o assess.o show.o fetch.o decode.o execute.o mem_access.o writeback.o
	gcc -o main main.o init.o assess.o show.o fetch.o decode.o execute.o mem_access.o writeback.o

main.o : main.c
	gcc -c main.c

init.o : ./utils/init.c
	gcc -c ./utils/init.c

assess.o : ./utils/assess.c
	gcc -c ./utils/assess.c

show.o : ./utils/show.c
	gcc -c ./utils/show.c

fetch.o : ./stages/fetch.c
	gcc -c ./stages/fetch.c

decode.o : ./stages/decode.c
	gcc -c ./stages/decode.c

execute.o : ./stages/execute.c
	gcc -c ./stages/execute.c

mem_access.o : ./stages/mem_access.c
	gcc -c ./stages/mem_access.c

writeback.o: ./stages/writeback.c
	gcc -c ./stages/writeback.c

clean :
	rm *.o

cleanall :
	rm *.o main