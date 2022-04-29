#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* uniq_spc(char* str){
    char *from, *to;
    int spc=0;
    to=from=str;
    while(1){
        if(spc && *from == ' ' && to[-1] == ' ')
            ++from;
        else {
            spc = (*from==' ')? 1 : 0;
            *to++ = *from++;
            if(!to[-1])break;
        }
    }
    return str;
}

int strparsing(char *input, char** output)
{
    char command[8][256];
    int i=0;
    int j=0;
    int z=0;
    int commandcount=0;
    
    for(i=0;i<8;i++){
        for(j=0;j<64;j++){
            command[i][j] = '\0';
        }
    }

    i=0;
    j=0;
    
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
    for(i=0;i<commandcount;i++){
        strcpy(command[i],uniq_spc(command[i]));
        if (command[i][0] == ' ') memmove(command[i], command[i]+1, strlen(command[i]));
        if (command[i][strlen(command[i])-1] == ' ') command[i][strlen(command[i])-1] = '\0';
        output[i] = command[i];
    }
    return commandcount;
}
