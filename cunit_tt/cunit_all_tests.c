/******************************************************************************
 * 5G Reliable Packet Transmission Engine
 * CUnit Test Suite
 *
 * Part 1
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../packet.h"
#include "../buffer.h"
#include "../window.h"
#include "../sender.h"
#include "../receiver.h"
#include "../ack.h"
#include "../timer.h"
#include "../statistics.h"
#include "../throughput.h"
#include "../channel.h"
#include "../logging.h"
#include "../adaptivewindow.h"

/***********************************************************
                PACKET TESTS
************************************************************/

static void testPacketCreation(void)
{
    Packet p = createPacket(
                    1,
                    "Hello",
                    HIGH,
                    FILE_TRANSFER);

    CU_ASSERT_EQUAL(p.seqNo,1);
    CU_ASSERT_STRING_EQUAL(p.data,"Hello");
    CU_ASSERT_EQUAL(p.priority,HIGH);
}

static void testPacketChecksum(void)
{
    Packet p =
        createPacket(
            5,
            "ABCDE",
            LOW,
            CONTROL);

    CU_ASSERT_EQUAL(
        p.checksum,
        calculateChecksum("ABCDE"));
}

static void testPacketCopy(void)
{
    Packet p =
        createPacket(
            9,
            "Testing",
            MEDIUM,
            VIDEO);

    CU_ASSERT_STRING_EQUAL(
        p.data,
        "Testing");
}

static void testPacketSequence(void)
{
    Packet p =
        createPacket(
            25,
            "DATA",
            LOW,
            CONTROL);

    CU_ASSERT_EQUAL(
        p.seqNo,
        25);
}

static void testFragmentPacketCreation(void)
{
    Packet p =
        createFragmentPacket(
            0,
            0,
            3,
            "HELLO",
            HIGH,
            VOICE);

    CU_ASSERT_EQUAL(p.seqNo, 0);
    CU_ASSERT_EQUAL(p.fragmentNo, 0);
    CU_ASSERT_EQUAL(p.totalFragments, 3);
    CU_ASSERT_STRING_EQUAL(p.data, "HELLO");
    CU_ASSERT_EQUAL(p.priority, HIGH);
    CU_ASSERT_EQUAL(p.messageType, VOICE);
}

static void testFragmentNumbering(void)
{
    Packet p1 =
        createFragmentPacket(
            0,
            0,
            3,
            "HEL",
            HIGH,
            VOICE);

    Packet p2 =
        createFragmentPacket(
            1,
            1,
            3,
            "LO ",
            HIGH,
            VOICE);

    Packet p3 =
        createFragmentPacket(
            2,
            2,
            3,
            "END",
            HIGH,
            VOICE);

    CU_ASSERT_EQUAL(p1.fragmentNo, 0);
    CU_ASSERT_EQUAL(p2.fragmentNo, 1);
    CU_ASSERT_EQUAL(p3.fragmentNo, 2);

    CU_ASSERT_EQUAL(p1.totalFragments, 3);
    CU_ASSERT_EQUAL(p2.totalFragments, 3);
    CU_ASSERT_EQUAL(p3.totalFragments, 3);
}

static void testFragmentChecksum(void)
{
    Packet p =
        createFragmentPacket(
            0,
            0,
            1,
            "HELLO",
            HIGH,
            VOICE);

    CU_ASSERT_EQUAL(
        p.checksum,
        calculateChecksum("HELLO"));
}

static void testFragmentDataIntegrity(void)
{
    Packet p =
        createFragmentPacket(
            0,
            0,
            2,
            "ABCDE",
            HIGH,
            VOICE);

    CU_ASSERT_STRING_EQUAL(
        p.data,
        "ABCDE");
}
/***********************************************************
                BUFFER TESTS
************************************************************/

static void testBufferInitialization(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    CU_ASSERT_EQUAL(buffer.front,0);
    CU_ASSERT_EQUAL(buffer.rear,-1);
    CU_ASSERT_EQUAL(buffer.count,0);
}

static void testBufferEmpty(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    CU_ASSERT_TRUE(
        isBufferEmpty(&buffer));
}

