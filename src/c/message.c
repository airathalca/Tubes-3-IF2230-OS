#include "header/message.h"


void getMessage(struct message *m, int segment){
  readSector(m, 0x104 + div(segment, 0x1000) - 2);
}

void sendMessage(struct message *m, int segment){
  writeSector(m, 0x104 + div(segment, 0x1000) - 2);
}