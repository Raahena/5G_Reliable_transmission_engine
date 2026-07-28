#include "test.h"

/*----------------------------------------------------------
    Test Process ACK
-----------------------------------------------------------*/
void testACKProcessing(void)
{
    uint16_t base = 0;

    processACK(2, &base);

    ASSERT_EQUAL(3,
                 base,
                 "ACK Processing");
}

/*----------------------------------------------------------
    Test Duplicate ACK
-----------------------------------------------------------*/
void testDuplicateACK(void)
{
    uint16_t base = 3;

    processACK(1, &base);

    ASSERT_EQUAL(3,
                 base,
                 "Duplicate ACK Ignored");
}

/*----------------------------------------------------------
    Test Mark Packet Acknowledged
-----------------------------------------------------------*/
void testMarkPacketAcknowledged(void)
{
    int ackArray[10] = {0};

    markPacketAcknowledged(ackArray, 4);

    ASSERT_EQUAL(1,
                 ackArray[4],
                 "Packet Marked ACK");
}

/*----------------------------------------------------------
    Test ACK Status
-----------------------------------------------------------*/
void testIsAcknowledged(void)
{
    int ackArray[10] = {0};

    ackArray[6] = 1;

    ASSERT_TRUE(isAcknowledged(ackArray, 6),
                "Packet Acknowledged");

    ASSERT_TRUE(!isAcknowledged(ackArray, 2),
                "Packet Not Acknowledged");
}

/*----------------------------------------------------------
    Test Multiple ACKs
-----------------------------------------------------------*/
void testMultipleACKs(void)
{
    int ackArray[10] = {0};

    for(int i = 0; i < 5; i++)
    {
        markPacketAcknowledged(ackArray, i);
    }

    int pass = 1;

    for(int i = 0; i < 5; i++)
    {
        if(!isAcknowledged(ackArray, i))
        {
            pass = 0;
            break;
        }
    }

    ASSERT_TRUE(pass,
                "Multiple ACKs");
}

/*----------------------------------------------------------
    Test ACK Table Display
-----------------------------------------------------------*/
void testDisplayACKStatus(void)
{
    int ackArray[5] = {1,0,1,1,0};

    displayACKStatus(ackArray,5);

    ASSERT_TRUE(1,
                "Display ACK Status");
}

/*----------------------------------------------------------
    Run All ACK Tests
-----------------------------------------------------------*/
void runACKTests(void)
{
    testACKProcessing();
    testDuplicateACK();
    testMarkPacketAcknowledged();
    testIsAcknowledged();
    testMultipleACKs();
    testDisplayACKStatus();
}