#define	LOADFILE	"/proc/loadavg"

/* What the contents of the proc file look like, as an example:
 * 0.00 0.00 0.00 1/59 10941
 */

#include "ttyload.h"

#include <stdio.h>		/* for perror */
#include <stdlib.h>		/* for exit() */
#include <unistd.h>		/* for sleep() */

void	getload(load_list *loadavgs)
{
    float	theload[3];
    FILE	*loadfile;
    int		ret;

    /* just for sanity: */
    if(!loadavgs)
    {
    	fprintf(stderr, "getload called without loadavgs pointer.\n");
	exit(2);
    }

    if(!(loadfile = fopen(LOADFILE, "r")))
    {
    	perror("Open of " LOADFILE " failed");
	exit(1);
    }

    ret = fscanf(loadfile, "%f %f %f ", &(theload[0]), &(theload[1]), &(theload[2]));

    /* so the caller _can_ (maybe not _will_) know how we did */
    loadavgs->numloads	= ret;
    loadavgs->one_minute	= theload[0] * 1024;
    loadavgs->five_minute	= theload[1] * 1024;
    loadavgs->fifteen_minute	= theload[2] * 1024;

    /* a little warning thing... should probably be re-written
     * for the Linux/fscanf stuff, this is a hold-over from the
     * solaris stuff.  */
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
