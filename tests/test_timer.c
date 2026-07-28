#include "test.h"
#include <unistd.h>

/*----------------------------------------------------------
    Test Timer Start
-----------------------------------------------------------*/
void testTimerStart(void)
{
    Timer timer;

    startTimer(&timer, 5);

    ASSERT_EQUAL(5,
                 timer.timeoutSeconds,
                 "Timer Started");
}

/*----------------------------------------------------------
    Test Timer Timeout
-----------------------------------------------------------*/
void testTimerTimeout(void)
{
    Timer timer;

    startTimer(&timer, 1);

    sleep(2);

    ASSERT_TRUE(hasTimedOut(&timer),
                "Timer Timeout");
}

/*----------------------------------------------------------
    Test Timer Reset
-----------------------------------------------------------*/
void testTimerReset(void)
{
    Timer timer;

    startTimer(&timer, 1);

    sleep(1);

    resetTimer(&timer);

    ASSERT_TRUE(!hasTimedOut(&timer),
                "Timer Reset");
}

/*----------------------------------------------------------
    Test Timer Without Timeout
-----------------------------------------------------------*/
void testTimerNoTimeout(void)
{
    Timer timer;

    startTimer(&timer, 5);

    ASSERT_TRUE(!hasTimedOut(&timer),
                "No Timeout");
}

/*----------------------------------------------------------
    Test Zero Timeout
-----------------------------------------------------------*/
void testZeroTimeout(void)
{
    Timer timer;

    startTimer(&timer, 0);

    ASSERT_TRUE(hasTimedOut(&timer),
                "Zero Timeout");
}

/*----------------------------------------------------------
    Test Timer Display
-----------------------------------------------------------*/
void testDisplayTimer(void)
{
    Timer timer;

    startTimer(&timer, 3);

    printf("\nDisplaying Timer:\n");

    displayTimer(&timer);

    ASSERT_TRUE(1,
                "Display Timer");
}

/*----------------------------------------------------------
    Test Multiple Resets
-----------------------------------------------------------*/
void testMultipleTimerReset(void)
{
    Timer timer;

    startTimer(&timer, 2);

    resetTimer(&timer);
    resetTimer(&timer);
    resetTimer(&timer);

    ASSERT_TRUE(!hasTimedOut(&timer),
                "Multiple Timer Resets");
}

/*----------------------------------------------------------
    Run All Timer Tests
-----------------------------------------------------------*/
void runTimerTests(void)
{
    testTimerStart();
    testTimerTimeout();
    testTimerReset();
    testTimerNoTimeout();
    testZeroTimeout();
    testDisplayTimer();
    testMultipleTimerReset();
}