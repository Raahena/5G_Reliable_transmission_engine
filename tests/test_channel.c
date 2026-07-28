#include "test.h"
#include <stdlib.h>
#include <time.h>

/*----------------------------------------------------------
    Test Packet Loss (0%)
-----------------------------------------------------------*/
void testPacketLoss(void)
{
    srand((unsigned int)time(NULL));

    int lost = simulatePacketLoss(0);

    ASSERT_EQUAL(0,
                 lost,
                 "0% Packet Loss");
}

/*----------------------------------------------------------
    Test Packet Loss (100%)
-----------------------------------------------------------*/
void testPacketLossAlways(void)
{
    srand((unsigned int)time(NULL));

    int lost = simulatePacketLoss(100);

    ASSERT_EQUAL(1,
                 lost,
                 "100% Packet Loss");
}

/*----------------------------------------------------------
    Test Packet Delay (0%)
-----------------------------------------------------------*/
void testPacketDelay(void)
{
    srand((unsigned int)time(NULL));

    int delayed = simulatePacketDelay(0);

    ASSERT_EQUAL(0,
                 delayed,
                 "0% Packet Delay");
}

/*----------------------------------------------------------
    Test Packet Delay (100%)
-----------------------------------------------------------*/
void testPacketDelayAlways(void)
{
    srand((unsigned int)time(NULL));

    int delayed = simulatePacketDelay(100);

    ASSERT_EQUAL(1,
                 delayed,
                 "100% Packet Delay");
}

/*----------------------------------------------------------
    Test Packet Corruption (0%)
-----------------------------------------------------------*/
void testPacketCorruption(void)
{
    srand((unsigned int)time(NULL));

    int corrupted = simulatePacketCorruption(0);

    ASSERT_EQUAL(0,
                 corrupted,
                 "0% Packet Corruption");
}

/*----------------------------------------------------------
    Test Packet Corruption (100%)
-----------------------------------------------------------*/
void testPacketCorruptionAlways(void)
{
    srand((unsigned int)time(NULL));

    int corrupted = simulatePacketCorruption(100);

    ASSERT_EQUAL(1,
                 corrupted,
                 "100% Packet Corruption");
}

/*----------------------------------------------------------
    Test Random Simulation
-----------------------------------------------------------*/
void testRandomSimulation(void)
{
    srand((unsigned int)time(NULL));

    int loss = simulatePacketLoss(50);
    int delay = simulatePacketDelay(50);
    int corruption = simulatePacketCorruption(50);

    ASSERT_TRUE((loss == 0 || loss == 1),
                "Random Loss");

    ASSERT_TRUE((delay == 0 || delay == 1),
                "Random Delay");

    ASSERT_TRUE((corruption == 0 || corruption == 1),
                "Random Corruption");
}

/*----------------------------------------------------------
    Run All Channel Tests
-----------------------------------------------------------*/
void runChannelTests(void)
{
    testPacketLoss();
    testPacketLossAlways();

    testPacketDelay();
    testPacketDelayAlways();

    testPacketCorruption();
    testPacketCorruptionAlways();

    testRandomSimulation();
}