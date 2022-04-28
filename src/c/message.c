#include "header/message.h"

void getMessage(struct message *m, int segment){
  interrupt(0x21, 0x2, m, 0x104 + div(segment, 0x1000) - 2, 0);
}

void sendMessage(struct message *m, int segment){
  interrupt(0x21, 0x3, m, 0x104 + div(segment, 0x1000) - 2, 0);
}