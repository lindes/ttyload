/*
 * archtest.c -- simple harness to the arch-specific stuff, so
 * that we can test those things in relative isolation.
 */

#include "ttyload.h"

#include <stdio.h>

int rows = -1, cols = -1;

int main(int argc, char *argv[])
{
    load_list	loads;

    gettermsize();
    printf("termsize: %d rows, %d cols\n", rows, cols);

    getload(&loads);

    printf("Load averages %g, %g, %g\n",
	(loads.one_minute / 1024.0),
	(loads.five_minute / 1024.0),
	(loads.fifteen_minute / 1024.0));

    return(0);
}