static void testBufferEnqueue(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    Packet p =
        createPacket(
            1,
            "ABC",
            LOW,
            CONTROL);

    enqueuePacket(
        &buffer,
        p);

    CU_ASSERT_EQUAL(
        buffer.count,
        1);
}

static void testBufferPeek(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(
        &buffer,
        createPacket(
            10,
            "Hello",
            LOW,
            CONTROL));

    Packet p =
        peekPacket(
            &buffer);

    CU_ASSERT_EQUAL(
        p.seqNo,
        10);
}

static void testBufferDequeue(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(
        &buffer,
        createPacket(
            20,
            "XYZ",
            LOW,
            CONTROL));

    Packet p =
        dequeuePacket(
            &buffer);

    CU_ASSERT_EQUAL(
        p.seqNo,
        20);

    CU_ASSERT_TRUE(
        isBufferEmpty(
            &buffer));
}

static void testBufferFIFO(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(
        &buffer,
        createPacket(1,"A",LOW,CONTROL));

    enqueuePacket(
        &buffer,
        createPacket(2,"B",LOW,CONTROL));

    enqueuePacket(
        &buffer,
        createPacket(3,"C",LOW,CONTROL));

    Packet p1 =
        dequeuePacket(&buffer);

    Packet p2 =
        dequeuePacket(&buffer);

    Packet p3 =
        dequeuePacket(&buffer);

    CU_ASSERT_EQUAL(p1.seqNo,1);
    CU_ASSERT_EQUAL(p2.seqNo,2);
    CU_ASSERT_EQUAL(p3.seqNo,3);
}
/***********************************************************
 * WINDOW TESTS
 ***********************************************************/

static void testWindowInitialization(void)
{
    Window window;

    initWindow(&window, 4);

    CU_ASSERT_EQUAL(window.base, 0);
    CU_ASSERT_EQUAL(window.nextSeqNum, 0);
    CU_ASSERT_EQUAL(window.windowSize, 4);
}

static void testCanSend(void)
{
    Window window;

    initWindow(&window, 4);

    CU_ASSERT_TRUE(canSend(&window));

    window.nextSeqNum = 4;

    CU_ASSERT_FALSE(canSend(&window));
}

static void testWindowSlide(void)
{
    Window window;

    initWindow(&window, 4);

    window.nextSeqNum = 4;

    slideWindow(&window, 2);

    CU_ASSERT_EQUAL(window.base, 3);
}

static void testOutstandingPackets(void)
{
    Window window;

    initWindow(&window, 4);

    CU_ASSERT_FALSE(hasOutstandingPackets(&window));

    window.nextSeqNum = 2;

    CU_ASSERT_TRUE(hasOutstandingPackets(&window));
}

static void testMultipleSlides(void)
{
    Window window;

    initWindow(&window, 5);

    window.nextSeqNum = 5;

    slideWindow(&window, 0);
    CU_ASSERT_EQUAL(window.base, 1);

    slideWindow(&window, 2);
    CU_ASSERT_EQUAL(window.base, 3);

    slideWindow(&window, 4);
    CU_ASSERT_EQUAL(window.base, 5);
}

static void testInvalidACK(void)
{
    Window window;

    initWindow(&window, 4);

    window.base = 3;

    slideWindow(&window, 1);

    CU_ASSERT_EQUAL(window.base, 3);
}

static void testWindowLimit(void)
{
    Window window;

    initWindow(&window, 8);

    CU_ASSERT_EQUAL(window.windowSize, 8);
}

static void testDisplayWindow(void)
{
    Window window;

    initWindow(&window, 4);

    window.nextSeqNum = 2;

    displayWindow(&window, 10);

    CU_PASS("Display Window Executed");
}

/***********************************************************
 * SENDER TESTS
 ***********************************************************/

static void testSenderTransmission(void)
{
    Packet packet;

    packet = createPacket(1,
                          "Hello",
                          HIGH,
                          FILE_TRANSFER);

    sendPacket(&packet);

    CU_PASS("Send Packet Executed");
}

static void testRetransmission(void)
{
    Packet packet;

    packet = createPacket(2,
                          "Retransmit",
                          MEDIUM,
                          CONTROL);

    retransmitPacket(&packet);

    CU_PASS("Retransmission Executed");
}

