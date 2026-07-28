#include <stdio.h>
#include <stdint.h>
#include "window.h"

/*
 * Initialize Window
 */
void initWindow(Window *window, int windowSize)
{
    window->base       = 0;
    window->nextSeqNum = 0;
    window->windowSize = windowSize;
}

/*
 * Can sender send more packets?
 */
int canSend(Window *window)
{
    return (window->nextSeqNum < (window->base + window->windowSize));
}

/*
 * Slide window using cumulative ACK
 */
void slideWindow(Window *window, int ackNo)
{
    if (ackNo >= window->base)
    {
        window->base = ackNo + 1;
    }

    printf("\n[WINDOW] Slid Forward\n");
    printf("Base       = %d\n", window->base);
    printf("NextSeqNum = %d\n", window->nextSeqNum);
}

/*
 * Display Window
 */
void displayWindow(Window *window, int totalPackets)
{
    int end = window->base + window->windowSize - 1;

    if (end >= totalPackets)
    {
        end = totalPackets - 1;
    }

    printf("\n==============================\n");
    printf("Current Window:\n[");
    for (int i = window->base; i <= end; i++)
    {
        printf("%d", i);
    }
    printf("]\n");

    printf("Base        : %d\n", window->base);
    printf("NextSeqNum  : %d\n", window->nextSeqNum);
    printf("Window Size : %d\n", window->windowSize);
    printf("==============================\n");
}

/*
 * Check if outstanding packets exist
 *
 * base != nextSeqNum means some packets are still waiting for ACK
 */
int hasOutstandingPackets(Window *window)
{
    return (window->base != window->nextSeqNum);
}
