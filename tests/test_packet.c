#include "test.h"

/*----------------------------------------------------------
    Test Packet Creation
-----------------------------------------------------------*/
void testCreatePacket(void)
{
    Packet p = createPacket(1,
                            "Hello World",
                            HIGH,
                            FILE_TRANSFER);

    ASSERT_EQUAL(1,
                 p.seqNo,
                 "Sequence Number");

    ASSERT_TRUE(strcmp(p.data, "Hello World") == 0,
                "Payload Copied");

    ASSERT_EQUAL(HIGH,
                 p.priority,
                 "Priority Assigned");

    ASSERT_EQUAL(FILE_TRANSFER,
                 p.messageType,
                 "Message Type Assigned");

    ASSERT_EQUAL(0,
                 p.acked,
                 "ACK Initially Zero");
}

/*----------------------------------------------------------
    Test Checksum Generation
-----------------------------------------------------------*/
void testChecksum(void)
{
    Packet p = createPacket(2,
                            "Checksum",
                            MEDIUM,
                            CONTROL);

    ASSERT_EQUAL(calculateChecksum(p.data),
                 p.checksum,
                 "Checksum Correct");
}

/*----------------------------------------------------------
    Test Priority Assignment
-----------------------------------------------------------*/
void testPacketPriority(void)
{
    Packet low =
        createPacket(1,"Low",LOW,VOICE);

    Packet med =
        createPacket(2,"Medium",MEDIUM,VIDEO);

    Packet high =
        createPacket(3,"High",HIGH,IOT);

    ASSERT_EQUAL(LOW,
                 low.priority,
                 "LOW Priority");

    ASSERT_EQUAL(MEDIUM,
                 med.priority,
                 "MEDIUM Priority");

    ASSERT_EQUAL(HIGH,
                 high.priority,
                 "HIGH Priority");
}

/*----------------------------------------------------------
    Test Message Types
-----------------------------------------------------------*/
void testPacketType(void)
{
    ASSERT_EQUAL(VOICE,
        createPacket(1,"A",LOW,VOICE).messageType,
        "VOICE");

    ASSERT_EQUAL(VIDEO,
        createPacket(2,"A",LOW,VIDEO).messageType,
        "VIDEO");

    ASSERT_EQUAL(FILE_TRANSFER,
        createPacket(3,"A",LOW,FILE_TRANSFER).messageType,
        "FILE_TRANSFER");

    ASSERT_EQUAL(IOT,
        createPacket(4,"A",LOW,IOT).messageType,
        "IOT");

    ASSERT_EQUAL(CONTROL,
        createPacket(5,"A",LOW,CONTROL).messageType,
        "CONTROL");
}

/*----------------------------------------------------------
    Test Payload Integrity
-----------------------------------------------------------*/
void testPayloadIntegrity(void)
{
    Packet p =
        createPacket(10,
                     "Networking Lab",
                     HIGH,
                     FILE_TRANSFER);

    ASSERT_TRUE(strcmp(p.data,
                       "Networking Lab") == 0,
                "Payload Integrity");
}

/*----------------------------------------------------------
    Test Empty Payload
-----------------------------------------------------------*/
void testEmptyPayload(void)
{
    Packet p =
        createPacket(11,
                     "",
                     LOW,
                     FILE_TRANSFER);

    ASSERT_EQUAL(0,
                 strlen(p.data),
                 "Empty Payload");
}

/*----------------------------------------------------------
    Test Maximum Payload
-----------------------------------------------------------*/
void testLargePayload(void)
{
    char data[DATA_SIZE];

    memset(data,'A',DATA_SIZE-1);
    data[DATA_SIZE-1]='\0';

    Packet p =
        createPacket(12,
                     data,
                     HIGH,
                     FILE_TRANSFER);

    ASSERT_TRUE(strlen(p.data) > 0,
                "Large Payload");
}

/*----------------------------------------------------------
    Test Corrupted Checksum
-----------------------------------------------------------*/
void testCorruptedPacket(void)
{
    Packet p =
        createPacket(13,
                     "Checksum",
                     HIGH,
                     FILE_TRANSFER);

    p.checksum++;

    ASSERT_TRUE(calculateChecksum(p.data)
                != p.checksum,
                "Checksum Corruption Detected");
}

/*----------------------------------------------------------
    Test Sequence Number
-----------------------------------------------------------*/
void testSequenceNumber(void)
{
    Packet p =
        createPacket(255,
                     "Seq",
                     LOW,
                     CONTROL);

    ASSERT_EQUAL(255,
                 p.seqNo,
                 "Sequence Number");
}

/*----------------------------------------------------------
    Test Multiple Packets
-----------------------------------------------------------*/
void testMultiplePacketCreation(void)
{
    int i;

    for(i=0;i<20;i++)
    {
        Packet p =
            createPacket(i,
                         "Packet",
                         LOW,
                         FILE_TRANSFER);

        ASSERT_EQUAL(i,
                     p.seqNo,
                     "Packet Created");
    }
}

/*----------------------------------------------------------
    Display Packet Test
-----------------------------------------------------------*/
void testDisplayPacket(void)
{
    Packet p =
        createPacket(100,
                     "Display Test",
                     HIGH,
                     CONTROL);

    printf("\nDisplaying Packet:\n");

    displayPacket(p);

    ASSERT_TRUE(1,
                "Display Packet Executed");
}
/*----------------------------------------------------------
    Run All Packet Tests
-----------------------------------------------------------*/
void runPacketTests(void)
{
    testCreatePacket();
    testChecksum();
    testPacketPriority();
    testPacketType();
    testPayloadIntegrity();
    testEmptyPayload();
    testLargePayload();
    testCorruptedPacket();
    testSequenceNumber();
    testMultiplePacketCreation();
    testDisplayPacket();
}