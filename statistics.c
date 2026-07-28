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
    "Packets Sent           : %u\n",
    stats->sent);

    printf(
    "Packets Received       : %u\n",
    stats->received);

    printf(
    "Packets Lost           : %u\n",
    stats->lost);

    printf(
    "Packets Retransmitted  : %u\n",
    stats->retransmitted);

    printf(
    "=====================================\n");
}
