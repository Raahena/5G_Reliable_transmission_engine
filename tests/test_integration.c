#include "test.h"
#include <math.h>

/*----------------------------------------------------------
    Integration Test 1
    Sender -> Receiver -> ACK
-----------------------------------------------------------*/
void testSenderReceiverACK(void)
{
    Packet packet;
    Window senderWindow;
    Buffer buffer;
    Statistics stats;

    initWindow(&senderWindow, 4);
    initBuffer(&buffer);
    initStatistics(&stats);

    packet = createPacket(
                0,
                "Hello",
                HIGH,
                CONTROL);

    enqueuePacket(&buffer, packet);

    Packet txPacket = dequeuePacket(&buffer);

    sendPacket(&txPacket);

    receivePacket(&txPacket);

    ASSERT_TRUE(1,
                "Receiver accepted packet");

    int ack = generateACK(&txPacket);

    processACK(ack, &senderWindow.base);

    ASSERT_EQUAL(1,
                 senderWindow.base,
                 "Window slid after ACK");
}

/*----------------------------------------------------------
    Integration Test 2
    Window Sliding
-----------------------------------------------------------*/
void testWindowSliding(void)
{
    Window window;

    initWindow(&window, 4);

    processACK(0, &window.base);

    ASSERT_EQUAL(1,
                 window.base,
                 "Window Slide 1");

    processACK(1, &window.base);

    ASSERT_EQUAL(2,
                 window.base,
                 "Window Slide 2");
}

/*----------------------------------------------------------
    Integration Test 3
    Loss and Retransmission
-----------------------------------------------------------*/
void testLossRetransmission(void)
{
    Packet packet;

    packet = createPacket(
                1,
                "DATA",
                MEDIUM,
                FILE_TRANSFER);

    if (simulatePacketLoss(100))
    {
        retransmitPacket(&packet);

        ASSERT_TRUE(1,
                    "Retransmission after loss");
    }
}

/*----------------------------------------------------------
    Integration Test 4
    Adaptive Window
-----------------------------------------------------------*/
void testAdaptiveWindowControl(void)
{
    Window window;

    initWindow(&window, 5);

    adjustWindow(&window, 1);

    ASSERT_EQUAL(4,
                 window.windowSize,
                 "Decrease after loss");

    adjustWindow(&window, 0);

    ASSERT_EQUAL(5,
                 window.windowSize,
                 "Increase after success");
}

/*----------------------------------------------------------
    Integration Test 5
    Statistics
-----------------------------------------------------------*/
void testStatisticsMetrics(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent = 100;
    stats.received = 90;
    stats.lost = 10;
    stats.retransmitted = 5;

    float pdr = calculatePDR(
                    stats.received,
                    stats.sent);

    float loss = calculateLossRate(
                     stats.lost,
                     stats.sent);

    ASSERT_TRUE(fabs(pdr - 90.0f) < 0.001f,
                "PDR");

    ASSERT_TRUE(fabs(loss - 10.0f) < 0.001f,
                "Loss Rate");
}

/*----------------------------------------------------------
    Integration Test 6
    Complete Transmission
-----------------------------------------------------------*/
void testCompleteTransmission(void)
{
    Packet packet;
    Window window;
    Buffer buffer;
    Statistics stats;

    initWindow(&window, 4);
    initBuffer(&buffer);
    initStatistics(&stats);

    packet = createPacket(
                0,
                "Integration Test",
                HIGH,
                FILE_TRANSFER);

    enqueuePacket(&buffer, packet);

    Packet tx = dequeuePacket(&buffer);

    sendPacket(&tx);

    stats.sent++;

    receivePacket(&tx);

    stats.received++;

    int ack = generateACK(&tx);

    processACK(ack, &window.base);

    ASSERT_EQUAL(1,
                 stats.sent,
                 "Packet Sent");

    ASSERT_EQUAL(1,
                 stats.received,
                 "Packet Received");

    ASSERT_EQUAL(1,
                 window.base,
                 "Window Updated");
}

/*----------------------------------------------------------
    Run All Integration Tests
-----------------------------------------------------------*/
void runAllIntegrationTests(void)
{
    printf("\n========================================");
    printf("\nINTEGRATION TESTS");
    printf("\n========================================\n");

    testSenderReceiverACK();
    testWindowSliding();
    testLossRetransmission();
    testAdaptiveWindowControl();
    testStatisticsMetrics();
    testCompleteTransmission();
}