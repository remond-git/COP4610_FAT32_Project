FAT32Utility: Shell.o
	gcc -o FAT32Utility.x Shell.o

Shell.o: src/Shell.c
	gcc -c src/Shell.

clean:
	rm *.o *.x
