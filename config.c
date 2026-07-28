#include <stdio.h>
#include <stdint.h>
#include "config.h"

void getNetworkProfile(Config *config)
{
    int choice;

    printf("\n===== NETWORK PROFILE =====\n");
    printf("1. Excellent Network\n");
    printf("2. Average Network\n");
    printf("3. Congested Network\n");
    printf("4. Custom\n");

    printf("Choice : ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            config->totalPackets   = 10;
            config->windowSize     = 8;
            config->timeout        = 2;
            config->lossRate       = 2;
            config->delayRate      = 5;
            config->corruptionRate = 1;
            break;

        case 2:
            config->totalPackets   = 10;
            config->windowSize     = 4;
            config->timeout        = 3;
            config->lossRate       = 10;
            config->delayRate      = 15;
            config->corruptionRate = 5;
            break;

        case 3:
            config->totalPackets   = 10;
            config->windowSize     = 2;
            config->timeout        = 5;
            config->lossRate       = 30;
            config->delayRate      = 40;
            config->corruptionRate = 15;
            break;

        case 4:
            printf("Packets : ");
            scanf("%d", &config->totalPackets);

            printf("Window Size : ");
            scanf("%d", &config->windowSize);

            printf("Timeout : ");
            scanf("%d", &config->timeout);

            printf("Packet Loss %% : ");
            scanf("%d", &config->lossRate);

            printf("Delay %% : ");
            scanf("%d", &config->delayRate);

            printf("Corruption %% : ");
            scanf("%d", &config->corruptionRate);
            break;
    }
}
