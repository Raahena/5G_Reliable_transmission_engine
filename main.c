
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>

#include "packet.h"
#include "buffer.h"
#include "sender.h"
#include "config.h"
#include "adaptivewindow.h"
#include "receiver.h"
#include "ack.h"
#include "window.h"
#include "timer.h"
#include "channel.h"
#include "statistics.h"
#include "logging.h"
#include "throughput.h"

#define FRAGMENT_SIZE 1

int main(void)
{
    int messageChoice;
    MessageType type;
    Priority packetPriority;
    char fullMessage[512];

    printf("\n===== TRAFFIC TYPE =====\n");
    printf("1. Voice Call\n");
    printf("2. Video Streaming\n");
    printf("3. File Download\n");
    printf("4. IoT Messaging\n");
    printf("5. Control Signalling\n");
    printf("\nChoice : ");

    scanf("%d", &messageChoice);

    switch (messageChoice)
    {
        case 1:
            type = VOICE;
            packetPriority = HIGH;
            break;

        case 2:
            type = VIDEO;
            packetPriority = MEDIUM;
            break;

        case 3:
            type = FILE_TRANSFER;
            packetPriority = LOW;
            break;

        case 4:
            type = IOT;
            packetPriority = HIGH;
            break;

        default:
            type = CONTROL;
            packetPriority = HIGH;
            break;
    }

    while (getchar() != '\n')
    {
    }

    printf("\nEnter Message : ");

    fgets(
        fullMessage,
        sizeof(fullMessage),
        stdin);

    fullMessage[strcspn(fullMessage, "\n")] = '\0';

    Config config;

    srand(time(NULL));

    getNetworkProfile(&config);

    Packet packets[100];

    Window *window = malloc(sizeof(Window));
    Timer *timer = malloc(sizeof(Timer));
    Statistics *stats = malloc(sizeof(Statistics));
    Buffer *txBuffer = malloc(sizeof(Buffer));

    if (window == NULL ||
        timer == NULL ||
        stats == NULL ||
        txBuffer == NULL)
    {
        printf("Memory allocation failed!\n");

        free(window);
        free(timer);
        free(stats);
        free(txBuffer);

        return 1;
    }

    time_t startTime;
    time_t endTime;

    initWindow(
        window,
        config.windowSize);

    initStatistics(stats);

    initBuffer(txBuffer);

    initializeLog();

    startTime = time(NULL);

    /*
     * Create packets
     */
    int messageLength = strlen(fullMessage);

    int totalFragments =
        (messageLength + FRAGMENT_SIZE - 1) /
        FRAGMENT_SIZE;

    config.totalPackets = totalFragments;

    printf("\n===== SIMULATION CONFIG =====\n");

    printf(
        "Packets      : %d\n",
        config.totalPackets);

    printf(
        "Window Size  : %d\n",
        config.windowSize);

    printf(
        "Timeout      : %d sec\n",
        config.timeout);

    printf(
        "Loss Rate    : %d %%\n",
        config.lossRate);

    printf(
        "Delay Rate   : %d %%\n",
        config.delayRate);

    printf(
        "Corruption   : %d %%\n",
        config.corruptionRate);

    printf("==============================\n");

    for (int i = 0;
         i < totalFragments;
         i++)
    {
        char fragment[FRAGMENT_SIZE + 1];

        strncpy(
            fragment,
            &fullMessage[i * FRAGMENT_SIZE],
            FRAGMENT_SIZE);

        fragment[FRAGMENT_SIZE] = '\0';

        Packet p =
            createFragmentPacket(
                i,
                i,
                totalFragments,
                fragment,
                packetPriority,
                type);

        packets[i] = p;

        enqueuePacket(
            txBuffer,
            p);
    }

    displayBuffer(txBuffer);

    printf(
        "\n===== GO-BACK-N TRANSMISSION START =====\n");

    while (window->base < config.totalPackets)
    {
        displayBuffer(txBuffer);

        displayWindow(
            window,
            config.totalPackets);

        sendPacketsInWindow(
            packets,
            window,
            config.totalPackets,
            timer,
            config.timeout);

        for (int i = window->base;
             i < window->nextSeqNum;
             i++)
        {
            if (packets[i].acked)
            {
                continue;
            }

            stats->sent++;

            /*
             * LOSS
             */
            if (simulatePacketLoss(config.lossRate))
            {
                printf(
                    "\n[CHANNEL] Packet %d Lost\n",
                    packets[i].seqNo);

                logPacketEvent(
                    "LOST",
                    packets[i].seqNo);

                stats->lost++;

                adjustWindow(
                    window,
                    1);

                continue;
            }

            /*
             * WORK ON TEMP COPY
             */
            Packet tempPacket = packets[i];

            /*
             * DELAY
             */
            if (simulatePacketDelay(config.delayRate))
            {
                printf(
                    "[CHANNEL] Packet %d Delayed\n",
                    tempPacket.seqNo);

                logPacketEvent(
                    "DELAYED",
                    tempPacket.seqNo);

                sleep(1);
            }

            /*
             * CORRUPTION
             */
            if (simulatePacketCorruption(
                    config.corruptionRate))
            {
                printf(
                    "[CHANNEL] Packet %d Corrupted\n",
                    tempPacket.seqNo);

                logPacketEvent(
                    "CORRUPTED",
                    tempPacket.seqNo);

                tempPacket.checksum += 100;
            }

            receivePacket(&tempPacket);

            if (validatePacket(&tempPacket))
            {
                int ackNo =
                    generateACK(&tempPacket);

                int oldBase = window->base;

                processACK(
                    ackNo,
                    &window->base);

                if (window->base > oldBase)
                {
                    adjustWindow(
                        window,
                        0);

                    if (!packets[i].acked)
                    {
                        packets[i].acked = 1;

                        stats->received++;

                        if (!isBufferEmpty(txBuffer))
                        {
                            dequeuePacket(txBuffer);
                        }
                    }
                }
            }
            else
            {
                printf(
                    "[RECEIVER] Corrupted Packet Rejected\n");
            }
        }

        /*
         * TIMEOUT HANDLING
         */
        if (hasOutstandingPackets(window))
        {
            sleep(config.timeout + 1);

            if (hasTimedOut(timer))
            {
                printf(
                    "\n[TIMER] Timeout Occurred\n");

                logEvent(
                    "TIMEOUT OCCURRED");

                goBackNRetransmit(
                    packets,
                    window);

                stats->retransmitted +=
                    (window->nextSeqNum -
                     window->base);

                /*
                 * Process retransmitted packets
                 */
                for (int i = window->base;
                     i < window->nextSeqNum;
                     i++)
                {
                    receivePacket(
                        &packets[i]);

                    if (validatePacket(
                            &packets[i]))
                    {
                        int ackNo =
                            generateACK(
                                &packets[i]);

                        int oldBase =
                            window->base;

                        processACK(
                            ackNo,
                            &window->base);

                        if (window->base > oldBase)
                        {
                            adjustWindow(
                                window,
                                0);

                            if (!packets[i].acked)
                            {
                                packets[i].acked = 1;

                                stats->received++;

                                if (!isBufferEmpty(
                                        txBuffer))
                                {
                                    dequeuePacket(
                                        txBuffer);
                                }
                            }
                        }
                    }
                }

                resetTimer(timer);
            }
        }
    }

    endTime = time(NULL);

    double elapsedTime =
        difftime(
            endTime,
            startTime);

    printStatistics(stats);

    displayAdaptiveWindow(window);

    displayMetrics(
        stats->sent,
        stats->received,
        stats->lost,
        stats->retransmitted,
        elapsedTime);

    printf("\nFINAL BUFFER STATUS\n");

    displayBuffer(txBuffer);

    free(window);
    free(timer);
    free(stats);
    free(txBuffer);

    closeLog();

    return 0;
}
