#include "ttyload.h"

#include <stdio.h>		/* for perror */
#include <stdlib.h>		/* for exit() */
#include <kstat.h>		/* for the various kstat stuff */

void	getload(load_list *loadavgs)
{
    static kstat_ctl_t		*kc;	/* kstat control */
    static kstat_t		*ksrec;	/* the kstat record */
    kstat_named_t		*info;	/* our pointer to the info */

    if(!kc)	/* initialize, first time only */
    {
    	if(!(kc = kstat_open()))
	{
	    perror("kstat_open failed");
	    exit(1);
	}

	/* find the record where the load averages are stored
	 * (It seems like it's safe to only do this once) */
	if(!(ksrec = kstat_lookup(kc, "unix", 0, "system_misc")))
	{
	    perror("ksatat_lookup failed");
	    exit(1);
	}
    }

    /* read in the latest data into the buffer space that ksrec
     * keeps for us. */
    if(kstat_read(kc, ksrec, NULL) == -1)
    {
    	perror("kstat_read failed");
	exit(1);
    }

    /* need a pointer with a type we can dig into: */
    for(info = (kstat_named_t*)ksrec->ks_data, loadavgs->numloads = 0;
	(char*)info < ((char*)ksrec->ks_data + ksrec->ks_data_size); info++)
    {
	/* DEBUG: printf("info->name = %s\n", info->name); */

	/* in theory, the loop will take us through all three of
	 * these, in order...  I depend on that being true: */
	if(strcmp(info->name, "avenrun_1min") == 0)
	{
	    /* Note: Sun stores this data as the float value *
	     * 256; we use 1024 as the multiplier, so multiply
	     * by 4 to make up the difference: */
	    loadavgs->one_minute	= info->value.ul * 4;
	    loadavgs->numloads++;
	}
	else if(strcmp(info->name, "avenrun_5min") == 0)
	{
	    loadavgs->five_minute	= info->value.ul * 4;
	    loadavgs->numloads++;
	}
	else if(strcmp(info->name, "avenrun_15min") == 0)
	{
	    loadavgs->fifteen_minute	= info->value.ul * 4;
	    loadavgs->numloads++;
	}
    }

    /* hopefully, this never happens: */
    if(loadavgs->numloads != 3)
    {
    	fprintf(stderr, "Woah, kstat walking didn't get us the "
	    "number of load average data points we were expecting; "
	    "got %d instead of 3.\n", loadavgs->numloads);
	exit(1);
    }
}
