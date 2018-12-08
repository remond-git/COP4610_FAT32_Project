#include "size.h"
#include "utils.h"
#include <string.h>
#include "dirparse.h"

void size(const char* filename) {
  char parsedDir[32];
  strcpy(parsedDir, filename);
  ToFAT32(parsedDir);
  DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
  unsigned int i = 0;
  while (!tmp[i].END_OF_ARRAY) {
    if (strcmp(tmp[i].DIR_Name, parsedDir) == 0) {
	  printf("%d\n", tmp[i].DIR_FileSize);
	  return;
	}
	i++;
  }
  printf("Error: file does not exist!\n");
}