static void testSenderWindowLimit(void)
{
    Packet packets[5];
    Window window;
    Timer timer;

    initWindow(&window, 3);

    for (int i = 0; i < 5; i++)
    {
        packets[i] = createPacket(i,
                                  "Packet",
                                  LOW,
                                  FILE_TRANSFER);
    }

    sendPacketsInWindow(packets,
                        &window,
                        5,
                        &timer,
                        5);

    CU_ASSERT_EQUAL(window.nextSeqNum, 3);
}

static void testGoBackNRetransmit(void)
{
    Packet packets[5];
    Window window;

    initWindow(&window, 4);

    window.base = 1;
    window.nextSeqNum = 4;

    for (int i = 0; i < 5; i++)
    {
        packets[i] = createPacket(i,
                                  "Data",
                                  LOW,
                                  FILE_TRANSFER);
    }

    goBackNRetransmit(packets, &window);

    CU_PASS("Go Back-N Retransmission");
}

static void testDisplaySenderWindow(void)
{
    Packet packets[4];
    Window window;

    initWindow(&window, 4);

    for (int i = 0; i < 4; i++)
    {
        packets[i] = createPacket(i,
                                  "Packet",
                                  LOW,
                                  FILE_TRANSFER);

        packets[i].acked = (i < 2);
    }

    window.nextSeqNum = 4;

    displaySenderWindow(packets, &window);

    CU_PASS("Display Sender Window");
}

static void testNullPacket(void)
{
    sendPacket(NULL);
    retransmitPacket(NULL);

    CU_PASS("NULL Packet Handled");
}

static void testEmptyWindowSend(void)
{
    Packet packets[2];
    Window window;
    Timer timer;

    initWindow(&window, 4);

    sendPacketsInWindow(packets,
                        &window,
                        0,
                        &timer,
                        5);

    CU_ASSERT_EQUAL(window.nextSeqNum, 0);
}

/***********************************************************
 * RECEIVER TESTS
 ***********************************************************/

static void testReceiverAcceptPacket(void)
{
    Packet packet = createPacket(0,
                                 "Hello",
                                 HIGH,
                                 FILE_TRANSFER);

    receivePacket(&packet);

    CU_PASS("Receive Packet");
}

static void testValidatePacket(void)
{
    Packet packet = createPacket(1,
                                 "Checksum",
                                 MEDIUM,
                                 CONTROL);

    CU_ASSERT_TRUE(validatePacket(&packet));
}

static void testReceiverRejectCorruptPacket(void)
{
    Packet packet = createPacket(2,
                                 "Corrupted",
                                 LOW,
                                 FILE_TRANSFER);

    packet.checksum++;

    CU_ASSERT_FALSE(validatePacket(&packet));
}

static void testGenerateACK(void)
{
    Packet packet = createPacket(5,
                                 "ACK",
                                 HIGH,
                                 CONTROL);

    int ack = generateACK(&packet);

    CU_ASSERT_EQUAL(ack, -1);
}

static void testReceiverNullPacket(void)
{
    receivePacket(NULL);

    CU_ASSERT_FALSE(validatePacket(NULL));

    CU_ASSERT_EQUAL(generateACK(NULL), -1);
}
/***********************************************************
 * ACK TESTS
 ***********************************************************/

static void testACKProcessing(void)
{
    uint16_t base = 0;

    processACK(2, &base);

    CU_ASSERT_EQUAL(base, 3);
}

static void testDuplicateACK(void)
{
    uint16_t base = 3;

    processACK(1, &base);

    CU_ASSERT_EQUAL(base, 3);
}

static void testMarkPacketAcknowledged(void)
{
    int ackArray[10] = {0};

    markPacketAcknowledged(ackArray, 4);

    CU_ASSERT_EQUAL(ackArray[4], 1);
}

static void testIsAcknowledged(void)
{
    int ackArray[10] = {0};

    ackArray[6] = 1;

    CU_ASSERT_TRUE(isAcknowledged(ackArray, 6));
    CU_ASSERT_FALSE(isAcknowledged(ackArray, 2));
}

