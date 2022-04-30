#include "header/string.h"

unsigned int strlen(char *string) {
  int res = 0;
  while (string[res] != 0x0) {
    res++;
  }
  return res;
}

bool strcmp(char *s1, char *s2) {
  int i = 0;
  
  if (strlen(s1) != strlen(s2)) {
    return false;
  }
  for (i = 0; i < strlen(s1); i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

void strcpy(char *dst, char *src) {
  clear(dst, strlen(dst));
  while ((*dst++ = *src++) != '\0');
}

int strparsing(char *input, char output[8][256])
{
    char command[8][256];
    char newcom[256];
    int i=0;
    int j=0;
    int z=0;
    int commandcount=0;
    
    for(i = 0;i < 8;i++){
        for(j = 0;j < 256;j++){
            command[i][j] = '\0';
        }
    }

    i=0;
    j=0;
    
    if (input[0] == '\0') {
      return 0;
    }

    while(1){
        if(input[i]=='\0'){
            commandcount++;
            break;
        } 
        else if(input[i]==';'){
            commandcount++;
            z=0;
            j++;
        }
        else{
            command[j][z] = input[i];
            z++;
        }
        i++;  
    }
    for(i = 0; i < commandcount; i++){
        j = 0;
        z = 0;
        clear(newcom,256);
        while (command[i][j] != '\0') {
          if (!(command[i][j] == ' ' && command[i][j+1] == ' ')) {
            newcom[z] = command[i][j];
            z++;
          }
          j++;
        }
        newcom[z] = '\0';
        if (newcom[0] == ' ') {
          j = 0;
          while (newcom[j+1] != '\0') {
            newcom[j] = newcom[j+1];
            j++;
          }
          newcom[j] = '\0';
        }
        while (newcom[strlen(newcom)-1] == ' ') {
            newcom[strlen(newcom)-1] = '\0';
        }
        strcpy(output[i], newcom);
    }
    return commandcount;
}