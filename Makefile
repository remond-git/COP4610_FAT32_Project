FLAGS = -std=c99

all:	exe

exe:	main.o utils.o filetable.o dirparse.o parse.o program.o
	gcc $(FLAGS) -o FATprog.x main.o utils.o filetable.o dirparse.o parse.o program.o

main.o:	main.c
	gcc $(FLAGS) -c main.c

filetable.o: filetable.c filetable.h
	gcc $(FLAGS) -c filetable.c

utils.o: utils.c utils.h
	gcc $(FLAGS) -c utils.c

parse.o: parse.c parse.h
	gcc $(FLAGS) -c parse.c

dirparse.o: dirparse.c dirparse.h
	gcc $(FLAGS) -c dirparse.c
	
ls.o: ls.c ls.h
	gcc $(FLAGS) -c ls.c
	
creat.o: creat.c creat.h
	gcc $(FLAGS) -c creat.c

program.o: program.c program.h
	gcc $(FLAGS) -c program.c

clean:
	rm *.o *.x
