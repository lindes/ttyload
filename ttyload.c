/*
 * ttyload
 *
 * tty equivelant to xload
 *
 * Copyright 1996 by David Lindes
 * all right reserved.
 *
 * Version information: $Id: ttyload.c,v 1.3 1996-06-16 02:53:30 lindes Exp $
 *
 */

#include <limits.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/sysmp.h>
#include <sys/types.h>
#include <unistd.h>

#include "ttyload.h"

char *c="$Id: ttyload.c,v 1.3 1996-06-16 02:53:30 lindes Exp $";

char	*kmemfile	= "/dev/kmem";
int	kmemfd;

void	getload(long,long,load_list *);
int	compute_height(load_t,load_t,int);
void	showloads(load_list *);
void	clear_screen();
void	cycle_load_list(load_list*,load_list,int);

int	debug=3;

int main(argc,argv,envp)
    int		argc;
    char	*argv[],
		*envp[];
{
    float	multiplier;
    load_t	loadaddr;
    load_list	*loadavgs, newload;
    int		i,j,k;

    loadavgs	= (load_list *)calloc(width,sizeof(load_list));

    if(!loadavgs)
    {
	perror("calloc for loadavgs failed");
    }

    loadaddr	= sysmp(MP_KERNADDR,MPKA_AVENRUN);

    if(loadaddr == -1)
    {
	perror("Couldn't determine load address");
	exit(1);
    }

    kmemfd	= open(kmemfile,O_RDONLY);

    if(kmemfd < 0)
    {
	perror("Couldn't open memory file");
	exit(1);
    }

    for(i=0;i<width;i++)
    {
	if(i != 0)
	{
	    sleep(intsec);
	}

	getload(kmemfd,loadaddr,&loadavgs[i]);

	if(debug > 3)
	    printf("Load averages: %f, %f, %f\n",
		    loadavgs[i].one_minute / 1024.,
		    loadavgs[i].five_minute / 1024.,
		    loadavgs[i].fifteen_minute / 1024.,
		NULL);

	showloads(loadavgs);

	if(i == (width - 1))
	{
	    if(debug > 4)
	    {
		printf("CYCLING LOAD LIST...\n");
		sleep(3);
	    }
	    getload(kmemfd,loadaddr,&newload);
	    cycle_load_list(loadavgs,newload,width);
	    i--;
	}
    }
}

