#ifndef BUFFER_H
#define BUFFER_H

#include "packet.h"
#include <stdint.h>

#define BUFFER_SIZE 50

typedef struct
{
    Packet packets[BUFFER_SIZE];
    int front;
    int rear;
    int count;
} Buffer;

/*
 * Initialize Buffer
 */
void initBuffer(Buffer *buffer);

/*
 * Check Buffer Status
 */
int isBufferFull(Buffer *buffer);
int isBufferEmpty(Buffer *buffer);

/*
 * Insert Packet
 */
void enqueuePacket(Buffer *buffer, Packet packet);

/*
 * Remove Packet
 */
Packet dequeuePacket(Buffer *buffer);

/*
 * Peek Front Packet
 */
Packet peekPacket(Buffer *buffer);

/*
 * Display Buffer
 */
void displayBuffer(Buffer *buffer);

#endif
