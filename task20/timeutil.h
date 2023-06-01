/* Copyright (C) 2003 IP Infusion, Inc. All Rights Reserved. */

#ifndef _ZEBOS_TIMEUTIL_H
#define _ZEBOS_TIMEUTIL_H

/* added code */
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ONE_WEEK_SECOND     (60*60*24*7)
#define ONE_DAY_SECOND      (60*60*24)
#define ONE_HOUR_SECOND     (60*60)
#define ONE_MIN_SECOND      (60)
#define ONE_SEC_DECISECOND  (10)
#define ONE_SEC_CENTISECOND (100)
#define ONE_SEC_MILLISECOND (1000)
#define ONE_SEC_MICROSECOND (1000000)

#define ONE_SEC_TENTHS_OF_SECOND  (10)

#define TV_ADJUST(A)        timeval_adjust (A)
#define TV_CEIL(A)          timeval_ceil ((A))
#define TV_FLOOR(A)         timeval_floor ((A))
#define TV_ADD(A,B)         timeval_add ((A), (B))
#define TV_SUB(A,B)         timeval_sub ((A), (B))
#define TV_CMP(A,B)         timeval_cmp ((A), (B))
#define INT2TV(A)           timeutil_int2tv ((A))

/* added code */
#define TV_USEC_PER_SEC ONE_SEC_MICROSECOND 
#define TIME_MAX_TV_SEC ONE_WEEK_SECOND
#define TIME_MAX_TV_USEC ONE_SEC_MICROSECOND


/* Time related utility function prototypes  */

char *
timeutil_uptime (char *, u_int32_t, time_t);
char *
timeval_uptime (char *, u_int32_t, struct timeval);
struct timeval
timeval_adjust (struct timeval);
struct timeval
timeval_subtract (struct timeval, struct timeval);
int
timeval_ceil (struct timeval);
int
timeval_floor (struct timeval);
struct timeval
timeval_add (struct timeval, struct timeval);
struct timeval
timeval_sub (struct timeval, struct timeval);
int
timeval_cmp (struct timeval, struct timeval);
struct timeval
timeutil_int2tv (u_int32_t);
#endif /* _ZEBOS_TIMEUTIL_H */
