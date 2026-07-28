#include <stdio.h>
#include <stdint.h>
#include "throughput.h"

/*
 * Throughput
 */
float calculateThroughput(int packetsReceived, double timeTaken)
{
    if (timeTaken <= 0)
        return 0;

    return (float)packetsReceived / (float)timeTaken;
}

/*
 * Packet Delivery Ratio
 */
float calculatePDR(int packetsReceived, int packetsSent)
{
    if (packetsSent == 0)
        return 0;

    return ((float)packetsReceived / packetsSent) * 100;
}

/*
 * Loss Percentage
 */
float calculateLossRate(int packetsLost, int packetsSent)
{
    if (packetsSent == 0)
        return 0;

    return ((float)packetsLost / packetsSent) * 100;
}

/*
 * Retransmission Rate
 */
float calculateRetransmissionRate(int retransmitted, int packetsSent)
{
    if (packetsSent == 0)
        return 0;

    return ((float)retransmitted / packetsSent) * 100;
}

/*
 * Display Metrics
 */
void displayMetrics(int sent, int received, int lost, int retransmitted, double timeTaken)
{
    float throughput          = calculateThroughput(received, timeTaken);
    float pdr                 = calculatePDR(received, sent);
    float lossRate            = calculateLossRate(lost, sent);
    float retransmissionRate  = calculateRetransmissionRate(retransmitted, sent);

    printf("\n=====================================\n");
    printf("       PERFORMANCE METRICS\n");
    printf("=====================================\n");

    printf("Throughput           : %.2f pkt/sec\n", throughput);
    printf("Packet Delivery Ratio: %.2f %%\n", pdr);
    printf("Loss Rate            : %.2f %%\n", lossRate);
    printf("Retransmission Rate  : %.2f %%\n", retransmissionRate);
    printf("Execution Time       : %.2f sec\n", timeTaken);

    printf("=====================================\n");
}
