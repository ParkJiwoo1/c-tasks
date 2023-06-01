/* Copyright (C) 2003 IP Infusion, Inc. All Rights Reserved. */


#include "timeutil.h"

/* added code.. tamp */
int
time_gmt (time_t *tp, struct tm *gmt)
{
	struct tm *tmp;

	tmp = gmtime (tp);
	if (tmp)
	{
		memcpy (gmt, tmp, sizeof (struct tm));
	}

	return (tmp ? 0 : EAGAIN);
}

/* Uptime string.  */
char *
timeutil_uptime (char *timebuf,
		u_int32_t bufsiz,
		time_t uptime)
{
	struct tm tm;

	/* Get current time. */
	time_gmt (&uptime, &tm);

	/* Making formatted timer string. */
	if (uptime < ONE_DAY_SECOND)
		snprintf (timebuf, bufsiz, "%02d:%02d:%02d",
				tm.tm_hour, tm.tm_min, tm.tm_sec);
	else if (uptime < ONE_WEEK_SECOND)
		snprintf (timebuf, bufsiz, "%02dd%02dh%02dm",
				tm.tm_yday, tm.tm_hour, tm.tm_min);
	else
		snprintf (timebuf, bufsiz, "%02dw%02dd%02dh",
				tm.tm_yday/7, tm.tm_yday - ((tm.tm_yday/7) * 7), tm.tm_hour);

	return timebuf;
}

/* Uptime string.  */
/* convert to day time and print */
char *
timeval_uptime (char *timebuf,
		u_int32_t bufsiz,
		struct timeval uptime)
{
	/* Making formatted timer string. */
	if (uptime.tv_sec < ONE_DAY_SECOND)
		snprintf (timebuf, bufsiz, "%02ld:%02ld:%02ld",
				uptime.tv_sec / ONE_HOUR_SECOND,
				(uptime.tv_sec % ONE_HOUR_SECOND) / ONE_MIN_SECOND,
				uptime.tv_sec % ONE_MIN_SECOND);
	else if (uptime.tv_sec < ONE_WEEK_SECOND)
		snprintf (timebuf, bufsiz, "%02ldd%02ldh%02ldm",
				uptime.tv_sec / ONE_DAY_SECOND,
				(uptime.tv_sec % ONE_DAY_SECOND) / ONE_HOUR_SECOND,
				(uptime.tv_sec % ONE_HOUR_SECOND) / ONE_MIN_SECOND);
	else
		snprintf (timebuf, bufsiz, "%02ldw%02ldd%02ldh",
				uptime.tv_sec / ONE_WEEK_SECOND,
				(uptime.tv_sec % ONE_WEEK_SECOND) / ONE_DAY_SECOND,
				(uptime.tv_sec % ONE_DAY_SECOND) / ONE_HOUR_SECOND);

	return timebuf;
}

struct timeval
timeval_adjust (struct timeval a)
{
	while (a.tv_usec >= TV_USEC_PER_SEC)
	{
		a.tv_usec -= TV_USEC_PER_SEC;
		a.tv_sec++;
	}

	while (a.tv_usec < 0)
	{
		a.tv_usec += TV_USEC_PER_SEC;
		a.tv_sec--;
	}

	if (a.tv_sec < 0)
	{
		a.tv_sec = 0;
		a.tv_usec = 10;
	}

	if (a.tv_sec > TV_USEC_PER_SEC) /* limit sec?? */
		a.tv_sec = TV_USEC_PER_SEC;

	return a;
}

struct timeval
timeval_subtract (struct timeval a, struct timeval b)
{
	struct timeval ret;

	ret.tv_usec = a.tv_usec - b.tv_usec;
	ret.tv_sec = a.tv_sec - b.tv_sec;

	return timeval_adjust (ret);
}

int
timeval_ceil (struct timeval a)
{
	a = timeval_adjust (a);

	return (a.tv_usec ? a.tv_sec + 1 : a.tv_sec); /* only sec(convert) */
}

int
timeval_floor (struct timeval a)
{
	a = timeval_adjust (a);

	return a.tv_sec; /* only sec */
}

struct timeval
timeval_add (struct timeval a, struct timeval b)
{
	struct timeval ret;

	ret.tv_sec = a.tv_sec + b.tv_sec; /* add */
	ret.tv_usec = a.tv_usec + b.tv_usec;

	return timeval_adjust (ret);
}

struct timeval
timeval_sub (struct timeval a, struct timeval b)
{
	struct timeval ret;

	ret.tv_sec = a.tv_sec - b.tv_sec; /* subtract */
	ret.tv_usec = a.tv_usec - b.tv_usec;

	return timeval_adjust (ret);
}

/**
 * compare two timeval
 * 
 * @param[in] a. first timeval
 * @param[in] b. second timeval
 * 
 * @return same value(0), a is larger than b (positive value) else (negative)
 */
int
timeval_cmp (struct timeval a, struct timeval b)
{
	return (a.tv_sec == b.tv_sec ?
			a.tv_usec - b.tv_usec : a.tv_sec - b.tv_sec);
}

/**
 * convert int value to timeval
 * 
 * @parma[in] a. int value will be converted
 *
 * @return converted timeval
 */
struct timeval
timeutil_int2tv (u_int32_t a)
{
	struct timeval ret;

	ret.tv_sec = a; /* second */
	ret.tv_usec = 0; /* micro second */

	return ret;
}
