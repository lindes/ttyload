/*
 * ttyload.h
 *
 * support file for ttyload.c
 *
 * Copyright 1996 David Lindes,
 * all rights reserved.
 *
 */

#define	TTYLOAD_H_IDENT	"$Id: ttyload.h,v 1.6 2001-08-28 01:00:31 lindes Exp $";

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

/* functions in arch-specific files: */
extern	void	getload(load_list *);
extern	void	gettermsize();
