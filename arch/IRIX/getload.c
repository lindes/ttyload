/*
 * ttyload/arch/IRIX/getload.c -- getload() implimentation for the
 * IRIX platform.
 *
 * Copyright 1996-2001, David Lindes.  All rights reserved.
 */

#include "ttyload.h"

#include <sys/fcntl.h>
#include <sys/sysmp.h>	/* for sysmp() */
#include <sys/types.h>	/* for lseek() */
#include <unistd.h>	/* for lseek() */

// where's /dev/kmem?  (just in case this ever changed)
#define	KMEMFILE	"/dev/kmem"

void	getload(load_list *loadavgs)
{
    static int		kmemfd = -1;
    static load_t	loadaddr;

    if(kmemfd < 0)	/* initialize, first-time only: */
    {
	/* first, figure out where in /dev/kmem space the load average
	 * data gets stored... */
	loadaddr	= sysmp(MP_KERNADDR, MPKA_AVENRUN);

	/* and make sure that worked: */
	if(loadaddr == -1)
	{
	    perror("Couldn't determine load address");
	    exit(1);
	}

	/* then, if that worked, try to open kmem: */
	kmemfd	= open(KMEMFILE, O_RDONLY);

	/* and verify: */
	if(kmemfd < 0)
	{
	    perror("Couldn't open " KMEMFILE);
	    exit(1);
	}
    }

    /* assuming the above went well, we now pull out the load average
     * data from kmem via a seek/read pair: */

    /* first seek: */
    if((lseek(kmemfd, loadaddr, SEEK_SET)) != loadaddr)
    {
	perror("Couldn't seek to load address");
	exit(1);
    }

    /* then read: */
    if((read(kmemfd,loadavgs,3*sizeof(load_t))) != 3*sizeof(load_t))
    {
	perror("Couldn't read load data");
	exit(1);
    }

    /* and since IRIX uses an integer data type for this stuff, which is
     * 1024 times the floating-point equiv, we don't need to do any
     * conversions, because that's what load_list uses (since ttyload
     * was originally written for IRIX in the first place). */
}
