#include <stdio.h>
#include <stdint.h>
#include "receiver.h"
#include "logging.h"

/*
 * Expected packet number
 * for Go-Back-N receiver
 */
static int expectedSeqNo = 0;

/*
 * Receive Packet
 */
void receivePacket(Packet *packet)
{
    if (packet == NULL)
        return;

    printf("\n[RECEIVER] Packet %d Received\n", packet->seqNo);
    logPacketEvent("RECEIVED", packet->seqNo);
    printf("Data : %s\n", packet->data);
    printf("Checksum : %d\n", packet->checksum);
}

/*
 * Checksum Validation
 */
int validatePacket(Packet *packet)
{
    if (packet == NULL)
        return 0;

    int checksum = calculateChecksum(packet->data);

    if (checksum == packet->checksum)
    {
        printf("[RECEIVER] Checksum verified\n");
        return 1;
    }

    printf("[RECEIVER] Packet Corrupted\n");
    return 0;
}

/*
 * Verify Sequence
 */
int validateSequence(int seqNo)
{
    return (seqNo == expectedSeqNo);
}

/*
 * Generate ACK
 *
 * Go-Back-N:
 * Accept only expected packet.
 * Send duplicate ACK otherwise.
 */
int generateACK(Packet *packet)
{
    if (packet == NULL)
        return -1;

    if (validateSequence(packet->seqNo))
    {
        printf("[RECEIVER] Packet %d Accepted\n", packet->seqNo);
        expectedSeqNo++;
        printf("[RECEIVER] ACK %d Sent\n", packet->seqNo);
        return packet->seqNo;
    }

    printf("[RECEIVER] Out Of Order Packet %d\n", packet->seqNo);
    printf("[RECEIVER] Duplicate ACK %d Sent\n", expectedSeqNo - 1);

    return expectedSeqNo - 1;
}

/*
 * Show Receiver Status
 */

