#ifndef ACK_H
#define ACK_H
#include <stdint.h>
/*
 * Process ACK at sender
 */
void processACK(
int ackNo,
 uint16_t *base);

/*
 * Mark packet acknowledged
 */
void markPacketAcknowledged(
int ackArray[],
 int ackNo);

/*
 * Check ACK status
 */
int isAcknowledged(
 int ackArray[],
int seqNo);

/*
 * Display ACK Table
 */
void displayACKStatus(
    int ackArray[],
int totalPackets);

#endif