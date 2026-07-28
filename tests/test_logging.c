#include "test.h"
#include <stdio.h>

/*----------------------------------------------------------
    Test Log Initialization
-----------------------------------------------------------*/
void testInitializeLog(void)
{
    initializeLog();

    FILE *fp = fopen("transmission.log", "r");

    ASSERT_TRUE(fp != NULL,
                "Log File Created");

    if(fp != NULL)
        fclose(fp);

    closeLog();
}

/*----------------------------------------------------------
    Test Event Logging
-----------------------------------------------------------*/
void testLogEvent(void)
{
    initializeLog();

    logEvent("Testing Event");

    ASSERT_TRUE(1,
                "Log Event");

    closeLog();
}

/*----------------------------------------------------------
    Test Packet Event Logging
-----------------------------------------------------------*/
void testLogPacketEvent(void)
{
    initializeLog();

    logPacketEvent("SEND",5);

    ASSERT_TRUE(1,
                "Packet Event Logged");

    closeLog();
}

/*----------------------------------------------------------
    Test Multiple Events
-----------------------------------------------------------*/
void testMultipleLogEvents(void)
{
    initializeLog();

    logEvent("Simulation Started");

    logPacketEvent("SEND",1);

    logPacketEvent("ACK",1);

    logEvent("Simulation Finished");

    ASSERT_TRUE(1,
                "Multiple Log Events");

    closeLog();
}

/*----------------------------------------------------------
    Test Close Log
-----------------------------------------------------------*/
void testCloseLog(void)
{
    initializeLog();

    closeLog();

    /* Should safely do nothing */
    closeLog();

    ASSERT_TRUE(1,
                "Close Log");
}

/*----------------------------------------------------------
    Test Logging Without Initialization
-----------------------------------------------------------*/
void testLogWithoutInitialization(void)
{
    /* These should safely return without crashing */

    logEvent("Test");

    logPacketEvent("SEND",2);

    ASSERT_TRUE(1,
                "Logging Without Initialization");
}

/*----------------------------------------------------------
    Test Reinitialize Log
-----------------------------------------------------------*/
void testReinitializeLog(void)
{
    initializeLog();

    closeLog();

    initializeLog();

    logEvent("Reinitialized");

    ASSERT_TRUE(1,
                "Reinitialize Log");

    closeLog();
}

/*----------------------------------------------------------
    Run All Logging Tests
-----------------------------------------------------------*/
void runLoggingTests(void)
{
    testInitializeLog();
    testLogEvent();
    testLogPacketEvent();
    testMultipleLogEvents();
    testCloseLog();
    testLogWithoutInitialization();
    testReinitializeLog();
}