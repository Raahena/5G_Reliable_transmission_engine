#include "test.h"

/*----------------------------------------------------------
    Test Buffer Initialization
-----------------------------------------------------------*/
void testBufferInitialization(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    ASSERT_EQUAL(0, buffer.front, "Front Initialized");
    ASSERT_EQUAL(-1, buffer.rear, "Rear Initialized");
    ASSERT_EQUAL(0, buffer.count, "Count Initialized");
}

/*----------------------------------------------------------
    Test Buffer Empty
-----------------------------------------------------------*/
void testBufferEmpty(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    ASSERT_TRUE(isBufferEmpty(&buffer),
                "Buffer Empty");
}

/*----------------------------------------------------------
    Test Buffer Full
-----------------------------------------------------------*/
void testBufferFull(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    for(int i=0;i<BUFFER_SIZE;i++)
    {
        Packet p = createPacket(i,
                                "Data",
                                LOW,
                                FILE_TRANSFER);

        enqueuePacket(&buffer,p);
    }

    ASSERT_TRUE(isBufferFull(&buffer),
                "Buffer Full");
}

/*----------------------------------------------------------
    Test Enqueue
-----------------------------------------------------------*/
void testBufferEnqueue(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    Packet p = createPacket(1,
                            "Hello",
                            HIGH,
                            FILE_TRANSFER);

    enqueuePacket(&buffer,p);

    ASSERT_EQUAL(1,
                 buffer.count,
                 "Packet Enqueued");
}

/*----------------------------------------------------------
    Test Dequeue
-----------------------------------------------------------*/
void testBufferDequeue(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    Packet p=createPacket(5,
                          "Hello",
                          HIGH,
                          FILE_TRANSFER);

    enqueuePacket(&buffer,p);

    Packet out=dequeuePacket(&buffer);

    ASSERT_EQUAL(5,
                 out.seqNo,
                 "Correct Packet Removed");

    ASSERT_EQUAL(0,
                 buffer.count,
                 "Count Updated");
}

/*----------------------------------------------------------
    Test Peek Packet
-----------------------------------------------------------*/
void testBufferPeek(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    Packet p=createPacket(10,
                          "Test",
                          MEDIUM,
                          CONTROL);

    enqueuePacket(&buffer,p);

    Packet front=peekPacket(&buffer);

    ASSERT_EQUAL(10,
                 front.seqNo,
                 "Peek Packet");
}

/*----------------------------------------------------------
    Test Underflow
-----------------------------------------------------------*/
void testBufferUnderflow(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    Packet p=dequeuePacket(&buffer);

    ASSERT_EQUAL(-1,
                 p.seqNo,
                 "Buffer Underflow");
}

/*----------------------------------------------------------
    Test Overflow
-----------------------------------------------------------*/
void testBufferOverflow(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    for(int i=0;i<BUFFER_SIZE;i++)
    {
        Packet p=createPacket(i,
                              "Overflow",
                              LOW,
                              FILE_TRANSFER);

        enqueuePacket(&buffer,p);
    }

    Packet extra=createPacket(99,
                              "Extra",
                              LOW,
                              FILE_TRANSFER);

    enqueuePacket(&buffer,extra);

    ASSERT_EQUAL(BUFFER_SIZE,
                 buffer.count,
                 "Overflow Prevented");
}

/*----------------------------------------------------------
    Test FIFO Order
-----------------------------------------------------------*/
void testFIFOOrder(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(&buffer,
                  createPacket(1,"A",LOW,VOICE));

    enqueuePacket(&buffer,
                  createPacket(2,"B",LOW,VOICE));

    enqueuePacket(&buffer,
                  createPacket(3,"C",LOW,VOICE));

    ASSERT_EQUAL(1,
                 dequeuePacket(&buffer).seqNo,
                 "FIFO Packet 1");

    ASSERT_EQUAL(2,
                 dequeuePacket(&buffer).seqNo,
                 "FIFO Packet 2");

    ASSERT_EQUAL(3,
                 dequeuePacket(&buffer).seqNo,
                 "FIFO Packet 3");
}

/*----------------------------------------------------------
    Test Multiple Operations
-----------------------------------------------------------*/
void testMultipleOperations(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    for(int i=0;i<10;i++)
    {
        enqueuePacket(&buffer,
            createPacket(i,
                         "Data",
                         LOW,
                         FILE_TRANSFER));
    }

    for(int i=0;i<5;i++)
    {
        dequeuePacket(&buffer);
    }

    ASSERT_EQUAL(5,
                 buffer.count,
                 "Multiple Operations");
}

/*----------------------------------------------------------
    Test Circular Buffer
-----------------------------------------------------------*/
void testCircularBuffer(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    for(int i=0;i<BUFFER_SIZE;i++)
        enqueuePacket(&buffer,
            createPacket(i,
                         "Data",
                         LOW,
                         FILE_TRANSFER));

    for(int i=0;i<20;i++)
        dequeuePacket(&buffer);

    for(int i=0;i<20;i++)
        enqueuePacket(&buffer,
            createPacket(100+i,
                         "New",
                         LOW,
                         FILE_TRANSFER));

    ASSERT_EQUAL(BUFFER_SIZE,
                 buffer.count,
                 "Circular Buffer");
}

/*----------------------------------------------------------
    Test Buffer Count
-----------------------------------------------------------*/
void testBufferCount(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(&buffer,
        createPacket(1,"A",LOW,VOICE));

    enqueuePacket(&buffer,
        createPacket(2,"B",LOW,VOICE));

    ASSERT_EQUAL(2,
                 buffer.count,
                 "Buffer Count");
}

/*----------------------------------------------------------
    Test Display Buffer
-----------------------------------------------------------*/
void testDisplayBuffer(void)
{
    Buffer buffer;

    initBuffer(&buffer);

    enqueuePacket(&buffer,
        createPacket(1,"Display",LOW,CONTROL));

    printf("\nDisplaying Buffer:\n");

    displayBuffer(&buffer);

    ASSERT_TRUE(1,
                "Display Buffer Executed");
}
/*----------------------------------------------------------
    Run All Buffer Tests
-----------------------------------------------------------*/
void runBufferTests(void)
{
    testBufferInitialization();
    testBufferEmpty();
    testBufferFull();
    testBufferEnqueue();
    testBufferDequeue();
    testBufferPeek();
    testBufferUnderflow();
    testBufferOverflow();
    testFIFOOrder();
    testMultipleOperations();
    testCircularBuffer();
    testBufferCount();
    testDisplayBuffer();
}