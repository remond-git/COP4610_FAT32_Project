#ifndef _FILETABLE_H
#define _FILETABLE_H

struct FileTable {
  char name[32];
  char mode[8];
  struct FileTable* next;
};

void FTAdd(const char* fileName, const char* mode);
void FTRemove(const char* fileName);
int FTIsOpen(const char* fileName);
int FTIsOpenInRead(const char* fileName);
int FTIsOpenInWrite(const char* fileName);
void FTPrint();
void FTPrintEntry(struct FileTable* ent);
void FTCleanup();

#endif
