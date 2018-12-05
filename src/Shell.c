#include <stdio.h>
#include "../include/FATImage.h"
#include "../include/filetable.h"

int OpenFAT32Image(char* filename);

void Exit(void);
void Info(void);
void Ls(char* directoryName);
void Cd(char* directoryName);
void Size(char* filename);
void Creat(char* filename);
void Mkdir(char* directoryName);
void Open(char* filename, char* mode);
void Close(char* filename);
void Read(char* filename, unsigned int offset, unsigned int size);
void Write(char* filename, unsigned int offset, unsigned int size, char* string);
void Rm(char* filename);
void Rmdir(char* directoryName);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Error: No FAT32 file system image supplied.\n");
    } else if (argc == 2) {
        if !OpenFAT32Image(argv[1]) {
            printf("Error: Could not open FAT32 file system image.\n");
        }
    }
    
    Exit();
}

void OpenFAT32Image(char* filename) {

}

void Exit() {
    return;
}
