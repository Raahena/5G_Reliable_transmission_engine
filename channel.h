#ifndef CHANNEL_H
#define CHANNEL_H

#include "packet.h"
#include <stdint.h>

/*
 * Simulate Packet Loss
 */
int simulatePacketLoss(int lossRate);

/*
 * Simulate Packet Delay
 */
int simulatePacketDelay(int delayRate);

/*
 * Simulate Packet Corruption
 */
int simulatePacketCorruption(int corruptionRate);

#endif
