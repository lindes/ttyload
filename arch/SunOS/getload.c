#include "ttyload.h"

#include <stdio.h>		/* for perror */
#include <stdlib.h>		/* for exit() */
#include <unistd.h>		/* for sleep() */

#include <sys/loadavg.h>	/* for getloadavg() */

void	getload(load_list *loadavgs)
{
    double	theload[3];
    int		ret;

    if((ret = getloadavg(theload, 3)) < 0)
    {
    	perror("getloadavg() failed");
	exit(1);
    }

    /* so the caller _can_ (maybe not _will_) know how we did */
    loadavgs->numloads	= ret;
    loadavgs->one_minute	= theload[0] * 1024;
    loadavgs->five_minute	= theload[1] * 1024;
    loadavgs->fifteen_minute	= theload[2] * 1024;

    switch(ret)
    {
	case 2:
	    fprintf(stderr, "15 minute Load average is unreliable.\n");
	    sleep(5);
	    break;
	case 1:
	    fprintf(stderr, "5 and 15 minute load averages are unreliable.\n");
	    sleep(5);
	    break;
	case 0:
	    fprintf(stderr, "Sorry, couldn't get any load "
	    	"averages.  This is, therefore,  pointless.\n");
	    exit(1);
    }
}
