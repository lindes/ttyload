/*
 * arch/Linux/terminfo.c -- routines for getting terminal
 * information on Linux machines.
 *
 * Copyright 2001, David Lindes.  All rights reserved.
 */

#include <sys/ioctl.h>	/* for ioctl() */

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
