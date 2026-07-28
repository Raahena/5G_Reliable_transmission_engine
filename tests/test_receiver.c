#include "test.h"

/*----------------------------------------------------------
    Test Receive Packet
-----------------------------------------------------------*/
void testReceiverAcceptPacket(void)
{
    Packet packet = createPacket(0,
                                 "Hello",
                                 HIGH,
                                 FILE_TRANSFER);

    receivePacket(&packet);

    ASSERT_TRUE(1,
                "Receive Packet");
}

/*----------------------------------------------------------
    Test Validate Packet
-----------------------------------------------------------*/
void testValidatePacket(void)
{
    Packet packet = createPacket(1,
                                 "Checksum",
                                 MEDIUM,
                                 CONTROL);

    ASSERT_TRUE(validatePacket(&packet),
                "Valid Packet");
}

/*----------------------------------------------------------
    Test Corrupted Packet
-----------------------------------------------------------*/
void testReceiverRejectCorruptPacket(void)
{
    Packet packet = createPacket(2,
                                 "Corrupted",
                                 LOW,
                                 FILE_TRANSFER);

    packet.checksum++;

    ASSERT_TRUE(!validatePacket(&packet),
                "Corrupted Packet Rejected");
}

/*----------------------------------------------------------
    Test Generate ACK
-----------------------------------------------------------*/
void testGenerateACK(void)
{
    Packet packet = createPacket(5,
                                 "ACK",
                                 HIGH,
                                 CONTROL);

    int ack = generateACK(&packet);

    ASSERT_EQUAL(-1,
                 ack,
                 "Generate ACK");
}

/*----------------------------------------------------------
    Test NULL Packet
-----------------------------------------------------------*/
void testReceiverNullPacket(void)
{
    receivePacket(NULL);

    ASSERT_TRUE(!validatePacket(NULL),
                "NULL Packet Validation");

    ASSERT_EQUAL(-1,
                 generateACK(NULL),
                 "NULL ACK");
}

/*----------------------------------------------------------
    Run All Receiver Tests
-----------------------------------------------------------*/
void runReceiverTests(void)
{
    testReceiverAcceptPacket();
    testValidatePacket();
    testReceiverRejectCorruptPacket();
    testGenerateACK();
    testReceiverNullPacket();
}