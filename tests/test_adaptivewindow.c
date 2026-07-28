#include "test.h"

/*----------------------------------------------------------
    Test Increase Window
-----------------------------------------------------------*/
void testIncreaseWindow(void)
{
    Window window;

    initWindow(&window, 5);

    increaseWindow(&window);

    ASSERT_EQUAL(6,
                 window.windowSize,
                 "Window Increased");
}

/*----------------------------------------------------------
    Test Decrease Window
-----------------------------------------------------------*/
void testDecreaseWindow(void)
{
    Window window;

    initWindow(&window, 5);

    decreaseWindow(&window);

    ASSERT_EQUAL(4,
                 window.windowSize,
                 "Window Decreased");
}

/*----------------------------------------------------------
    Test Increase Beyond Maximum
-----------------------------------------------------------*/
void testIncreaseToMaximum(void)
{
    Window window;

    initWindow(&window, MAX_WINDOW_SIZE);

    increaseWindow(&window);

    ASSERT_EQUAL(MAX_WINDOW_SIZE,
                 window.windowSize,
                 "Maximum Window Size Maintained");
}

/*----------------------------------------------------------
    Test Decrease Below Minimum
-----------------------------------------------------------*/
void testDecreaseToMinimum(void)
{
    Window window;

    initWindow(&window, MIN_WINDOW_SIZE);

    decreaseWindow(&window);

    ASSERT_EQUAL(MIN_WINDOW_SIZE,
                 window.windowSize,
                 "Minimum Window Size Maintained");
}

/*----------------------------------------------------------
    Test Adaptive Increase
-----------------------------------------------------------*/
void testAdaptiveIncrease(void)
{
    Window window;

    initWindow(&window, 5);

    adjustWindow(&window, 0);

    ASSERT_EQUAL(6,
                 window.windowSize,
                 "Adaptive Increase");
}

/*----------------------------------------------------------
    Test Adaptive Decrease
-----------------------------------------------------------*/
void testAdaptiveDecrease(void)
{
    Window window;

    initWindow(&window, 5);

    adjustWindow(&window, 1);

    ASSERT_EQUAL(4,
                 window.windowSize,
                 "Adaptive Decrease");
}

/*----------------------------------------------------------
    Test Display Adaptive Window
-----------------------------------------------------------*/
void testDisplayAdaptiveWindow(void)
{
    Window window;

    initWindow(&window, 5);

    displayAdaptiveWindow(&window);

    ASSERT_TRUE(1,
                "Display Adaptive Window");
}

/*----------------------------------------------------------
    Run All Adaptive Window Tests
-----------------------------------------------------------*/
void runAdaptiveWindowTests(void)
{
    testIncreaseWindow();
    testDecreaseWindow();
    testIncreaseToMaximum();
    testDecreaseToMinimum();
    testAdaptiveIncrease();
    testAdaptiveDecrease();
    testDisplayAdaptiveWindow();
}