#include <limits.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/sysmp.h>
#include <sys/types.h>
#include <unistd.h>

#define	MIN(a,b)	((a)<(b)?(a):(b))
#define	MAX(a,b)	((a)>(b)?(a):(b))

#define	ONE	01;
#define	FIVE	02;
#define	FIFTEEN	04;

char *loadstrings[] = { " ", "+", "x", "*", ".", "=", "X", "@" };

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


char	*kmemfile	= "/dev/kmem";

void	getload(long,long,load_list *);
int	compute_height(load_t,load_t,int);

int main(argc,argv,envp)
    int		argc;
    char	*argv[],
		*envp[];
{
    int		kmemfd,
		omin,omax,
		lmin,lmax;
    float	multiplier;
    load_t	loadaddr;
    load_list	min	= {LONG_MAX,LONG_MAX,LONG_MAX},
		max	= {0,0,0},
		*loadavgs;

    loadavgs	= (load_list *)calloc(width,sizeof(load_list));

    if(!loadavgs)
    {
	perror("malloc for loadavgs failed");
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

	/*
	printf("Load averages: %f, %f, %f\n",
		loadavgs[i].one_minute / 1024.,
		loadavgs[i].five_minute / 1024.,
		loadavgs[i].fifteen_minute / 1024.,
	    NULL);
	*/
    }
    for(i=0;i<width;i++)
    {
	min.one_minute	= MIN(min.one_minute,loadavgs[i].one_minute);
	min.five_minute	= MIN(min.five_minute,loadavgs[i].five_minute);
	min.fifteen_minute
		= MIN(min.fifteen_minute,loadavgs[i].fifteen_minute);
	max.one_minute	= MAX(max.one_minute,loadavgs[i].one_minute);
	max.five_minute	= MAX(max.five_minute,loadavgs[i].five_minute);
	max.fifteen_minute
		= MAX(max.fifteen_minute,loadavgs[i].fifteen_minute);
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

    printf("Overall MIN, MAX: %f, %f\n",lmin/1024.,lmax/1024.);

    omin	= lmin / 1024;
    omax	= (lmax / 1024) + 1;

    lmin	= 1024 * omin;
    lmax	= 1024 * omax;

    printf("Boundaries: %d, %d\n",omin,omax);
    printf("Long Boundaries: %d, %d\n",lmin,lmax);


    for(i=0;i<width;i++)
    {
	printf("Load averages: %f, %f, %f  -- ",
		loadavgs[i].one_minute / 1024.,
		loadavgs[i].five_minute / 1024.,
		loadavgs[i].fifteen_minute / 1024.,
	    NULL);
	printf("Heights: %d, %d, %d\n",
		loadavgs[i].height1 =
		compute_height(loadavgs[i].one_minute,lmax,height),
		loadavgs[i].height5 =
		compute_height(loadavgs[i].five_minute,lmax,height),
		loadavgs[i].height15 =
		compute_height(loadavgs[i].fifteen_minute,lmax,height),
	    NULL);
    }

    for(j=0;j<height;j++)
    {
	printf("%6.2f ",
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