static void testMultipleACKs(void)
{
    int ackArray[10] = {0};

    for (int i = 0; i < 5; i++)
        markPacketAcknowledged(ackArray, i);

    for (int i = 0; i < 5; i++)
        CU_ASSERT_TRUE(isAcknowledged(ackArray, i));
}

static void testDisplayACKStatus(void)
{
    int ackArray[5] = {1,0,1,1,0};

    displayACKStatus(ackArray,5);

    CU_PASS("Display ACK Status");
}

/***********************************************************
 * TIMER TESTS
 ***********************************************************/

static void testTimerStart(void)
{
    Timer timer;

    startTimer(&timer,5);

    CU_ASSERT_EQUAL(timer.timeoutSeconds,5);
}

static void testTimerTimeout(void)
{
    Timer timer;

    startTimer(&timer,1);

    sleep(2);

    CU_ASSERT_TRUE(hasTimedOut(&timer));
}

static void testTimerReset(void)
{
    Timer timer;

    startTimer(&timer,1);

    sleep(1);

    resetTimer(&timer);

    CU_ASSERT_FALSE(hasTimedOut(&timer));
}

static void testTimerNoTimeout(void)
{
    Timer timer;

    startTimer(&timer,5);

    CU_ASSERT_FALSE(hasTimedOut(&timer));
}

static void testZeroTimeout(void)
{
    Timer timer;

    startTimer(&timer,0);

    CU_ASSERT_TRUE(hasTimedOut(&timer));
}

static void testDisplayTimer(void)
{
    Timer timer;

    startTimer(&timer,3);

    displayTimer(&timer);

    CU_PASS("Display Timer");
}

static void testMultipleTimerReset(void)
{
    Timer timer;

    startTimer(&timer,2);

    resetTimer(&timer);
    resetTimer(&timer);
    resetTimer(&timer);

    CU_ASSERT_FALSE(hasTimedOut(&timer));
}

/***********************************************************
 * STATISTICS TESTS
 ***********************************************************/

static void testStatisticsInit(void)
{
    Statistics stats;

    initStatistics(&stats);

    CU_ASSERT_EQUAL(stats.sent,0);
    CU_ASSERT_EQUAL(stats.received,0);
    CU_ASSERT_EQUAL(stats.lost,0);
    CU_ASSERT_EQUAL(stats.retransmitted,0);
}

static void testSentCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent++;

    CU_ASSERT_EQUAL(stats.sent,1);
}

static void testReceivedCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.received += 5;

    CU_ASSERT_EQUAL(stats.received,5);
}

static void testLostCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.lost = 3;

    CU_ASSERT_EQUAL(stats.lost,3);
}

static void testRetransmittedCounter(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.retransmitted = 2;

    CU_ASSERT_EQUAL(stats.retransmitted,2);
}

static void testStatisticsIncrement(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent += 10;
    stats.received += 8;
    stats.lost += 2;
    stats.retransmitted += 1;

    CU_ASSERT_EQUAL(stats.sent,10);
    CU_ASSERT_EQUAL(stats.received,8);
    CU_ASSERT_EQUAL(stats.lost,2);
    CU_ASSERT_EQUAL(stats.retransmitted,1);
}

static void testPrintStatistics(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent = 15;
    stats.received = 13;
    stats.lost = 2;
    stats.retransmitted = 1;

    printStatistics(&stats);

    CU_PASS("Print Statistics");
}
/***********************************************************
 * THROUGHPUT TESTS
 ***********************************************************/

static void testThroughputCalculation(void)
{
    float throughput = calculateThroughput(100, 10.0);

    CU_ASSERT_DOUBLE_EQUAL(throughput, 10.0f, 0.001);
}

static void testZeroTime(void)
{
    CU_ASSERT_EQUAL(calculateThroughput(100, 0), 0);
}

static void testPacketDeliveryRatio(void)
{
    float pdr = calculatePDR(90, 100);

    CU_ASSERT_DOUBLE_EQUAL(pdr, 90.0f, 0.001);
}

static void testLossRate(void)
{
    float loss = calculateLossRate(10, 100);

    CU_ASSERT_DOUBLE_EQUAL(loss, 10.0f, 0.001);
}

