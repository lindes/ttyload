/*
 * arch/SunOS/terminfo.c -- routines for getting terminal
 * information on Solaris (SunOS) machines.
 *
 * Copyright 2001, David Lindes.  All rights reserved.
 */

#include <unistd.h>		/* for ioctl() */
#include <termios.h>		/* for TIOCGWINSZ */
#define _KMEMUSER		/* something needs this in order
				 * for strsubr.h to be happy */
#include <sys/systm.h>		/* sys/strsubr.h needs this */
#include <sys/poll.h>		/* sys/strsubr.h needs this */
#include <sys/strsubr.h>	/* sys/ptem.h needs this */
#include <sys/ptem.h>		/* for struct winsize */

/* globals */
extern	int rows;
extern	int cols;

void gettermsize()
{
    struct winsize info;

    /* try to get data via IOCTL: */
    if (ioctl(1 /* stdout */, TIOCGWINSZ, &info) != -1)
    {
	/* if successful, and the data seems sane, set the
	 * program's globals: */
	if(info.ws_col > 0)
	    cols = info.ws_col;
	if(info.ws_row > 0)
	    rows = info.ws_row;
    }
}
