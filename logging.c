#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "logging.h"

static FILE *logFile = NULL;

/*
 * Create/Open log file
 */
void initializeLog(void)
{
    logFile = fopen("transmission.log", "w");

    if (logFile == NULL)
    {
        printf("[LOG] Failed to create log file\n");
        return;
    }

    fprintf(logFile, "===== Transmission Log Started =====\n");
    fflush(logFile);
}

/*
 * General Event Logging
 */
void logEvent(const char *message)
{
    if (logFile == NULL)
        return;

    time_t now = time(NULL);

    fprintf(logFile, "[%ld] %s\n", now, message);
    fflush(logFile);
}

/*
 * Packet Event Logging
 */
void logPacketEvent(const char *event, int seqNo)
{
    if (logFile == NULL)
        return;

    time_t now = time(NULL);

    fprintf(logFile, "[%ld] %s Packet %d\n", now, event, seqNo);
    fflush(logFile);
}

/*
 * Close log file
 */
void closeLog(void)
{
    if (logFile != NULL)
    {
        fprintf(logFile, "===== Transmission Ended =====\n");
        fclose(logFile);
        logFile = NULL;
    }
}
