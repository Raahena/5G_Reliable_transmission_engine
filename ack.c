#include <stdio.h>
#include "ack.h"
#include "logging.h"
/*
 * Go-Back-N uses
 * cumulative ACKs
 *
 * ACK 3 means:
 * Packets 0,1,2,3
 * successfully received.
 */

void processACK(int ackNo, uint16_t *base)
{
    printf("[SENDER] ACK %d Received\n", ackNo);
    logPacketEvent("ACK", ackNo);

    if (ackNo >= *base)
    {
        *base = ackNo + 1;

        printf("[GBN] Window Slid\n");
        printf("[GBN] New Base = %d\n", *base);
    }
}

/*
 * Mark packet ACKed
 */
void markPacketAcknowledged(int ackArray[], int ackNo)
{
    ackArray[ackNo] = 1;
}

/*
 * Check ACK status
 */
int isAcknowledged(const int ackArray[], int seqNo)
{
    return ackArray[seqNo];
}

/*
 * Display ACK Table
 */
void displayACKStatus(int ackArray[], int totalPackets)
{
    printf("\n===== ACK STATUS =====\n");

    for (int i = 0; i < totalPackets; i++)
    {
        printf("Packet %d : %s\n",
               i,
               ackArray[i] ? "ACKED" : "PENDING");
    }

    printf("======================\n");
}
