#ifndef RECEIVER_H
#define RECEIVER_H
#include <stdint.h>
#include "packet.h"

void receivePacket(Packet *packet);

int validatePacket(Packet *packet);

int generateACK(Packet *packet);

void storeFragment(Packet *packet);
const char* getReassembledMessage(void);
#endif