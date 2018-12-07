#include <string.h>
#include "dirparse.h"
#include "utils.h"
#include "program.h"
#include "cd.h"

void cd(const char* path){
  if (strcmp(path, "..") == 0) {
    unsigned int prevClus = PopPreviousDirectoryClusterNum();
	if (prevClus == 0xFFFFFFFF) {}
	else {
	  SetCurrentDirectoryClusterNum(prevClus);
	}
  }
  else {
	DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int i = 0;
	unsigned char found = 0;
    char parsedDir[USER_INPUT_BUFFER_LENGTH];
	strcpy(parsedDir, path);
	ToFAT32(parsedDir);
	while (!tmp[i].END_OF_ARRAY) {
	  if (strcmp(parsedDir, tmp[i].DIR_Name) == 0 && tmp[i].DIR_Attr & 0x10) {
	    PushPreviousDirectoryClusterNum(GetCurrentDirectoryClusterNum());
		SetCurrentDirectoryClusterNum(tmp[i].DIR_FstClus);
		found = 1;
	  }
	  i++;
	}
	if (found == 0) {
	  printf("Not a valid directory...\n");
	}
  }
}
