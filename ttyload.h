/*
 * ttyload.h
 *
 * support file for ttyload.c
 *
 * Copyright 1996 David Lindes,
 * all rights reserved.
 *
 */

static char *h="$Id: ttyload.h,v 1.4 2000-05-20 01:31:30 lindes Exp $";

#define	MIN(a,b)	(((a)<(b))?(a):(b))
#define	MAX(a,b)	(((a)>(b))?(a):(b))

#define	ONE	01;
#define	FIVE	02;
#define	FIFTEEN	04;

typedef	long	load_t;

typedef struct load_list {
	load_t	one_minute;
	load_t	five_minute;
	load_t	fifteen_minute;
	int	height1;
	int	height5;
	int	height15;
	int	numloads;
    } load_list;

typedef struct clock_info {
	int	pos;
	char	clock[6];
    } clock_info;
