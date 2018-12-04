fileSystem: fileSystem.o
	gcc -o fileSystem.x fileSystem.o

fileSystem.o: src/filetable.c
	gcc -c src/filetable.c

clean:
	rm *.o *.x
