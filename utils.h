#ifndef _UTILS_H
#define _UTILS_H
#include <stdio.h>

char* ImageFileName;

void OpenImageFile(const char* name);
void CloseImageFile();
FILE* GetImageFile();
short GetBytesPerSec(void);
short GetSecPerClus(void);
unsigned int GetRootClusterNum(void);
int FindFirstSectorOfCluster(int n);
void ParseBootSector(void);
void PrintBootSector(void);
void FindRootDirectory(void);

unsigned int little_to_big(unsigned char*, int);
unsigned char* big_to_little(unsigned int value, unsigned int size);
void WriteIntToImage(unsigned int value, unsigned int offset);
void WriteShortToImage(unsigned short value, unsigned int offset);
void WriteCharToImage(unsigned char value, unsigned int offset);
unsigned int FindNextFreeCluster(void);
void WriteToFAT(unsigned int cluster_number, unsigned int next_cluster);
unsigned int FindFirstFreeDirectoryEntry(unsigned int clusterNum);

unsigned int FAT_Start();
unsigned int FAT_End();

#endif
