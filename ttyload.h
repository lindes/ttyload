/*
 * ttyload.h
 *
 * support file for ttyload.c
 *
 * Copyright 1996 David Lindes,
 * all rights reserved.
 *
 */

#ident "$Id: ttyload.h,v 1.1 1996-06-16 02:49:29 lindes Exp $"

#define	MIN(a,b)	(((a)<(b))?(a):(b))
#define	MAX(a,b)	(((a)>(b))?(a):(b))

#define	ONE	01;
#define	FIVE	02;
#define	FIFTEEN	04;

char *loadstrings[] = {
	" ",	/* blank */
	"\033[31m*\033[m",	/* one minute average */
	"\033[32m*\033[m",	/* five minute average */
	"\033[33m*\033[m",	/* one & five, together */
	"\033[34m*\033[m",	/* fifteen minute average */
	"\033[35m*\033[m",	/* one & fifteen, together */
	"\033[36m*\033[m",	/* five & fifteen, together */
	"\033[37m*\033[m"	/* one, five & fifteen, together */
    };

typedef	long	load_t;

typedef struct load_list {
	load_t	one_minute;
	load_t	five_minute;
	load_t	fifteen_minute;
	int	height1;
	int	height5;
	int	height15;
    } load_list;

/* The following variables should probably be assigned using
   some sort of real logic, rather than these hard-coded
   defaults, but the defaults work for now... */
int	rows	= 40,
	cols	= 80,
	height	= 35,
	width	= 70,
	intsec	= 1,
	i,j,k;


