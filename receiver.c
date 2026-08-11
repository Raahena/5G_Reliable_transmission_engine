
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "receiver.h"
#include "logging.h"

/*
 * Expected packet number
 * for Go-Back-N receiver
 */
#define REASSEMBLY_BUFFER_SIZE 4096
#define MAX_FRAGMENTS          100

static char fragmentStore[MAX_FRAGMENTS][DATA_SIZE];
static int fragmentReceived[MAX_FRAGMENTS] = {0};

static char reassembledMessage[
    REASSEMBLY_BUFFER_SIZE] = "";

static int expectedSeqNo = 0;

/*
 * Receive Packet
 */
void receivePacket(Packet *packet)
{
    if (packet == NULL)
    {
        return;
    }

    printf(
        "\n[RECEIVER] Packet %d Received\n",
        packet->seqNo);

    logPacketEvent(
        "RECEIVED",
        packet->seqNo);

    printf(
        "Data : %s\n",
        packet->data);

    printf(
        "Checksum : %d\n",
        packet->checksum);
}

/*
 * Store Fragment
 */
void storeFragment(Packet *packet)
{
    if (packet == NULL)
    {
        return;
    }

    if (packet->fragmentNo < 0 ||
        packet->fragmentNo >= MAX_FRAGMENTS)
    {
        return;
    }

    if (fragmentReceived[packet->fragmentNo] == 0)
    {
        strcpy(
            fragmentStore[packet->fragmentNo],
            packet->data);

        fragmentReceived[packet->fragmentNo] = 1;

        printf(
            "[RECEIVER] Fragment %d/%d Stored\n",
            packet->fragmentNo + 1,
            packet->totalFragments);

        int allReceived = 1;

        for (int i = 0;
             i < packet->totalFragments;
             i++)
        {
            if (fragmentReceived[i] == 0)
            {
                allReceived = 0;
                break;
            }
        }

        if (allReceived)
        {
            reassembledMessage[0] = '\0';

            for (int i = 0;
                 i < packet->totalFragments;
                 i++)
            {
                strcat(
                    reassembledMessage,
                    fragmentStore[i]);
            }

            for (int i = 0;
                 i < MAX_FRAGMENTS;
                 i++)
            {
                fragmentReceived[i] = 0;
                fragmentStore[i][0] = '\0';
            }
        }
    }
}

/*
 * Checksum Validation
 */
int validatePacket(Packet *packet)
{
    if (packet == NULL)
    {
        return 0;
    }

    int checksum =
        calculateChecksum(packet->data);

    if (checksum == packet->checksum)
    {
        printf(
            "[RECEIVER] Checksum verified\n");

        return 1;
    }

    printf(
        "[RECEIVER] Packet Corrupted\n");

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
    {
        return -1;
    }

    if (validateSequence(packet->seqNo))
    {
        printf(
            "[RECEIVER] Packet %d Accepted\n",
            packet->seqNo);

        expectedSeqNo++;

        printf(
            "[RECEIVER] ACK %d Sent\n",
            packet->seqNo);

        return packet->seqNo;
    }

    printf(
        "[RECEIVER] Out Of Order Packet %d\n",
        packet->seqNo);

    printf(
        "[RECEIVER] Duplicate ACK %d Sent\n",
        expectedSeqNo - 1);

    return expectedSeqNo - 1;
}

/*
 * Get Reassembled Message
 */
const char *getReassembledMessage(void)
{
    return reassembledMessage;
}

