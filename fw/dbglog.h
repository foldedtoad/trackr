/*----------------------------------------------------------------------------*/
/*  dbglog.h   for debug log output                                           */
/*----------------------------------------------------------------------------*/
#ifndef DBGLOG_H
#define DBGLOG_H

#if defined(DBGLOG_SUPPORT)

#include <stdio.h>

#define PRINTF   printf
#define PUTS     puts

#else /* DBGLOG_SUPPORT */

#define PRINTF(...)
#define PUTS(s)

#endif /* DBGLOG_SUPPORT */

#define TRACE PRINTF("at: %s(%d)\n", __FUNCTION__, __LINE__)

#endif  /* DBGLOG_H */
