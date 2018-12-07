#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

const char PREVDIR[11] = "..         ";
const char CURRDIR[11] = ".          ";
int i;

void ToFAT32(char * input) {
   char FAT32string [13] = " ", temp [13] = " ";
   short inputSize = strlen(input);
   short count_spaces = 11 - inputSize;

   if(!strcmp(input, "..")) {
       strcpy(input, PREVDIR);
       return;
   }

   if(!strcmp(input, ".")) {
       strcpy(input, CURRDIR);
       return;
   }

   i = 0;
   while(i < inputSize) {
       if(input[i] >= 'a' && input[i] <= 'z')
           FAT32string[i] = input[i] - 32;
       else {
           if (input[i] == '.' && inputSize < 12) {
               count_spaces = 11-(i+3);
           }
		   FAT32string[i] = input[i];
       }
	   i++;
   }
   for(i = inputSize; i < 12; i++) {
       FAT32string[i] = ' ';
   }
   strcpy(temp, FAT32string);

   for(i = 0; i < 11; i++) {
		int j;
       if(FAT32string[i] == '.') {
           FAT32string[8] = temp[i+1];
           FAT32string[9] = temp[i+2];
           FAT32string[10] = temp[i+3];
           for(j = 0; j < count_spaces; j++)
               FAT32string[i+j] = ' ';
           break;
       }
   } 
   FAT32string[11] = '\0';
   strcpy(input, FAT32string); 
}

void ToStandardFormat(char * input)
{
   if(!strcmp(input, PREVDIR)) {
      strcpy(input, "..");
      return;
   }

   if(!strcmp(input, CURRDIR)) {
      strcpy(input, ".");
      return;
   }

   char exten[4];
   char temp[12] = " ";

   exten[0] = input[8];
   exten[1] = input[9];
   exten[2] = input[10];
   exten[3] = '\0';

   for(int i = 0; i < 4; ++i) {
       if(exten[i] == ' ')
           exten[i] = '\0';
   }
   
   for(int i = 0; i < 11; i++) {
       if(input[i] == ' ') {
          if(exten[0] >= 'A' && exten[0] <= 'Z')
             temp[i] = '.';
          strcat(temp, exten);
          temp[i+strlen(exten)+1] = '\0';
          strcpy(input, temp);
          return;
       }
       temp[i] = input[i]; 
   }
   temp[11] = '\0';
   strcpy(input,temp);
}

char* DynStrPushBack(char* dest, char c) {
	size_t total_len = strlen(dest) + 2;
	char* new_str = (char*)calloc(total_len, sizeof(char));
	strcpy(new_str, dest);
	new_str[total_len-2] = c;
	new_str[total_len-1] = '\0';
	free(dest);
	return new_str;
}
