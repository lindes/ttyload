/* some function replacements for platforms that don't have the
 * real things...  *sigh* */

#include <string.h>
#include <time.h>
#include <stdio.h>


/**********************/
/***** ascftime() *****/
/**********************/

#ifndef	HAVE_ASCFTIME

/* our own version of ascftime, since Linux (and others?)
 * doesn't (don't) seem to have it.  For now, this is done in a
 * very klugey way, and it will certainly need to be re-written
 * (to be useful, anyway) if and when there ever become new
 * format strings used by our caller(s). */

int ascftime(char *s, const char *format, const struct tm *timeptr)
{
    int	ret;

    /* if we don't have arguments, bail now. */
    if(!s || !format || !timeptr)
    {
    	return(0);
    }

    *s	= '\0';

    if(strncmp(format, "^%H:%M", 6) == 0)
    {
	ret = sprintf(s, "^%02d:%02d",
	    timeptr->tm_hour, timeptr->tm_min);
    }
    else if(strncmp(format, "%T", 2) == 0)
    {
	ret = sprintf(s, "^%02d:%02d:%02d",
	    timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);
    }
    else
    {
	/* indicate failure, since we don't have generic
	 * compliance for any old format: */
    	ret	= 0;
    }

    return(ret);
}

#endif	/* HAVE_ASCFTIME */
