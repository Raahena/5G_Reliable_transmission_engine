#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

typedef struct
{
    uint16_t base;
    uint16_t nextSeqNum;
    uint16_t windowSize;
} Window;

/*
 * Initialize Window
 */
void initWindow(Window *window, int windowSize);

/*
 * Check if sender can send
 */
int canSend(Window *window);

/*
 * Slide window after ACK
 */
void slideWindow(Window *window, int ackNo);

/*
 * Display current window
 */
void displayWindow(Window *window, int totalPackets);

/*
 * Check outstanding packets
 */
int hasOutstandingPackets(Window *window);

#endif
