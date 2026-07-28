#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "packet.h"

/*
 * Calculate Checksum
 */
int calculateChecksum(const char *data)
{
    int sum = 0;

    while (*data)
    {
        sum += (unsigned char)(*data);
        data++;
    }

    return sum;
}

/*
 * Create Packet
 */
Packet createPacket(int seqNo, const char *data, Priority priority, MessageType type)
{
    Packet p;

    p.seqNo = seqNo;

    strncpy(p.data, data, DATA_SIZE - 1);
    p.data[DATA_SIZE - 1] = '\0';

    p.priority    = priority;
    p.messageType = type;
    p.checksum    = calculateChecksum(p.data);
    p.acked       = 0;

    return p;
}

/*
 * Display Packet
 */
void displayPacket(Packet packet)
{
    printf("\n========================\n");
    printf("Sequence Number : %d\n", packet.seqNo);
    printf("Data            : %s\n", packet.data);
    printf("Checksum        : %d\n", packet.checksum);

    printf("Priority        : ");
    switch (packet.priority)
    {
        case HIGH:   printf("HIGH\n");   break;
        case MEDIUM: printf("MEDIUM\n"); break;
        case LOW:    printf("LOW\n");    break;
        default:     printf("UNKNOWN\n");
    }

    printf("Message Type    : ");
    switch (packet.messageType)
    {
        case VOICE:        printf("VOICE\n");         break;
        case VIDEO:        printf("VIDEO\n");         break;
        case FILE_TRANSFER:printf("FILE TRANSFER\n"); break;
        case IOT:          printf("IOT\n");           break;
        case CONTROL:      printf("CONTROL\n");       break;
    }

    printf("ACK Status      : %s\n", packet.acked ? "ACKED" : "PENDING");
    printf("========================\n");
}
