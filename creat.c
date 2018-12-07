#include "dirparse.h"
#include "utils.h"
#include "creat.h"

void creat(const char* name) {
  DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
  int i = 0;
  while(!tmp[i].END_OF_ARRAY) {
	if (strcmp(tmp[i].DIR_Name, name) == 0) {
	  printf("File already exists!\n");
	  return;
	}
	i++;
  }
  
  DirEntryType ent;
  strcpy(ent.DIR_Name, name);
  ent.DIR_Attr = 0x00;
  unsigned int firstCluster = FindNextFreeCluster();
  if (firstCluster == 0xFFFFFFFF) {
    return;
  }
  WriteToFAT(firstCluster, 0x0FFFFFF8);
  ent.DIR_FstClus = firstCluster;
  ent.DIR_FileSize = 0x0000;
  WriteDirectoryEntry(ent, GetCurrentDirectoryClusterNum());
}
