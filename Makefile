ARCH	= `uname -s`

OBJS	= arch/$(ARCH)/getload.o

# for the things in the sub-directory:
INCLUDES	= -I${PWD}

# Hopefully you don't need this:
OTHER_FLAGS	= -DNEED_LOCAL_HEADERS

CFLAGS	= $(INCLUDES) $(OTHER_FLAGS)

default:
	make ttyload ARCH=`uname -s`

ttyload.c:	ttyload.h

ttyload: $(OBJS) ttyload.o

clean:
	rm -f *.o $(OBJS)
