#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdint.h>

typedef struct
{
    time_t   startTime;
    uint16_t timeoutSeconds;
} Timer;

/*
 * Start Timer
 */

void startTimer(Timer *timer, int timeoutSeconds);

/*
 * Check Timeout
 */
int hasTimedOut(Timer *timer);

/*
 * Reset Timer
 */
void resetTimer(Timer *timer);

/*
 * Display Timer Status
 */
void displayTimer(Timer *timer);

#endif
