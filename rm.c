#include "rm.h"
#include "utils.h"
#include "dirparse.h"
#include "cd.h"
#include <string.h>

const unsigned int EMPTY_BYTE = 0xE5;

void rm (char * fname) {
  unsigned int firstClusterNum;
  unsigned int dirAddress;
    
  DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());

  int found = 0;
  int i = 0;
  while(!tmp[i].END_OF_ARRAY) {
	if(strcmp(tmp[i].DIR_Name, fname) == 0) {
	  if (tmp[i].DIR_Attr&0x10) {
		printf("Cannot use rm on a directory...\n");
		return;
	  }
	  else {
		found = 1;
		break;
	  }
    }
	i++;
  }
  if (found == 0) {
    printf("File does not exist...\n");
	return;
  }

  WriteCharToImage(EMPTY_BYTE, tmp[i].DIR_EntryByteAddress);
  firstClusterNum = tmp[i].DIR_FstClus;
  RemoveFromFAT(firstClusterNum);
}

void rmdir (char * dirname) {
  unsigned int firstClusterNum;
  unsigned int dirAddress;
    
  DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
  
  int found = 0;
  int i = 0;
  while(!tmp[i].END_OF_ARRAY) {
	if (strcmp(tmp[i].DIR_Name, dirname) == 0) {
	  if (!tmp[i].DIR_Attr&0x10) {
		printf("Cannot use rmdir on a file...\n");
		return;
	  }
	  else {
		found = 1;
		break;
	  }
	}
	i++;
  }
  if (found == 0) {
	printf("Directory does not exist...\n");
	return;
  }

  cd(dirname);
  tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
  int j = 0;
  while(!tmp[j].END_OF_ARRAY) {
    j++;
  }
  if (j > 2) {
    printf("Directory not empty\n");
	cd("..");
	return;
  }
  cd("..");
  tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());

  WriteCharToImage(EMPTY_BYTE, tmp[i].DIR_EntryByteAddress);
  firstClusterNum = tmp[i].DIR_FstClus;
  RemoveFromFAT(firstClusterNum);
}

void ZeroDataEntry(unsigned int clusterNum) {
  FILE* ImageFile = GetImageFile();
  unsigned int byteAddress = FindFirstSectorOfCluster(clusterNum);
  unsigned int numBytes = GetBytesPerSec()*GetSecPerClus();
  fseek(ImageFile, byteAddress, SEEK_SET);
  unsigned int i = 0;
  while(i < numBytes) {
        fputc('\0', ImageFile);
        i++;
  }
}

void RemoveFromFAT(unsigned int cluster_number) {
  unsigned int clusterNums[65536];
  unsigned short i = 0;
  int i2 = i-1;
  unsigned int temp = cluster_number;
  do {
    clusterNums[i] = temp;
    temp = next_cluster(temp);
    i++;
  }
  while(temp < 0x0FFFFFF8);

  while(i2 >= 0) {
    ZeroDataEntry(clusterNums[i2]);
    WriteToFAT(clusterNums[i2], 0);
	i2--;
  }
}
