#include "ttyload.h"

#include <stdio.h>		/* for perror */
#include <stdlib.h>		/* for exit() */
#include <unistd.h>		/* for sleep() */

#ifdef NEED_LOCAL_HEADERS
#include "loadavg.h"
#else	/* don't NEED_LOCAL_HEADERS */
#include <sys/loadavg.h>	/* for getloadavg() */
#endif	/* (don't) NEED_LOCAL_HEADERS */

void	getload(load_list *loadavgs)
{
    double	theload[3];
    int		ret;

    if((ret = getloadavg(theload, 3)) < 0)
    {
    	perror("getloadavg() failed");
	exit(1);
    }

    switch(ret)
    {
	case 1:
	    loadavgs->one_minute	= theload[0];
	    fprintf(stderr, "5 minute Load average is unreliable.\n");
	    sleep(1);
	    /* we warned, but fall through anyway */
	case 2:
	    loadavgs->five_minute	= theload[1];
	    fprintf(stderr, "15 minute Load average is unreliable.\n");
	    sleep(1);
	    /* we warned, but fall through anyway */
	case 3:
	    /* so the caller _can_ know how we did */
	    loadavgs->numloads	= ret;
	    loadavgs->fifteen_minute	= theload[2];
	    break;
	default:
	    fprintf(stderr, "Sorry, couldn't get any load "
	    	"averages.  This is, therefore,  pointless.\n");
	    exit(1);
    }
}
