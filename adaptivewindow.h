#ifndef ADAPTIVEWINDOW_H
#define ADAPTIVEWINDOW_H

#include "window.h"
#include <stdint.h>

#define MAX_WINDOW_SIZE 10
#define MIN_WINDOW_SIZE 1

/*
 * Increase Window Size
 */
void increaseWindow(Window *window);

/*
 * Decrease Window Size
 */
void decreaseWindow(Window *window);

/*
 * Adaptive Adjustment
 */
void adjustWindow(Window *window, int packetLoss);

/*
 * Display Window Status
 */
void displayAdaptiveWindow(const Window *window);

#endif
