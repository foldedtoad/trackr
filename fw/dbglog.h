/*----------------------------------------------------------------------------*/
/*  dbglog.h   for debug log output                                           */
/*----------------------------------------------------------------------------*/
#ifndef DBGLOG_H
#define DBGLOG_H

#if defined(DBGLOG_SUPPORT)

#include <stdio.h>

#define PRINTF   printf
#define PUTS     puts

void dump_bytes(uint8_t * Buffer, int Length);

#else /* DBGLOG_SUPPORT */

#define PRINTF(...)
#define PUTS(s)

#define dump_bytes(a, b)

#endif /* DBGLOG_SUPPORT */

#endif  /* DBGLOG_H */
