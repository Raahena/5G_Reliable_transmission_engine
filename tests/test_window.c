#include "test.h"

/*----------------------------------------------------------
    Test Window Initialization
-----------------------------------------------------------*/
void testWindowInitialization(void)
{
    Window window;

    initWindow(&window, 4);

    ASSERT_EQUAL(0, window.base, "Base Initialized");
    ASSERT_EQUAL(0, window.nextSeqNum, "Next Sequence Initialized");
    ASSERT_EQUAL(4, window.windowSize, "Window Size Initialized");
}

/*----------------------------------------------------------
    Test Can Send
-----------------------------------------------------------*/
void testCanSend(void)
{
    Window window;

    initWindow(&window, 4);

    ASSERT_TRUE(canSend(&window),
                "Sender Can Send Initially");

    window.nextSeqNum = 4;

    ASSERT_TRUE(!canSend(&window),
                "Window Full - Cannot Send");
}

/*----------------------------------------------------------
    Test Slide Window
-----------------------------------------------------------*/
void testWindowSlide(void)
{
    Window window;

    initWindow(&window, 4);

    window.nextSeqNum = 4;

    slideWindow(&window, 2);

    ASSERT_EQUAL(3,
                 window.base,
                 "Window Slid to ACK+1");
}

/*----------------------------------------------------------
    Test Outstanding Packets
-----------------------------------------------------------*/
void testOutstandingPackets(void)
{
    Window window;

    initWindow(&window, 4);

    ASSERT_TRUE(!hasOutstandingPackets(&window),
                "No Outstanding Packets");

    window.nextSeqNum = 2;

    ASSERT_TRUE(hasOutstandingPackets(&window),
                "Outstanding Packets Present");
}

/*----------------------------------------------------------
    Test Multiple Window Slides
-----------------------------------------------------------*/
void testMultipleSlides(void)
{
    Window window;

    initWindow(&window, 5);

    window.nextSeqNum = 5;

    slideWindow(&window, 0);
    ASSERT_EQUAL(1, window.base, "Slide 1");

    slideWindow(&window, 2);
    ASSERT_EQUAL(3, window.base, "Slide 2");

    slideWindow(&window, 4);
    ASSERT_EQUAL(5, window.base, "Slide 3");
}

/*----------------------------------------------------------
    Test Invalid ACK
-----------------------------------------------------------*/
void testInvalidACK(void)
{
    Window window;

    initWindow(&window, 4);

    window.base = 3;

    slideWindow(&window, 1);

    ASSERT_EQUAL(3,
                 window.base,
                 "Old ACK Ignored");
}

/*----------------------------------------------------------
    Test Window Limit
-----------------------------------------------------------*/
void testWindowLimit(void)
{
    Window window;

    initWindow(&window, 8);

    ASSERT_EQUAL(8,
                 window.windowSize,
                 "Window Size Stored");
}

/*----------------------------------------------------------
    Test Display Window
-----------------------------------------------------------*/
void testDisplayWindow(void)
{
    Window window;

    initWindow(&window, 4);

    window.nextSeqNum = 2;

    printf("\nDisplaying Window:\n");

    displayWindow(&window, 10);

    ASSERT_TRUE(1,
                "Display Window Executed");
}

/*----------------------------------------------------------
    Run All Window Tests
-----------------------------------------------------------*/
void runWindowTests(void)
{
    testWindowInitialization();
    testCanSend();
    testWindowSlide();
    testOutstandingPackets();
    testMultipleSlides();
    testInvalidACK();
    testWindowLimit();
    testDisplayWindow();
}