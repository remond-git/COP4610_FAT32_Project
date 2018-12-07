#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "dirparse.h"
#include "utils.h"
#include "program.h"
#include "ls.h"

void list(unsigned int clusterNum) {
  DirEntryType* tmp = GetDirectoryContents(clusterNum);
  unsigned int i = 0;
  while (!tmp[i].END_OF_ARRAY) {
    char tempBuff[32];
    memset(tempBuff, '\0', 32);
    strcpy(tempBuff, tmp[i++].DIR_Name);
    ToStandardFormat(tempBuff);
    printf("%s\n", tempBuff);
  }
}

void ls(const char* path) {
  if (strcmp(path, ".") == 0) {
    list(GetCurrentDirectoryClusterNum());
  }
  else if (strcmp(path, "..") == 0) {
	unsigned int prevClus = GetPreviousDirectoryClusterNum();
    if (prevClus != 0xFFFFFFFF) {
	  list(prevClus);
	}
  }
  else {
    DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int i = 0;
	unsigned int found = 0;
	char parsedDir[USER_INPUT_BUFFER_LENGTH];
	strcpy(parsedDir, path);
	ToFAT32(parsedDir);
	while (!tmp[i].END_OF_ARRAY) {
	  if (strcmp(parsedDir, tmp[i].DIR_Name) == 0 &&
		tmp[i].DIR_Attr & 0x10) 
		    {
			  list(tmp[i].DIR_FstClus);
			  found = 1;
			}
		  i++;
	  }
	if (!found) {
	  printf("Directory not found...\n");
	}
  }
}

