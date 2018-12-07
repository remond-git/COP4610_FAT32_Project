
FAT32Utility:	Shell.o utils.o dirparse.o
	gcc -o FAT32Utility.x Shell.o utils.o dirparse.o -lreadline

Shell.o: src/Shell.c include/utils.h include/dirparse.h
	gcc -c src/Shell.c

utils.o: src/utils.c include/utils.h
	gcc -c src/utils.c

dirparse.o: src/dirparse.c include/dirparse.h include/utils.h
		gcc -c src/dirparse.c

clean:
	rm *.o *.x