static void testRetransmissionRate(void)
{
    float rate = calculateRetransmissionRate(20, 100);

    CU_ASSERT_DOUBLE_EQUAL(rate, 20.0f, 0.001);
}

static void testZeroPackets(void)
{
    CU_ASSERT_EQUAL(calculatePDR(0,0),0);
    CU_ASSERT_EQUAL(calculateLossRate(0,0),0);
    CU_ASSERT_EQUAL(calculateRetransmissionRate(0,0),0);
}

static void testDisplayMetrics(void)
{
    displayMetrics(100,90,10,5,10.0);

    CU_PASS("Display Metrics");
}

static void testHighThroughput(void)
{
    float throughput = calculateThroughput(1000,2.0);

    CU_ASSERT_DOUBLE_EQUAL(throughput,500.0f,0.001);
}

/***********************************************************
 * CHANNEL TESTS
 ***********************************************************/

static void testPacketLoss(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketLoss(0),0);
}

static void testPacketLossAlways(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketLoss(100),1);
}

static void testPacketDelay(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketDelay(0),0);
}

static void testPacketDelayAlways(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketDelay(100),1);
}

static void testPacketCorruption(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketCorruption(0),0);
}

static void testPacketCorruptionAlways(void)
{
    srand((unsigned int)time(NULL));

    CU_ASSERT_EQUAL(simulatePacketCorruption(100),1);
}

static void testRandomSimulation(void)
{
    srand((unsigned int)time(NULL));

    int loss = simulatePacketLoss(50);
    int delay = simulatePacketDelay(50);
    int corruption = simulatePacketCorruption(50);

    CU_ASSERT(loss==0 || loss==1);
    CU_ASSERT(delay==0 || delay==1);
    CU_ASSERT(corruption==0 || corruption==1);
}

/***********************************************************
 * LOGGING TESTS
 ***********************************************************/

static void testInitializeLog(void)
{
    initializeLog();

    FILE *fp = fopen("transmission.log","r");

    CU_ASSERT_PTR_NOT_NULL(fp);

    if(fp)
        fclose(fp);

    closeLog();
}

static void testLogEvent(void)
{
    initializeLog();

    logEvent("Testing Event");

    CU_PASS("Log Event");

    closeLog();
}

static void testLogPacketEvent(void)
{
    initializeLog();

    logPacketEvent("SEND",5);

    CU_PASS("Packet Event");

    closeLog();
}

static void testMultipleLogEvents(void)
{
    initializeLog();

    logEvent("Simulation Started");
    logPacketEvent("SEND",1);
    logPacketEvent("ACK",1);
    logEvent("Simulation Finished");

    CU_PASS("Multiple Events");

    closeLog();
}

static void testCloseLog(void)
{
    initializeLog();

    closeLog();
    closeLog();

    CU_PASS("Close Log");
}

static void testLogWithoutInitialization(void)
{
    logEvent("Test");
    logPacketEvent("SEND",2);

    CU_PASS("Logging Without Initialization");
}

static void testReinitializeLog(void)
{
    initializeLog();
    closeLog();

    initializeLog();

    logEvent("Reinitialized");

    CU_PASS("Reinitialize Log");

    closeLog();
}

/***********************************************************
 * ADAPTIVE WINDOW TESTS
 ***********************************************************/

static void testIncreaseWindow(void)
{
    Window window;

    initWindow(&window,5);

    increaseWindow(&window);

    CU_ASSERT_EQUAL(window.windowSize,6);
}

static void testDecreaseWindow(void)
{
    Window window;

    initWindow(&window,5);

    decreaseWindow(&window);

    CU_ASSERT_EQUAL(window.windowSize,4);
}

static void testIncreaseToMaximum(void)
{
    Window window;

    initWindow(&window,MAX_WINDOW_SIZE);

    increaseWindow(&window);

    CU_ASSERT_EQUAL(window.windowSize,MAX_WINDOW_SIZE);
}

