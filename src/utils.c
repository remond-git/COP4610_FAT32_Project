#include "utils.h"
#include "FATImage.h"
#include <stdio.h>
#include <stdlib.h>

FILE* ImageFile = NULL;

const int ENDOFCLUSTER = 268435448;

struct ReservedRegion rr;

unsigned int FAT_StartLoc, FAT_EndLoc;

void OpenImageFile(const char* name) {
  ImageFile = fopen(name, "r+b");
  ImageFileName = name;
  if (ImageFile == NULL) {
	  printf("Image file could not be opened...\n");
	  exit(-1);
  }	
}

void CloseImageFile() {
  if (ImageFile != NULL) {
	fclose(ImageFile);
  }
}

FILE* GetImageFile() {
  return ImageFile;
}

short GetBytesPerSec(void) {
  return rr->BPB_BytsPerSec;
}

short GetSecPerClus(void) {
  return rr->BPB_SecPerClus;
}

unsigned int GetRootClusterNum(void) {
  return rr->BPB_RootClus;
}

void ParseBootSector(void) {
  unsigned short store_bytes[4];
  unsigned char SPC[1];
  int FATOffset, ThisFATSecNum;

  fseek(ImageFile, 11, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);		
  rr->BPB_BytsPerSec = store_bytes[0];

  fseek(ImageFile, 13, SEEK_SET);
  fread(SPC, sizeof(char), 1, ImageFile);
  rr->BPB_SecPerClus = little_to_big(SPC, 1);

  fseek(ImageFile, 14, SEEK_SET);
  fread(store_bytes, sizeof(char), 2, ImageFile);
  rr->BPB_RsvdSecCnt = little_to_big(store_bytes, 2);

  fseek(ImageFile, 16, SEEK_SET);
  fread(store_bytes, sizeof(char), 1, ImageFile);
  rr->BPB_NumFATs = little_to_big(store_bytes, 1);

  fseek(ImageFile, 36, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr->BPB_FATSz32 = little_to_big(store_bytes, 4);
    
  // root cluster
  fseek(ImageFile, 44, SEEK_SET);
  fread(store_bytes, sizeof(char), 4, ImageFile);
  rr->BPB_RootClus = little_to_big(store_bytes, 4);

  FindFirstSectorOfCluster(rr->BPB_RootClus);
    
  FATOffset = rr->BPB_RootClus * 4;
  ThisFATSecNum =  rr->BPB_RsvdSecCnt + (FATOffset / rr->BPB_BytsPerSec);
    
  FAT_StartLoc = ThisFATSecNum*rr->BPB_BytsPerSec;
  FAT_EndLoc = rr->BPB_FATSz32 * rr->BPB_BytsPerSec + FAT_StartLoc;
}

int FindFirstSectorOfCluster(int n) {
  int FirstDataSector = rr->BPB_RsvdSecCnt + (rr->BPB_NumFATs * rr->BPB_FATSz32);
  int FirstSectorofCluster = ((n - 2) * rr->BPB_SecPerClus) + FirstDataSector;
  int DirLocation = FirstSectorofCluster * rr->BPB_BytsPerSec;
  return DirLocation;
}

unsigned int little_to_big(unsigned char *array, int bytes){
  unsigned int ret = 0;
  int i = 0;
  while(i < bytes) {
	  ret += (unsigned int)(array[i])<<(i*8);
	  i++;
  }
  return ret;
}

unsigned char* big_to_little(unsigned int value, unsigned int size)
{
  static unsigned char ret[4];
  memset(ret, 0, 4);
  unsigned int i = 0;
  unsigned int mask = 0x000000FF;
  unsigned int tmp_val = value;
	while(i < size) {
	  ret[i] = tmp_val&mask;
	  tmp_val = tmp_val >> 8;
	  i++;
	}
	return ret;
}

void WriteIntToImage(unsigned int value, unsigned int offset) {
  fseek(ImageFile, offset, SEEK_SET);
  unsigned char* tmp = big_to_little(value, 4);
  int i = 0;
  while(i < 4) {
	fputc(tmp[i], ImageFile);
	i++;
  }
}

void WriteShortToImage(unsigned short value, unsigned int offset) {
  fseek(ImageFile, offset, SEEK_SET);
  unsigned char* tmp = big_to_little(value, 2);
  int i = 0;
  while(i < 2) {
    fputc(tmp[i], ImageFile);
	i++;
  }
}

void WriteCharToImage(unsigned char value, unsigned int offset) {
  fseek(ImageFile, offset, SEEK_SET);
  fputc(value, ImageFile);
}

unsigned int FindNextFreeCluster(void) {
  unsigned int cluster_value;
  unsigned short store_bytes[4];
  unsigned int seek_pos = FAT_StartLoc;
  unsigned int cluster_number = 0;
    
    do {
      fseek(ImageFile, seek_pos, SEEK_SET);
      fread(store_bytes, sizeof(char), 4, ImageFile);
      cluster_value = little_to_big(store_bytes, 4);
      seek_pos += 4;
      ++cluster_number;
    }
    while(cluster_value != 0x00000000 || seek_pos >= FAT_EndLoc);

    if(seek_pos == FAT_EndLoc) {
      printf("ERROR: No available clusters, image is FULL.\n");
	  return 0xFFFFFFFF;
    }

    cluster_number--;
    return cluster_number;
}

void WriteToFAT(unsigned int cluster_number, unsigned int next_cluster) {
  unsigned int byteOffsetTable1 = FAT_Start()+cluster_number*4;
  unsigned int byteOffsetTable2 = FAT_End()+cluster_number*4;
  WriteIntToImage(next_cluster, byteOffsetTable1);
  WriteIntToImage(next_cluster, byteOffsetTable2);
}

unsigned int FindFirstFreeDirectoryEntry(unsigned int clusterNum) {
  unsigned int currentClusterIndex = clusterNum;
  unsigned int previousClusterIndex = clusterNum;
  do {
	unsigned int byteStart = FindFirstSectorOfCluster(currentClusterIndex);
	unsigned int byteOffset = 0;
	char rawData[1];
	do {
	  fseek(ImageFile, byteStart + byteOffset, SEEK_SET);
	  fread(rawData, sizeof(char), 1, ImageFile);
	  byteOffset += 32;
	} while (*rawData != 0xE5 && *rawData != 0x00 && byteOffset < GetBytesPerSec()*GetSecPerClus());
	if (*rawData == 0xE5 || *rawData == 0x00) {
	  return byteStart+byteOffset-32;
	}
	previousClusterIndex = currentClusterIndex;
	currentClusterIndex = next_cluster(currentClusterIndex);
  } while (currentClusterIndex < 0x0FFFFFF8);
  unsigned int allocatedCluster = FindNextFreeCluster();
  WriteToFAT(previousClusterIndex, allocatedCluster);
  WriteToFAT(allocatedCluster, 0x0FFFFFF8);
  return FindFirstSectorOfCluster(allocatedCluster);
}

unsigned int FAT_Start(void) {
  return FAT_StartLoc;
}

unsigned int FAT_End(void) {
  return FAT_EndLoc;
}
