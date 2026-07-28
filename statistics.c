#include <stdio.h>
#include <stdint.h>
#include "statistics.h"

/*
 * Initialize Statistics
 */
void initStatistics(
        Statistics *stats)
{
    stats->sent = 0;

    stats->received = 0;

    stats->lost = 0;

    stats->retransmitted = 0;
}

/*
 * Print Final Report
 */
void printStatistics(
        Statistics *stats)
{
    printf(
    "\n=====================================\n");

    printf(
    "      TRANSMISSION REPORT\n");

    printf(
    "=====================================\n");

    printf(
    "Packets Sent           : %d\n",
    stats->sent);

    printf(
    "Packets Received       : %d\n",
    stats->received);

    printf(
    "Packets Lost           : %d\n",
    stats->lost);

    printf(
    "Packets Retransmitted  : %d\n",
    stats->retransmitted);

    printf(
    "=====================================\n");
}
