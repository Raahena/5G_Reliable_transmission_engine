#include <stdlib.h>
#include <stdint.h>

/*
 * Simulate Packet Loss
 */
int simulatePacketLoss(int lossRate)
{
    return (rand() % 100) < lossRate;
}

/*
 * Simulate Packet Delay
 */
int simulatePacketDelay(int delayRate)
{
    return (rand() % 100) < delayRate;
}

/*
 * Simulate Packet Corruption
 */
int simulatePacketCorruption(int corruptionRate)
{
    return (rand() % 100) < corruptionRate;
}