static void testDecreaseToMinimum(void)
{
    Window window;

    initWindow(&window,MIN_WINDOW_SIZE);

    decreaseWindow(&window);

    CU_ASSERT_EQUAL(window.windowSize,MIN_WINDOW_SIZE);
}

static void testAdaptiveIncrease(void)
{
    Window window;

    initWindow(&window,5);

    adjustWindow(&window,0);

    CU_ASSERT_EQUAL(window.windowSize,6);
}

static void testAdaptiveDecrease(void)
{
    Window window;

    initWindow(&window,5);

    adjustWindow(&window,1);

    CU_ASSERT_EQUAL(window.windowSize,4);
}

static void testDisplayAdaptiveWindow(void)
{
    Window window;

    initWindow(&window,5);

    displayAdaptiveWindow(&window);

    CU_PASS("Display Adaptive Window");
}
/***********************************************************
 * INTEGRATION TESTS
 ***********************************************************/

static void testSenderReceiverACK(void)
{
    Packet packet;
    Window senderWindow;
    Buffer buffer;
    Statistics stats;

    initWindow(&senderWindow, 4);
    initBuffer(&buffer);
    initStatistics(&stats);

    packet = createPacket(0,
                          "Hello",
                          HIGH,
                          CONTROL);

    enqueuePacket(&buffer, packet);

    Packet txPacket = dequeuePacket(&buffer);

    sendPacket(&txPacket);
    receivePacket(&txPacket);

    int ack = generateACK(&txPacket);

    processACK(ack, &senderWindow.base);

    CU_ASSERT_EQUAL(senderWindow.base, 1);
}

static void testWindowSlidingIntegration(void)
{
    Window window;

    initWindow(&window, 4);

    processACK(0, &window.base);
    CU_ASSERT_EQUAL(window.base, 1);

    processACK(1, &window.base);
    CU_ASSERT_EQUAL(window.base, 2);
}

static void testLossRetransmissionIntegration(void)
{
    Packet packet;

    packet = createPacket(1,
                          "DATA",
                          MEDIUM,
                          FILE_TRANSFER);

    if (simulatePacketLoss(100))
    {
        retransmitPacket(&packet);
    }

    CU_PASS("Loss and Retransmission");
}

static void testAdaptiveWindowIntegration(void)
{
    Window window;

    initWindow(&window, 5);

    adjustWindow(&window, 1);
    CU_ASSERT_EQUAL(window.windowSize, 4);

    adjustWindow(&window, 0);
    CU_ASSERT_EQUAL(window.windowSize, 5);
}

static void testStatisticsIntegration(void)
{
    Statistics stats;

    initStatistics(&stats);

    stats.sent = 100;
    stats.received = 90;
    stats.lost = 10;
    stats.retransmitted = 5;

    CU_ASSERT_DOUBLE_EQUAL(
        calculatePDR(stats.received, stats.sent),
        90.0,
        0.001);

    CU_ASSERT_DOUBLE_EQUAL(
        calculateLossRate(stats.lost, stats.sent),
        10.0,
        0.001);
}

