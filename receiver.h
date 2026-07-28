#ifndef RECEIVER_H
#define RECEIVER_H
#include <stdint.h>
#include "packet.h"

void receivePacket(Packet *packet);

int validatePacket(Packet *packet);

int generateACK(Packet *packet);

#endif