#ifndef STATISTICS_H
#define STATISTICS_H
#include <stdint.h>
typedef struct
{
    uint32_t sent;

    uint32_t received;

   uint32_t lost;

    uint32_t retransmitted;

} Statistics;

/*
 * Initialize Statistics
 */
void initStatistics(
        Statistics *stats);

/*
 * Display Statistics
 */
void printStatistics(
        Statistics *stats);

#endif
