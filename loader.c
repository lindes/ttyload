/*
 * loader.c -- a time-limited, max-load-limited load bomb
 *
 * In general, you don't want to run this.
 *
 * Copyright 2001, David Lindes, All Rights Reserved.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int		ttl,	/* how long to load things */
		forks;	/* how many jobs to run at once */
    time_t	start;	/* when we started */
    pid_t	pid;	/* return value for fork() */

    /* for now, set these purely by hand... later, I'll
     * make 'em be based on argv. */

    ttl		= 90;
    forks	= 11;	/* this one goes to 11.  ;-) */

    for(/*pre-initted*/; --forks > 0 ;)
    {
	if((pid = fork()) != 0)
	{
	    /* check to see if the fork actually failed: */
	    if(pid < 0)
	    {
	    	perror("fork failed");
		exit(1);
	    }

	    /* otherwise, we're a child... */
	    break;	/* don't continue this loop as child. */
	}
    }

    printf("Bomb running for %d seconds\n", ttl);

    start	= time(NULL);

    while(time(NULL) < (start + ttl))
    {
	;	/* do nothing, tight loop is intentional. */
    }

    printf("Bomb ending at %ld, started %ld\n", time(NULL), start);

    exit(0);
}
