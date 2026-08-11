
#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define DATA_SIZE 128

typedef enum
{
    VOICE,
    VIDEO,
    FILE_TRANSFER,
    IOT,
    CONTROL
} MessageType;

typedef enum
{
    LOW = 0,
    MEDIUM,
    HIGH
} Priority;

typedef struct
{
    int         seqNo;
    int         fragmentNo;
    int         totalFragments;
    char        data[DATA_SIZE];
    uint16_t    checksum;
    Priority    priority;
    MessageType messageType;
    uint8_t     acked;
} Packet;

/*
 * Create Packet
 */
Packet createPacket(
    int seqNo,
    const char *data,
    Priority priority,
    MessageType type);

/*
 * Create Fragment Packet
 */
Packet createFragmentPacket(
    int seqNo,
    int fragmentNo,
    int totalFragments,
    const char *data,
    Priority priority,
    MessageType type);

/*
 * Calculate Checksum
 */
int calculateChecksum(const char *data);

/*
 * Display Packet
 */
void displayPacket(Packet packet);

#endif /* PACKET_H */

