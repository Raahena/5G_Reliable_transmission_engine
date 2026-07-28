#include <stdio.h>
#include <stdint.h>
#include "sender.h"
#include "logging.h"

/*
 * Send a single packet
 */
void sendPacket(Packet *packet)
{
    if (packet == NULL)
        return;

    printf("[SENDER] Sending Packet %d\n", packet->seqNo);
    printf("Data : %s\n", packet->data);
    printf("Checksum : %x\n", packet->checksum);

    printf("Priority : ");
    logPacketEvent("SENT", packet->seqNo);
    switch (packet->priority)
    {
        case HIGH:   printf("HIGH\n");   break;
        case MEDIUM: printf("MEDIUM\n"); break;
        case LOW:    printf("LOW\n");    break;
        default:     printf("UNKNOWN\n");
    }
}

/*
 * Retransmit packet
 */
void retransmitPacket(Packet *packet)
{
    if (packet == NULL)
        return;

    printf("\n[SENDER] Retransmitting Packet %d\n", packet->seqNo);
   logPacketEvent("RETRANSMITTED", packet->seqNo);
}

/*
 * Send packets within window
 *
 * Start timer only for oldest unacknowledged packet
 */
void sendPacketsInWindow(Packet packets[], Window *window, int totalPackets, Timer *timer, int timeout)
{
    while (canSend(window) && window->nextSeqNum < totalPackets)
    {
        /*
         * Start timer for oldest unacknowledged packet
         */
        if (window->base == window->nextSeqNum)
        {
            startTimer(timer, timeout);
            printf("\n[TIMER] Started for Packet %d\n", window->base);
        }

        sendPacket(&packets[window->nextSeqNum]);
        window->nextSeqNum++;
    }
}

/*
 * Go Back N Retransmission
 *
 * Retransmit packets from BASE -> NEXTSEQNUM-1
 */
void goBackNRetransmit(Packet packets[], Window *window)
{
    printf("\n====================================\n");
    printf("             GO BACK-N\n");
    printf("====================================\n");
    printf("Retransmitting Packets from %d to %d\n\n", window->base, window->nextSeqNum - 1);

    for (int i = window->base; i < window->nextSeqNum; i++)
    {
        retransmitPacket(&packets[i]);
    }

    printf("\n====================================\n");
}

/*
 * Display Sender Window
 */
void displaySenderWindow(Packet packets[], Window *window)
{
    printf("\n===== CURRENT WINDOW =====\n");
    printf("Base : %d\n", window->base);
    printf("NextSeqNum : %d\n", window->nextSeqNum);
    printf("Window Size : %d\n", window->windowSize);

    printf("\nPackets In Window:\n");
    for (int i = window->base; i < window->nextSeqNum; i++)
    {
        printf("Packet %d [%s]\n",
               packets[i].seqNo,
               packets[i].acked ? "ACKED" : "WAITING");
    }

    printf("==========================\n");
}
