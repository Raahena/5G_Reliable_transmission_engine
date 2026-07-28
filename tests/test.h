#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Project Headers */
#include "../packet.h"
#include "../buffer.h"
#include "../window.h"
#include "../sender.h"
#include "../receiver.h"
#include "../ack.h"
#include "../timer.h"
#include "../channel.h"
#include "../adaptivewindow.h"
#include "../statistics.h"
#include "../throughput.h"
#include "../logging.h"
#include "../config.h"

/*----------------------------------------------------------
    Global Test Counters
-----------------------------------------------------------*/
extern int totalTests;
extern int passedTests;

/*----------------------------------------------------------
    Assertion Macros
-----------------------------------------------------------*/
#define ASSERT_TRUE(condition, message)                     \
do                                                          \
{                                                           \
    totalTests++;                                           \
    if(condition)                                           \
    {                                                       \
        passedTests++;                                      \
        printf("[PASS] %s\n", message);                     \
    }                                                       \
    else                                                    \
    {                                                       \
        printf("[FAIL] %s\n", message);                     \
    }                                                       \
} while(0)

#define ASSERT_EQUAL(expected, actual, message)             \
do                                                          \
{                                                           \
    totalTests++;                                           \
    if((expected) == (actual))                             \
    {                                                       \
        passedTests++;                                      \
        printf("[PASS] %s\n", message);                     \
    }                                                       \
    else                                                    \
    {                                                       \
        printf("[FAIL] %s\n", message);                     \
        printf("Expected : %d\n", (int)(expected));         \
        printf("Actual   : %d\n", (int)(actual));           \
    }                                                       \
} while(0)

/*----------------------------------------------------------
    Packet Tests
-----------------------------------------------------------*/
void testCreatePacket(void);
void testChecksum(void);
void testPacketPriority(void);
void testPacketType(void);
void testPayloadIntegrity(void);
void testEmptyPayload(void);
void testLargePayload(void);
void testCorruptedPacket(void);
void testSequenceNumber(void);
void runPacketTests(void);
void testMultiplePacketCreation(void);

/*----------------------------------------------------------
    Buffer Tests
-----------------------------------------------------------*/
void testBufferInitialization(void);
void testBufferEnqueue(void);
void testBufferDequeue(void);
void testBufferPeek(void);
void testBufferFull(void);
void testBufferEmpty(void);
void testBufferOverflow(void);
void testBufferUnderflow(void);
void testFIFOOrder(void);
void testMultipleOperations(void);
void testCircularBuffer(void);
void runBufferTests(void);
void testBufferCount(void);

/*----------------------------------------------------------
    Window Tests
-----------------------------------------------------------*/
void testWindowInitialization(void);
void testWindowSlide(void);
void testCanSend(void);
void runWindowTests(void);
void testMultipleSlides(void);
void testInvalidACK(void);
void testWindowLimit(void);
void testDisplayWindow(void);
void testOutstandingPackets(void);

/*----------------------------------------------------------
    Sender Tests
-----------------------------------------------------------*/
void testGoBackNRetransmit(void);
void testDisplaySenderWindow(void);
void testNullPacket(void);
void testEmptyWindowSend(void);
void runSenderTests(void);

/*----------------------------------------------------------
    Receiver Tests
-----------------------------------------------------------*/
void testValidatePacket(void);
void testGenerateACK(void);
void testReceiverNullPacket(void);

void runReceiverTests(void);

/*----------------------------------------------------------
    ACK Tests
-----------------------------------------------------------*/
void testMarkPacketAcknowledged(void);
void testIsAcknowledged(void);
void testMultipleACKs(void);
void testDisplayACKStatus(void);

void runACKTests(void);
/*----------------------------------------------------------
    Timer Tests
-----------------------------------------------------------*/
void testTimerNoTimeout(void);
void testZeroTimeout(void);
void testDisplayTimer(void);
void testMultipleTimerReset(void);

void runTimerTests(void);

/*----------------------------------------------------------
    Channel Tests
-----------------------------------------------------------*/
void testPacketLossAlways(void);
void testPacketDelayAlways(void);
void testPacketCorruptionAlways(void);
void testRandomSimulation(void);

void runChannelTests(void);

/*----------------------------------------------------------
    Adaptive Window Tests
-----------------------------------------------------------*/
void testIncreaseToMaximum(void);
void testDecreaseToMinimum(void);
void testAdaptiveIncrease(void);
void testAdaptiveDecrease(void);
void testDisplayAdaptiveWindow(void);

void runAdaptiveWindowTests(void);

/*----------------------------------------------------------
    Statistics Tests
-----------------------------------------------------------*/
void testSentCounter(void);
void testReceivedCounter(void);
void testLostCounter(void);
void testRetransmittedCounter(void);
void testPrintStatistics(void);

void runStatisticsTests(void);

/*----------------------------------------------------------
    Throughput Tests
-----------------------------------------------------------*/
void testZeroTime(void);
void testPacketDeliveryRatio(void);
void testLossRate(void);
void testRetransmissionRate(void);
void testZeroPackets(void);
void testDisplayMetrics(void);
void testHighThroughput(void);

void runThroughputTests(void);

/*----------------------------------------------------------
    Logging Tests
-----------------------------------------------------------*/
void testInitializeLog(void);
void testLogEvent(void);
void testLogPacketEvent(void);
void testMultipleLogEvents(void);
void testCloseLog(void);
void testLogWithoutInitialization(void);
void testReinitializeLog(void);

void runLoggingTests(void);



/*----------------------------------------------------------
    Integration Tests
-----------------------------------------------------------*/
void testSenderReceiverACK(void);
void testWindowSliding(void);
void testLossRetransmission(void);
void testAdaptiveWindowControl(void);
void testStatisticsMetrics(void);
void testCompleteTransmission(void);

void runAllIntegrationTests(void);
/*----------------------------------------------------------
    Test Runner
-----------------------------------------------------------*/
void runAllUnitTests(void);


#endif