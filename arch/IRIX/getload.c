/*
 * ttyload/arch/IRIX/getload.c -- getload() implimentation for the
 * IRIX platform.
 *
 * Copyright 1996-2001, David Lindes.  All rights reserved.
 */

#include "ttyload.h"

#include <sys/types.h>	/* sys/syget.h needs this */
#include <sys/sysget.h>	/* for sysget() */

void	getload(load_list *loadavgs)
{
    sgt_cookie_t cookie;

    SGT_COOKIE_INIT(&cookie);
    SGT_COOKIE_SET_KSYM(&cookie, "avenrun");

    /* since IRIX uses an integer data type for this stuff, which is
     * 1024 times the floating-point equiv, we don't need to do any
     * conversions, because that's what load_list uses (since ttyload
     * was originally written for IRIX in the first place). */
    if(sysget(SGT_KSYM, (char*)loadavgs, 3 * sizeof(load_t),
	    SGT_READ | SGT_SUM, &cookie) != (3 * sizeof(load_t)))
    {
	perror("Couldn't read load data, sysget failed");
	exit(1);
    }
}
