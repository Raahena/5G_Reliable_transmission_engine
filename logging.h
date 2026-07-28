#ifndef LOGGING_H
#define LOGGING_H
#include <stdint.h>
/*
 * Initialize log file
 */
void initializeLog(void);

/*
 * Log event
 */
void logEvent(const char *message);

/*
 * Log packet event
 */
void logPacketEvent(
 const char *event,
 int seqNo);

/*
 * Close log file
 */
void closeLog(void);

#endif
