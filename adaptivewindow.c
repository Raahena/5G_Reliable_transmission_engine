#include <stdio.h>
#include "adaptivewindow.h"

/*
 * Increase Window
 */
void increaseWindow(Window *window)
{
    if (window->windowSize < MAX_WINDOW_SIZE)
    {
        window->windowSize++;
        printf("[ADAPTIVE] Window Increased -> %d\n", window->windowSize);
    }
}

/*
 * Decrease Window
 */
void decreaseWindow(Window *window)
{
    if (window->windowSize > MIN_WINDOW_SIZE)
    {
        window->windowSize--;
        printf("[ADAPTIVE] Window Reduced -> %d\n", window->windowSize);
    }
}

/*
 * Adaptive Control
 */
void adjustWindow(Window *window, int packetLoss)
{
    if (packetLoss)
    {
        decreaseWindow(window);
    }
    else
    {
        increaseWindow(window);
    }
}

/*
 * Display Status
 */
void displayAdaptiveWindow(Window *window)
{
    printf("\n===== ADAPTIVE WINDOW =====\n");
    printf("Current Window Size : %d\n", window->windowSize);
    printf("Max Window Size     : %d\n", MAX_WINDOW_SIZE);
    printf("Min Window Size     : %d\n", MIN_WINDOW_SIZE);
    printf("===========================\n");
}
