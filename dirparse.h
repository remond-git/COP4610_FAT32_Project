#ifndef _DIRPARSE_H
#define _DIRPARSE_H

typedef struct DirectoryEntry {
	char DIR_Name[32];
	unsigned char DIR_Attr;
	unsigned int DIR_FstClus;
	unsigned int DIR_FileSize;
	unsigned int DIR_EntryByteAddress;
	unsigned char END_OF_ARRAY;
} DirEntryType;

DirEntryType* GetDirectoryContents(unsigned int clusterNum);
void WriteDirectoryEntry(DirEntryType dir, unsigned int clusterNum);
void PrintDirectoryVector(const DirEntryType* d);

// this functions takes the string (in FAT32 format) of a file, then returns which cluster this file starts in
// otherwise the function returns "0xFFFFFFFF"
unsigned int NameToClusterNumber(const char* name);

unsigned int GetCurrentDirectoryClusterNum();
void SetCurrentDirectoryClusterNum(unsigned int clusterNum);
unsigned int GetPreviousDirectoryClusterNum();
unsigned int PopPreviousDirectoryClusterNum();
void PushPreviousDirectoryClusterNum(unsigned int clusterNum);

unsigned int next_cluster(unsigned int);

#endif
