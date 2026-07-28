#ifndef THROUGHPUT_H
#define THROUGHPUT_H

#include <stdint.h>

/*
 * Calculate throughput
 */
float calculateThroughput(int packetsReceived, double timeTaken);

/*
 * Packet Delivery Ratio
 */
float calculatePDR(int packetsReceived, int packetsSent);

/*
 * Loss Percentage
 */
float calculateLossRate(int packetsLost, int packetsSent);

/*
 * Retransmission Rate
 */
float calculateRetransmissionRate(int retransmitted, int packetsSent);

/*
 * Print Performance Metrics
 */
void displayMetrics(int sent,
                    int received,
                    int lost,
                    int retransmitted,
                    double timeTaken);

#endif
