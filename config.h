#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct
{
    int totalPackets;
    int windowSize;
    int timeout;

    int lossRate;
    int delayRate;
    int corruptionRate;
} Config;

/*
 * Get Network Profile
 */
void getNetworkProfile(Config *config);

#endif
