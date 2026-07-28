#include "test.h"

/*----------------------------------------------------------
    Test Statistics Initialization
-----------------------------------------------------------*/
void testStatisticsInit(void)
{
    Statistics stats;

    initStatistics(&stats);

    ASSERT_EQUAL(0,
                 stats.sent,
                 "Sent Initialized");

    ASSERT_EQUAL(0,
                 stats.received,
                 "Received Initialized");

    ASSERT_EQUAL(0,
                 stats.lost,
                 "Lost Initialized");

    ASSERT_EQUAL(0,
                 stats.retransmitted,
                 "Retransmitted Initialized");
}

/*----------------------------------------------------------
    Test Sent Counter
-----------------------------------------------------------*/
void testSentCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent++;

    ASSERT_EQUAL(1,
                 stats.sent,
                 "Sent Counter");
}

/*----------------------------------------------------------
    Test Received Counter
-----------------------------------------------------------*/
void testReceivedCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.received += 5;

    ASSERT_EQUAL(5,
                 stats.received,
                 "Received Counter");
}

/*----------------------------------------------------------
    Test Lost Counter
-----------------------------------------------------------*/
void testLostCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.lost = 3;

    ASSERT_EQUAL(3,
                 stats.lost,
                 "Lost Counter");
}

/*----------------------------------------------------------
    Test Retransmitted Counter
-----------------------------------------------------------*/
void testRetransmittedCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.retransmitted = 2;

    ASSERT_EQUAL(2,
                 stats.retransmitted,
                 "Retransmitted Counter");
}

/*----------------------------------------------------------
    Test Multiple Statistics Update
-----------------------------------------------------------*/
void testStatisticsIncrement(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent += 10;
    stats.received += 8;
    stats.lost += 2;
    stats.retransmitted += 1;

    ASSERT_EQUAL(10,
                 stats.sent,
                 "Sent Updated");

    ASSERT_EQUAL(8,
                 stats.received,
                 "Received Updated");

    ASSERT_EQUAL(2,
                 stats.lost,
                 "Lost Updated");

    ASSERT_EQUAL(1,
                 stats.retransmitted,
                 "Retransmitted Updated");
}

/*----------------------------------------------------------
    Test Statistics Display
-----------------------------------------------------------*/
void testPrintStatistics(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent = 15;
    stats.received = 13;
    stats.lost = 2;
    stats.retransmitted = 1;

    printStatistics(&stats);

    ASSERT_TRUE(1,
                "Statistics Display");
}

/*----------------------------------------------------------
    Run All Statistics Tests
-----------------------------------------------------------*/
void runStatisticsTests(void)
{
    testStatisticsInit();
    testSentCounter();
    testReceivedCounter();
    testLostCounter();
    testRetransmittedCounter();
    testStatisticsIncrement();
    testPrintStatistics();
}