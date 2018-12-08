#include "open.h"
#include "filetable.h"
#include "dirparse.h"

void open(const char* file_name, const char* mode) {
  if (strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0 &&
    strcmp(mode, "rw") != 0 && strcmp(mode, "wr") != 0)
  {
    printf("Invalid mode, options are: r w rw wr\n");
  }
  else if (FTIsOpen(file_name)) {
	printf("File already open.\n");
  }
  else {
	DirEntryType* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int i = 0;
	while (!tmp[i].END_OF_ARRAY) {
	  if (strcmp(file_name, tmp[i].DIR_Name) == 0) {
	    if (tmp[i].DIR_Attr & 0x10) {
		  printf("Cannot open a directory\n");
		}
		else {
		  FTAdd(file_name, mode);
		}
		return;
	  }
	  i++;
	}	
	printf("File not found in current directory.\n");
  }
}

void close(const char* file_name) {
  if (FTIsOpen(file_name) != 0) {
	FTRemove(file_name);
  }
  else {
	printf("File not open.\n");
  }
}