static void testCompleteTransmission(void)
{
    Packet packet;
    Window window;
    Buffer buffer;
    Statistics stats;

    initWindow(&window, 4);
    initBuffer(&buffer);
    initStatistics(&stats);

    packet = createPacket(0,
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

    CU_ASSERT_EQUAL(stats.sent, 1);
    CU_ASSERT_EQUAL(stats.received, 1);
    CU_ASSERT_EQUAL(window.base, 1);
}

/***********************************************************
 * MAIN
 ***********************************************************/

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("5G Reliable Transmission Engine", NULL, NULL);

    /* Packet */
    CU_add_test(suite, "Packet Creation", testPacketCreation);
    CU_add_test(suite, "Packet Checksum", testPacketChecksum);
    CU_add_test(suite, "Packet Copy", testPacketCopy);
    CU_add_test(suite, "Packet Sequence", testPacketSequence);

    /* Buffer */
    CU_add_test(suite, "Buffer Init", testBufferInitialization);
    CU_add_test(suite, "Buffer Empty", testBufferEmpty);
    CU_add_test(suite, "Buffer Enqueue", testBufferEnqueue);
    CU_add_test(suite, "Buffer Peek", testBufferPeek);
    CU_add_test(suite, "Buffer Dequeue", testBufferDequeue);
    CU_add_test(suite, "Buffer FIFO", testBufferFIFO);

    /* Window */
    CU_add_test(suite, "Window Init", testWindowInitialization);
    CU_add_test(suite, "Can Send", testCanSend);
    CU_add_test(suite, "Window Slide", testWindowSlide);
    CU_add_test(suite, "Outstanding Packets", testOutstandingPackets);
    CU_add_test(suite, "Multiple Slides", testMultipleSlides);
    CU_add_test(suite, "Invalid ACK", testInvalidACK);
    CU_add_test(suite, "Window Limit", testWindowLimit);
    CU_add_test(suite, "Display Window", testDisplayWindow);

    /* Sender */
    CU_add_test(suite, "Send Packet", testSenderTransmission);
    CU_add_test(suite, "Retransmission", testRetransmission);
    CU_add_test(suite, "Window Send", testSenderWindowLimit);
    CU_add_test(suite, "Go Back N", testGoBackNRetransmit);
    CU_add_test(suite, "Display Sender", testDisplaySenderWindow);
    CU_add_test(suite, "NULL Packet", testNullPacket);
    CU_add_test(suite, "Empty Window", testEmptyWindowSend);

    /* Receiver */
    CU_add_test(suite, "Receive Packet", testReceiverAcceptPacket);
    CU_add_test(suite, "Validate Packet", testValidatePacket);
    CU_add_test(suite, "Reject Corrupted", testReceiverRejectCorruptPacket);
    CU_add_test(suite, "Generate ACK", testGenerateACK);
    CU_add_test(suite, "Receiver NULL", testReceiverNullPacket);

    /* ACK */
    CU_add_test(suite, "ACK Processing", testACKProcessing);
    CU_add_test(suite, "Duplicate ACK", testDuplicateACK);
    CU_add_test(suite, "Mark ACK", testMarkPacketAcknowledged);
    CU_add_test(suite, "Is ACK", testIsAcknowledged);
    CU_add_test(suite, "Multiple ACK", testMultipleACKs);
    CU_add_test(suite, "Display ACK", testDisplayACKStatus);

    /* Timer */
    CU_add_test(suite, "Timer Start", testTimerStart);
    CU_add_test(suite, "Timer Timeout", testTimerTimeout);
    CU_add_test(suite, "Timer Reset", testTimerReset);
    CU_add_test(suite, "No Timeout", testTimerNoTimeout);
    CU_add_test(suite, "Zero Timeout", testZeroTimeout);
    CU_add_test(suite, "Display Timer", testDisplayTimer);
    CU_add_test(suite, "Multiple Reset", testMultipleTimerReset);

    /* Statistics */
    CU_add_test(suite, "Statistics Init", testStatisticsInit);
    CU_add_test(suite, "Sent Counter", testSentCounter);
    CU_add_test(suite, "Received Counter", testReceivedCounter);
    CU_add_test(suite, "Lost Counter", testLostCounter);
    CU_add_test(suite, "Retransmitted Counter", testRetransmittedCounter);
    CU_add_test(suite, "Statistics Increment", testStatisticsIncrement);
    CU_add_test(suite, "Print Statistics", testPrintStatistics);

    /* Throughput, Channel, Logging, Adaptive Window */
    CU_add_test(suite, "Throughput", testThroughputCalculation);
    CU_add_test(suite, "Channel", testPacketLoss);
    CU_add_test(suite, "Logging", testInitializeLog);
    CU_add_test(suite, "Adaptive Window", testIncreaseWindow);

    /* Integration */
    CU_add_test(suite, "Sender Receiver ACK", testSenderReceiverACK);
    CU_add_test(suite, "Window Sliding", testWindowSlidingIntegration);
    CU_add_test(suite, "Loss Retransmission", testLossRetransmissionIntegration);
    CU_add_test(suite, "Adaptive Window Integration", testAdaptiveWindowIntegration);
    CU_add_test(suite, "Statistics Integration", testStatisticsIntegration);
    CU_add_test(suite, "Complete Transmission", testCompleteTransmission);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}