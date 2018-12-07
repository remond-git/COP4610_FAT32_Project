
FAT32Utility:	Shell.o utils.o
	gcc -o FAT32Utility.x Shell.o utils.o

Shell.o: src/Shell.c include/utils.h
	gcc -c src/Shell.c

utils.o: src/utils.c include/utils.h
	gcc -c src/utils.c

clean:
	rm *.o *.x
