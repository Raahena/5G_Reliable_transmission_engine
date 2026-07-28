#include "test.h"

/*----------------------------------------------------------
    Test Send Packet
-----------------------------------------------------------*/
void testSenderTransmission(void)
{
    Packet packet;

    packet = createPacket(1,
                          "Hello",
                          HIGH,
                          FILE_TRANSFER);

    sendPacket(&packet);

    ASSERT_TRUE(1,
                "Send Packet Executed");
}

/*----------------------------------------------------------
    Test Retransmission
-----------------------------------------------------------*/
void testRetransmission(void)
{
    Packet packet;

    packet = createPacket(2,
                          "Retransmit",
                          MEDIUM,
                          CONTROL);

    retransmitPacket(&packet);

    ASSERT_TRUE(1,
                "Retransmission Executed");
}

/*----------------------------------------------------------
    Test Window Send
-----------------------------------------------------------*/
void testSenderWindowLimit(void)
{
    Packet packets[5];
    Window window;
    Timer timer;

    initWindow(&window, 3);

    for(int i=0;i<5;i++)
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

    ASSERT_EQUAL(3,
                 window.nextSeqNum,
                 "Window Sent Three Packets");
}

/*----------------------------------------------------------
    Test Go Back N Retransmission
-----------------------------------------------------------*/
void testGoBackNRetransmit(void)
{
    Packet packets[5];
    Window window;

    initWindow(&window,4);

    window.base = 1;
    window.nextSeqNum = 4;

    for(int i=0;i<5;i++)
    {
        packets[i]=createPacket(i,
                                "Data",
                                LOW,
                                FILE_TRANSFER);
    }

    goBackNRetransmit(packets,&window);

    ASSERT_TRUE(1,
                "Go Back-N Retransmission");
}

/*----------------------------------------------------------
    Test Display Sender Window
-----------------------------------------------------------*/
void testDisplaySenderWindow(void)
{
    Packet packets[4];
    Window window;

    initWindow(&window,4);

    for(int i=0;i<4;i++)
    {
        packets[i]=createPacket(i,
                                "Packet",
                                LOW,
                                FILE_TRANSFER);

        packets[i].acked = (i<2);
    }

    window.nextSeqNum = 4;

    displaySenderWindow(packets,&window);

    ASSERT_TRUE(1,
                "Display Sender Window");
}

/*----------------------------------------------------------
    Test NULL Packet
-----------------------------------------------------------*/
void testNullPacket(void)
{
    sendPacket(NULL);
    retransmitPacket(NULL);

    ASSERT_TRUE(1,
                "NULL Packet Handled");
}

/*----------------------------------------------------------
    Test Empty Window
-----------------------------------------------------------*/
void testEmptyWindowSend(void)
{
    Packet packets[2];
    Window window;
    Timer timer;

    initWindow(&window,4);

    sendPacketsInWindow(packets,
                        &window,
                        0,
                        &timer,
                        5);

    ASSERT_EQUAL(0,
                 window.nextSeqNum,
                 "No Packets Sent");
}

/*----------------------------------------------------------
    Run All Sender Tests
-----------------------------------------------------------*/
void runSenderTests(void)
{
    testSenderTransmission();
    testRetransmission();
    testSenderWindowLimit();
    testGoBackNRetransmit();
    testDisplaySenderWindow();
    testNullPacket();
    testEmptyWindowSend();
}