void	showloads(loadavgs)
    load_list	*loadavgs;
{
    load_list	min	= {LONG_MAX-1,LONG_MAX-1,LONG_MAX-1,0,0,0},
		max	= {0,0,0,0,0,0};
    load_t	omin,omax,
		lmin,lmax;
    int		i,j,k;

    if(debug>3)
    {
	printf("Starting with min set: %d,%d,%d\n",
		min.one_minute,
		min.five_minute,
		min.fifteen_minute,
	    NULL);
	printf("Starting with first set: %d,%d,%d\n",
		loadavgs[0].one_minute,
		loadavgs[0].five_minute,
		loadavgs[0].fifteen_minute,
	    NULL);
	sleep(1);
    }
    for(i=0;i<width;i++)
    {
	if(debug>9)
	{
	    printf("Checking for min/max at %d...\n",i);
	    printf("Comparing, for example, %d <=> %d\n",
		    min.one_minute,loadavgs[i].one_minute,
		NULL);
	}
	min.one_minute	= MIN(min.one_minute,loadavgs[i].one_minute);
	min.five_minute	= MIN(min.five_minute,loadavgs[i].five_minute);
	min.fifteen_minute
		= MIN(min.fifteen_minute,loadavgs[i].fifteen_minute);
	max.one_minute	= MAX(max.one_minute,loadavgs[i].one_minute);
	max.five_minute	= MAX(max.five_minute,loadavgs[i].five_minute);
	max.fifteen_minute
		= MAX(max.fifteen_minute,loadavgs[i].fifteen_minute);
    }
    if(debug>3)
    {
	printf("Continuing with min set: %d,%d,%d\n",
		min.one_minute,
		min.five_minute,
		min.fifteen_minute,
	    NULL);
	printf("Continuing with first set: %d,%d,%d\n",
		loadavgs[0].one_minute,
		loadavgs[0].five_minute,
		loadavgs[0].fifteen_minute,
	    NULL);
	sleep(1);
    }
    printf("MIN Load averages: %f, %f, %f\n",
	    min.one_minute / 1024.,
	    min.five_minute / 1024.,
	    min.fifteen_minute / 1024.,
	NULL);
    printf("MAX Load averages: %f, %f, %f\n",
	    max.one_minute / 1024.,
	    max.five_minute / 1024.,
	    max.fifteen_minute / 1024.,
	NULL);
    lmin=MIN(min.one_minute,MIN(min.five_minute,min.fifteen_minute));
    lmax=MAX(max.one_minute,MAX(max.five_minute,max.fifteen_minute));

    if(debug > 3)
	printf("Overall MIN, MAX: %f, %f\n",lmin/1024.,lmax/1024.);

    omin	= lmin / 1024;
    omax	= (lmax / 1024) + 1;

    lmin	= 1024 * omin;
    lmax	= 1024 * omax;

    if(debug > 3)
    {
	printf("Boundaries: %d, %d...  ",omin,omax);
	printf("Long Boundaries: %d, %d\n",lmin,lmax);
    }


    for(i=0;i<width;i++)
    {
	loadavgs[i].height1 =
	compute_height(loadavgs[i].one_minute,lmax,height);
	loadavgs[i].height5 =
	compute_height(loadavgs[i].five_minute,lmax,height);
	loadavgs[i].height15 =
	compute_height(loadavgs[i].fifteen_minute,lmax,height);

	if(debug > 3)
	{
	    printf("Load averages: %f, %f, %f  -- ",
		    loadavgs[i].one_minute / 1024.,
		    loadavgs[i].five_minute / 1024.,
		    loadavgs[i].fifteen_minute / 1024.,
		NULL);
	    printf("Heights: %d, %d, %d\n",
		    loadavgs[i].height1,
		    loadavgs[i].height5,
		    loadavgs[i].height15,
		NULL);
	}
    }

    clear_screen();
    for(j=0;j<height;j++)
    {
	printf("%6.2f   ",
		( ( ((float)omax) * (height-j)) / (height))
	    );
	for(i=0;i<width;i++)
	{
	    k=0;
	    if(loadavgs[i].height1	== j)
		k+=ONE;
	    if(loadavgs[i].height5	== j)
		k+=FIVE;
	    if(loadavgs[i].height15	== j)
		k+=FIFTEEN;
	    printf("%s",loadstrings[k]);
	}
	printf("\n");
    }

    printf("\n  Key:\n");
    printf(" 1 min: %s, 5 min: %s, 15 min: %s\n"
	   " 1&5 same: %s, 1&15: %s, 5&15: %s, all: %s\n",
	    loadstrings[1],
	    loadstrings[2],
	    loadstrings[4],
	    loadstrings[3],
	    loadstrings[5],
	    loadstrings[6],
	    loadstrings[7],
	NULL);
}

void	getload(kmemfd,loadaddr,loadavgs)
    long	kmemfd,loadaddr;
    load_list	*loadavgs;
{
    if((lseek(kmemfd,loadaddr,SEEK_SET)) != loadaddr)
    {
	perror("Couldn't seek to load address");
	exit(1);
    }

    if((read(kmemfd,loadavgs,3*sizeof(load_t))) != 3*sizeof(load_t))
    {
	perror("Couldn't read load data");
	exit(1);
    }
}

int	compute_height(thisload,maxload,height)
    load_t	thisload,
		maxload;
    int		height;
{
    return(/*height-*/(height*((maxload-thisload)/(float)maxload)));
}

void	clear_screen()
{
    printf("\033[H\033[2J");
}

void	cycle_load_list(loadavgs,newload,width)
    load_list	*loadavgs,
		newload;
    int		width;
{
    /* This function will eventually have code to
       clear locations on the screen that change. */

    int i;

    for(i=0;i<(width-1);i++)
    {
	loadavgs[i]	= loadavgs[i+1];
    }
    loadavgs[i]	= newload;
}
