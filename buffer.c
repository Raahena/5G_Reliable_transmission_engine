#include <stdio.h>
#include "thread.h"
#include "buffer.h"
#include <stdint.h>

/*
 * Initialize Buffer
 */
void initBuffer(Buffer *buffer)
{
    buffer->front = 0;
    buffer->rear = -1;
    buffer->count = 0;
}

/*
 * Check Full
 */
int isBufferFull(Buffer *buffer)
{
    return (buffer->count == BUFFER_SIZE);
}

/*
 * Check Empty
 */
int isBufferEmpty(Buffer *buffer)
{
    return (buffer->count == 0);
}

/*
 * Insert Packet
 */
void enqueuePacket(Buffer *buffer, Packet packet)
{
    pthread_mutex_lock(&bufferMutex);

    if (isBufferFull(buffer))
    {
        printf("[BUFFER] Buffer Full\n");
        pthread_mutex_unlock(&bufferMutex);
        return;
    }

    buffer->rear = (buffer->rear + 1) % BUFFER_SIZE;
    buffer->packets[buffer->rear] = packet;
    buffer->count++;

    printf("[BUFFER] Packet %d Added\n", packet.seqNo);

    pthread_mutex_unlock(&bufferMutex);
}
/*
 * Remove Packet
 */
Packet dequeuePacket(Buffer *buffer)
{
    pthread_mutex_lock(&bufferMutex);

    Packet empty = createPacket(-1, "", LOW, CONTROL);

    if (isBufferEmpty(buffer))
    {
        pthread_mutex_unlock(&bufferMutex);
        return empty;
    }

    Packet packet = buffer->packets[buffer->front];

    buffer->front = (buffer->front + 1) % BUFFER_SIZE;
    buffer->count--;

    if (buffer->count == 0)
    {
        buffer->front = 0;
        buffer->rear = -1;
    }

    pthread_mutex_unlock(&bufferMutex);

    return packet;
}

/*
 * View Front Packet
 */
Packet peekPacket(Buffer *buffer)
{
    Packet empty = createPacket(-1, "", LOW, CONTROL);

    if (isBufferEmpty(buffer))
        return empty;

    return buffer->packets[buffer->front];
}

/*
 * Display Buffer Contents
 */
void displayBuffer(Buffer *buffer)
{
    printf("\n===== BUFFER STATUS =====\n");
    printf("Packet Count : %d\n", buffer->count);

    if (isBufferEmpty(buffer))
    {
        printf("Buffer Empty\n");
        printf("=========================\n");
        return;
    }

    int index = buffer->front;
    printf("Packets : ");

    for (int i = 0; i < buffer->count; i++)
    {
        printf("%d ", buffer->packets[index].seqNo);
        index = (index + 1) % BUFFER_SIZE;
    }

    printf("\n=========================\n");
}
