#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "timer.h"

/*
 * Start Timer
 */
void startTimer(Timer *timer, int timeoutSeconds)
{
    timer->startTime      = time(NULL);
    timer->timeoutSeconds = timeoutSeconds;
}

/*
 * Check Timeout
 */
int hasTimedOut(Timer *timer)
{
    time_t currentTime = time(NULL);
    double elapsed     = difftime(currentTime, timer->startTime);

    if (elapsed >= timer->timeoutSeconds)
        return 1;

    return 0;
}

/*
 * Reset Timer
 */
void resetTimer(Timer *timer)
{
    timer->startTime = time(NULL);
}

/*
 * Display Timer Information
 */
void displayTimer(Timer *timer)
{
    time_t currentTime = time(NULL);
    double elapsed     = difftime(currentTime, timer->startTime);

    printf("\n===== TIMER STATUS =====\n");
    printf("Elapsed Time : %.0f sec\n", elapsed);
    printf("Timeout      : %d sec\n", timer->timeoutSeconds);
    printf("========================\n");
}
