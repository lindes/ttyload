/* the important (to me, anyway) bits of sys/loadavg.h: */

int getloadavg(double loadavg[], int nelem);

#define	LOADAVG_1MIN	0
#define	LOADAVG_5MIN	1
#define	LOADAVG_15MIN	2
