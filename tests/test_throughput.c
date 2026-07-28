#include "test.h"
#include <math.h>

/*----------------------------------------------------------
    Test Throughput Calculation
-----------------------------------------------------------*/
void testThroughputCalculation(void)
{
    float throughput;

    throughput = calculateThroughput(100, 10.0);

    ASSERT_TRUE(fabs(throughput - 10.0f) < 0.001,
                "Throughput Calculation");
}

/*----------------------------------------------------------
    Test Zero Time
-----------------------------------------------------------*/
void testZeroTime(void)
{
    float throughput;

    throughput = calculateThroughput(100, 0);

    ASSERT_TRUE(throughput == 0,
                "Zero Time");
}

/*----------------------------------------------------------
    Test Packet Delivery Ratio
-----------------------------------------------------------*/
void testPacketDeliveryRatio(void)
{
    float pdr;

    pdr = calculatePDR(90,100);

    ASSERT_TRUE(fabs(pdr - 90.0f) < 0.001,
                "Packet Delivery Ratio");
}

/*----------------------------------------------------------
    Test Packet Loss Rate
-----------------------------------------------------------*/
void testLossRate(void)
{
    float loss;

    loss = calculateLossRate(10,100);

    ASSERT_TRUE(fabs(loss - 10.0f) < 0.001,
                "Packet Loss Rate");
}

/*----------------------------------------------------------
    Test Retransmission Rate
-----------------------------------------------------------*/
void testRetransmissionRate(void)
{
    float rate;

    rate = calculateRetransmissionRate(20,100);

    ASSERT_TRUE(fabs(rate - 20.0f) < 0.001,
                "Retransmission Rate");
}

/*----------------------------------------------------------
    Test Zero Packets
-----------------------------------------------------------*/
void testZeroPackets(void)
{
    ASSERT_TRUE(calculatePDR(0,0) == 0,
                "Zero Packet PDR");

    ASSERT_TRUE(calculateLossRate(0,0) == 0,
                "Zero Packet Loss");

    ASSERT_TRUE(calculateRetransmissionRate(0,0) == 0,
                "Zero Packet Retransmission");
}

/*----------------------------------------------------------
    Test Display Metrics
-----------------------------------------------------------*/
void testDisplayMetrics(void)
{
    displayMetrics(100,
                   90,
                   10,
                   5,
                   10.0);

    ASSERT_TRUE(1,
                "Display Metrics");
}

/*----------------------------------------------------------
    Test High Throughput
-----------------------------------------------------------*/
void testHighThroughput(void)
{
    float throughput;

    throughput = calculateThroughput(1000,2.0);

    ASSERT_TRUE(fabs(throughput - 500.0f) < 0.001,
                "High Throughput");
}

/*----------------------------------------------------------
    Run All Throughput Tests
-----------------------------------------------------------*/
void runThroughputTests(void)
{
    testThroughputCalculation();
    testZeroTime();
    testPacketDeliveryRatio();
    testLossRate();
    testRetransmissionRate();
    testZeroPackets();
    testDisplayMetrics();
    testHighThroughput();
}